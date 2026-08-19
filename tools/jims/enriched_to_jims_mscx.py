#!/usr/bin/env python3
# SPDX-FileCopyrightText: 2026 Jim Plamondon
# SPDX-License-Identifier: GPL-3.0-only
"""Transcribe JiMS-enriched MusicXML identities into a fork .mscx score.

Pure format transcription (JiMStaff Milestone 1, Decision 7 pipeline): the
lattice identities and every staff state were derived by the JiMS Kernel
inside the enriched MusicXML (urn:jims:musicxml:1, :2, or :3); this script
computes NO musical facts. It takes (a) the enriched MusicXML and (b) the
.mscx produced by the fork's stock MusicXML import of the same arrangement,
and:
  1. rewrites the score's StaffType to the JiMS variant (jims12tet,
     carrying the first staff-state JSON from the enriched file, with one
     extra stacked period when the melody needs it),
  2. for every LATER staff-state in the enriched file (V3 change events,
     owner decisions 2026-08-16; the V2 second-staff-state convention),
     inserts a StaffTypeChange carrying that complete state at the measure
     that declares it — the change carrier is built from full states only;
     the Kernel-written jims:change summary is never inspected,
  3. injects each note's jimsNPer/jimsNGen tags in document order.

Usage: enriched_to_jims_mscx.py <enriched.musicxml> <in.mscx> <out.mscx> <lines>

Hand-authored fixtures (no XML timeline) may still declare a change through
JIMS_MODE_CHANGE / JIMS_KEY_CHANGE / JIMS_SCALE_CHANGE / JIMS_REFERENCE;
those overrides are rejected when the enriched file itself declares more
than one state, so there is never a second authority.
"""
import json
import os
import re
import sys
import xml.etree.ElementTree as ET

JIMS_NAMESPACES = ("urn:jims:musicxml:1", "urn:jims:musicxml:2", "urn:jims:musicxml:3")


def jims_namespace(root):
    """The JiMS namespace the document uses (V1, V2, or V3)."""
    for element in root.iter():
        if isinstance(element.tag, str) and element.tag.startswith("{urn:jims:musicxml:"):
            uri = element.tag[1:element.tag.index("}")]
            if uri not in JIMS_NAMESPACES:
                sys.exit(f"ERROR: unsupported JiMS namespace {uri}")
            return uri
    sys.exit("ERROR: the enriched file carries no JiMS namespace")


def parse_reference(element, ns):
    """The Kernel's StaffReferenceV1 JSON for a jims:reference element (or None -> \"none\").

    Absent (V1/V2) and the explicit V3 jims:none both mean the placement-only
    reference; the other three forms transcribe verbatim.
    """
    if element is None:
        return "none"
    children = [c for c in element if isinstance(c.tag, str)]
    if len(children) != 1:
        sys.exit("ERROR: jims:reference must carry exactly one form")
    child = children[0]
    local = child.tag[len(ns) + 2:]
    if local == "none":
        return "none"
    if local == "reference-pitch":
        return {"reference-pitch": {"key_number": int(child.get("key-number"))}}
    if local == "pitch-class":
        return {"pitch-class": {"pitch_class": int(child.text.strip())}}
    if local == "frequency-hz":
        return {"frequency-hz": {"hertz": float(child.text.strip())}}
    sys.exit(f"ERROR: unknown jims:reference form {local}")


def staff_state_json(element, ns):
    """Rebuild the JiMStaffStateV2 JSON from one jims:staff-state element."""
    q = lambda name: f"{{{ns}}}{name}"
    text = lambda name: element.find(q(name)).text.strip()
    embedding = element.find(q("embedding"))
    extent = element.find(q("extent"))
    state = {
        "scale": [step.text.strip() for step in element.find(q("scale")).findall(q("step"))],
        "collection_rotation": int(text("collection-rotation")),
        "mode_rotation": int(text("mode-rotation")),
        "generator_cents": float(text("generator-cents")),
        "period_cents": float(text("period-cents")),
        "embedding": {"large_steps": int(embedding.get("large-steps")), "small_steps": int(embedding.get("small-steps"))},
        "extent": {"lower_do_register": int(extent.get("lower-do-register")), "period_count": int(extent.get("period-count"))},
        "reference": parse_reference(element.find(q("reference")), ns),
    }
    tonic_ambit = element.find(q("tonic-ambit"))
    if tonic_ambit is None:
        tonic_ambit = element.find(q("tonic-extent"))   # legacy spelling (owner rename 2026-08-19)
    if tonic_ambit is not None and tonic_ambit.text:
        state["tonic_ambit"] = tonic_ambit.text.strip()
    return state


def read_enriched(path):
    """(namespace, [(measure_index, state)...], [(nPer, nGen)...]) in document order.

    The timeline lists every jims:staff-state of the first part with the
    one-based document-order index of the measure that declares it. The
    Kernel-written jims:change (V3) is deliberately never read: the carrier
    is built from full states only.
    """
    root = ET.parse(path).getroot()
    ns = jims_namespace(root)
    parts = root.findall("part")
    if len(parts) != 1:
        sys.exit(f"ERROR: expected exactly one part, found {len(parts)}")
    timeline = []
    identities = []
    for index, measure in enumerate(parts[0].findall("measure"), start=1):
        for attributes in measure.findall("attributes"):
            for state in attributes.findall(f"{{{ns}}}staff-state"):
                timeline.append((index, staff_state_json(state, ns)))
        for note in measure.findall("note"):
            jp = note.find(f"{{{ns}}}pitch")
            if jp is not None:
                identities.append((jp.get("n-per"), jp.get("n-gen")))
    if not timeline:
        sys.exit("ERROR: the enriched file declares no jims:staff-state")
    if timeline[0][0] != 1:
        sys.exit("ERROR: the first jims:staff-state must sit in measure 1")
    return ns, timeline, identities


def main(enriched_path, mscx_in, mscx_out, lines):
    mscx = open(mscx_in, encoding="utf-8").read()
    ns, timeline, identities = read_enriched(enriched_path)
    state = timeline[0][1]
    periods = max(1, (int(lines) - 1) // 12)
    state["extent"]["period_count"] = periods
    xml_timeline = len(timeline) > 1

    # Hand-authored fixture overrides (Milestone 5): JIMS_REFERENCE="<key_number>"
    # binds Re0 to a MIDI-style key number (the reference-pitch interop
    # anchor) in the BASE state; JIMS_MODE_CHANGE="<measure>:<mode_rotation>",
    # JIMS_KEY_CHANGE="<measure>:<nPer>:<nGen>" (the SHIFT-Do0-XxN gesture's
    # lattice delta; the reference pitch on Re0 shifts by its negative), and
    # JIMS_SCALE_CHANGE="<measure>:<collection_rotation>" compose into ONE
    # StaffTypeChange. They exist for fixtures without an XML timeline; when
    # the enriched file itself declares later states they are rejected
    # rather than merged — one authority per document.
    mode_change = os.environ.get("JIMS_MODE_CHANGE", "")
    key_change = os.environ.get("JIMS_KEY_CHANGE", "")
    scale_change = os.environ.get("JIMS_SCALE_CHANGE", "")
    reference = os.environ.get("JIMS_REFERENCE", "")
    if xml_timeline and (mode_change or key_change or scale_change or reference):
        sys.exit("ERROR: the enriched file declares its own state timeline; "
                 "JIMS_*_CHANGE / JIMS_REFERENCE overrides are not allowed alongside it")
    if reference:
        state["reference"] = {"reference-pitch": {"key_number": int(reference)}}
    state_text = json.dumps(state, separators=(",", ":"))

    # 1. StaffType: replace the imported standard staff type block.
    stafftype_re = re.compile(r'<StaffType group="pitched">.*?</StaffType>', re.S)
    jims_block = (
        '<StaffType group="pitched">\n'
        '        <name>jims12tet</name>\n'
        f'        <lines>{lines}</lines>\n'
        '        <clef>0</clef>\n'
        '        <keysig>0</keysig>\n'
        '        <ledgerlines>0</ledgerlines>\n'
        '        <jims>1</jims>\n'
        '        <jimsJiLines>1</jimsJiLines>\n'
        f'        <jimsStateJson>{state_text}</jimsStateJson>\n'
        '        </StaffType>'
    )
    mscx, n_st = stafftype_re.subn(jims_block, mscx, count=1)
    if n_st != 1:
        sys.exit("ERROR: expected exactly one pitched StaffType block")

    # Derive-and-save the tonic-ambit token (owner Q4 rider): a state that
    # already declares tonic_ambit (V2/V3 XML) keeps it; otherwise classify
    # via the Kernel when a runner is available; melodies wider than the
    # classifier's window keep whatever token the caller supplies via
    # JIMS_TONIC_TOKEN, else the field is omitted (degenerate frame).
    import subprocess, tempfile

    def derive_token(seg_identities, seg_state_text):
        """Kernel-classified tonic-ambit token for a melody segment."""
        runner = os.environ.get("JIMS_RUNNER", "")
        if not runner or not seg_identities:
            return ""
        melody = {"notes": [{"nPer": int(a), "nGen": int(b)} for a, b in seg_identities]}
        with tempfile.NamedTemporaryFile("w", suffix=".json", delete=False) as mf:
            json.dump(melody, mf)
            melody_path = mf.name
        with tempfile.NamedTemporaryFile("w", suffix=".json", delete=False) as sf:
            sf.write(seg_state_text)
            state_path = sf.name
        try:
            out = subprocess.run([runner, "tonic", melody_path, state_path],
                                 capture_output=True, text=True, timeout=60)
            return json.loads(out.stdout) if out.returncode == 0 else ""
        except Exception:
            return ""

    def with_token(seg_state_text, seg_token):
        stated = seg_state_text.rstrip()
        assert stated.endswith("}")
        return stated[:-1].rstrip().rstrip(",") + f',"tonic_ambit":"{seg_token}"}}'

    def stated_text(seg_state):
        """Compact JSON with tonic_ambit (when present) as the last field."""
        seg = dict(seg_state)
        token = seg.pop("tonic_ambit", "")
        text = json.dumps(seg, separators=(",", ":"))
        return with_token(text, token) if token else text, token

    # Measure boundaries in note counts, per enriched measure, for the
    # per-span tonic classification.
    measures = ET.parse(enriched_path).getroot().findall("part")[0].findall("measure")
    notes_per_measure = [len(m.findall(f"note/{{{ns}}}pitch")) for m in measures]
    def split_before(measure_index):
        return sum(notes_per_measure[:measure_index - 1])

    # The change carriers: from the XML timeline (every later state at its
    # measure), or from the override flags (one composed change).
    changes = []  # (measure_index, state dict)
    if xml_timeline:
        for measure_index, later in timeline[1:]:
            later = dict(later)
            later["extent"] = dict(later["extent"]); later["extent"]["period_count"] = periods
            changes.append((measure_index, later))
    else:
        change_measure, change_rotation = 0, 0
        key_delta = None
        scale_rotation = None
        for spec in (mode_change, key_change, scale_change):
            if spec:
                m = int(spec.split(":")[0])
                if change_measure and m != change_measure:
                    sys.exit("ERROR: JIMS_*_CHANGE flags must name the same measure")
                change_measure = m
        if mode_change:
            change_rotation = int(mode_change.split(":")[1])
        if key_change:
            _, n_per, n_gen = (int(x) for x in key_change.split(":"))
            key_delta = (n_per, n_gen)
            if not reference:
                sys.exit("ERROR: JIMS_KEY_CHANGE needs JIMS_REFERENCE=<key_number>")
        if scale_change:
            scale_rotation = int(scale_change.split(":")[1])
        if change_measure:
            state2 = json.loads(json.dumps(state))
            if mode_change:
                state2["mode_rotation"] = change_rotation
            if scale_rotation is not None:
                state2["collection_rotation"] = scale_rotation
            if key_delta is not None:
                # Re0's key number shifts by MINUS the interval Do0->XxN, in
                # 12-TET key-number steps: 12 per period, 7 per generator (the
                # reference-pitch anchor is a 12-TET interop anchor).
                n_per, n_gen = key_delta
                state2["reference"] = {"reference-pitch": {
                    "key_number": int(reference) - (12 * n_per + 7 * n_gen)}}
            changes.append((change_measure, state2))

    first_split = split_before(changes[0][0]) if changes else len(identities)
    base_text, base_token = stated_text(state)
    token = base_token or os.environ.get("JIMS_TONIC_TOKEN", "") or derive_token(identities[:first_split], state_text)
    if token and not base_token:
        mscx = mscx.replace(f"<jimsStateJson>{state_text}</jimsStateJson>",
                            f"<jimsStateJson>{with_token(state_text, token)}</jimsStateJson>", 1)

    opens = list(re.finditer(r"<Measure[^>]*>\s*", mscx))
    # Insert from the last change backwards so earlier offsets stay valid.
    for position, (change_measure, state2) in reversed(list(enumerate(changes))):
        state2_text, token2 = stated_text(state2)
        if not token2:
            next_split = split_before(changes[position + 1][0]) if position + 1 < len(changes) else len(identities)
            token2 = derive_token(identities[split_before(change_measure):next_split], state2_text) \
                or os.environ.get("JIMS_TONIC_TOKEN2", "") or token
            if token2:
                state2_text = with_token(state2_text, token2)
        stc_block = (
            '<StaffTypeChange>\n'
            '        <StaffType group="pitched">\n'
            '          <name>jims12tet</name>\n'
            f'          <lines>{lines}</lines>\n'
            '          <clef>0</clef>\n'
            '          <keysig>0</keysig>\n'
            '          <ledgerlines>0</ledgerlines>\n'
            '          <jims>1</jims>\n'
            '          <jimsJiLines>1</jimsJiLines>\n'
            f'          <jimsStateJson>{state2_text}</jimsStateJson>\n'
            '          </StaffType>\n'
            '        </StaffTypeChange>\n'
            '      ')
        if change_measure < 1 or change_measure > len(opens):
            sys.exit(f"ERROR: change measure {change_measure} out of range")
        at = opens[change_measure - 1].end()
        mscx = mscx[:at] + stc_block + mscx[at:]

    # 1c. Engraving font (Milestone 3): a JiMS score selects the
    # Kernel-generated JiMSMusic outlines; every symbol JiMSMusic does
    # not carry falls back to the stock engraving font.
    if "<musicalSymbolFont>" not in mscx:
        if "<Style>" in mscx:
            mscx = mscx.replace("<Style>", "<Style>\n      <musicalSymbolFont>JiMSMusic</musicalSymbolFont>\n      <hideInstrumentNameIfOneInstrument>0</hideInstrumentNameIfOneInstrument>", 1)
        else:
            mscx = mscx.replace("<Score>", "<Score>\n    <Style>\n      <musicalSymbolFont>JiMSMusic</musicalSymbolFont>\n      <hideInstrumentNameIfOneInstrument>0</hideInstrumentNameIfOneInstrument>\n      </Style>", 1)

    # 2. Notes: inject the lattice identity tags in document order.
    notes = list(re.finditer(r"<Note>\n(\s*)", mscx))
    if len(notes) != len(identities):
        sys.exit(f"ERROR: {len(notes)} .mscx notes vs {len(identities)} enriched identities")
    out, cursor = [], 0
    for match, (n_per, n_gen) in zip(notes, identities):
        indent = match.group(1)
        insert_at = match.end()
        out.append(mscx[cursor:insert_at])
        out.append(f"<jimsNPer>{n_per}</jimsNPer>\n{indent}<jimsNGen>{n_gen}</jimsNGen>\n{indent}")
        cursor = insert_at
    out.append(mscx[cursor:])
    open(mscx_out, "w", encoding="utf-8").write("".join(out))
    print(f"OK: {len(identities)} identities, StaffType jims12tet lines={lines} periods={periods}")


if __name__ == "__main__":
    main(*sys.argv[1:5])
