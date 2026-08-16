#!/usr/bin/env python3
# SPDX-FileCopyrightText: 2026 Jim Plamondon
# SPDX-License-Identifier: GPL-3.0-only
"""Transcribe JiMS-enriched MusicXML identities into a fork .mscx score.

Pure format transcription (JiMStaff Milestone 1, Decision 7 pipeline): the
lattice identities were derived by the JiMS Kernel inside the enriched
MusicXML (urn:jims:musicxml:1); this script computes NO musical facts. It
takes (a) the enriched MusicXML and (b) the .mscx produced by the fork's
stock MusicXML import of the same arrangement, and:
  1. rewrites the score's StaffType to the JiMS variant (jims12tet,
     carrying the staff-state JSON from the enriched file, with one extra
     stacked period when the melody needs it),
  2. injects each note's jimsNPer/jimsNGen tags in document order.

Usage: enriched_to_jims_mscx.py <enriched.musicxml> <in.mscx> <out.mscx> <lines>
"""
import json
import re
import sys


def staff_state_json(enriched):
    """Rebuild the JiMStaffStateV1 JSON from the jims:staff-state element."""
    block = re.search(r"<jims:staff-state>(.*?)</jims:staff-state>", enriched, re.S).group(1)
    steps = re.findall(r"<jims:step>([^<]+)</jims:step>", block)
    def text(tag):
        return re.search(rf"<jims:{tag}>([^<]+)</jims:{tag}>", block).group(1)
    embedding = re.search(r'<jims:embedding large-steps="(\d+)" small-steps="(\d+)"/>', block)
    extent = re.search(r'<jims:extent lower-do-register="(-?\d+)" period-count="(\d+)"/>', block)
    state = {
        "scale": steps,
        "collection_rotation": int(text("collection-rotation")),
        "mode_rotation": int(text("mode-rotation")),
        "generator_cents": float(text("generator-cents")),
        "period_cents": float(text("period-cents")),
        "embedding": {"large_steps": int(embedding.group(1)), "small_steps": int(embedding.group(2))},
        "extent": {"lower_do_register": int(extent.group(1)), "period_count": int(extent.group(2))},
        "reference": "none",
    }
    return state


def main(enriched_path, mscx_in, mscx_out, lines):
    enriched = open(enriched_path, encoding="utf-8").read()
    mscx = open(mscx_in, encoding="utf-8").read()

    identities = re.findall(r'<jims:pitch n-per="(-?\d+)" n-gen="(-?\d+)"/>', enriched)
    state = staff_state_json(enriched)
    periods = max(1, (int(lines) - 1) // 12)
    state["extent"]["period_count"] = periods
    # Milestone 5: JIMS_REFERENCE="<key_number>" binds Re0 to a MIDI-style
    # key number (the reference-pitch interop anchor) in the BASE state, so
    # a later key change (a reference shift) is expressible.
    import os as _os
    if _os.environ.get("JIMS_REFERENCE", ""):
        state["reference"] = {"reference-pitch": {"key_number": int(_os.environ["JIMS_REFERENCE"])}}
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

    # Derive-and-save the tonic-extent token (owner Q4 rider): classify
    # via the Kernel when a runner is available; melodies wider than the
    # classifier's window keep whatever token the caller supplies via
    # JIMS_TONIC_TOKEN, else the field is omitted (degenerate frame).
    # V2: the token is a first-class field of the state JSON, not a
    # side tag; the Kernel returns the bare serialized token.
    import os, subprocess, tempfile

    def derive_token(seg_identities, seg_state_text):
        """Kernel-classified tonic-extent token for a melody segment."""
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
        return stated[:-1].rstrip().rstrip(",") + f',"tonic_extent":"{seg_token}"}}'

    # Mid-score mode change (acceptance piece 4): JIMS_MODE_CHANGE is
    # "<measure>:<mode_rotation>". The change measure gets a
    # StaffTypeChange carrying the rotated state; each state's token is
    # classified from the notes its span governs.
    mode_change = os.environ.get("JIMS_MODE_CHANGE", "")
    # Milestone 5 (change indicators): the same StaffTypeChange carrier
    # also expresses a KEY change (JIMS_KEY_CHANGE="<measure>:<nPer>:<nGen>",
    # the SHIFT-Do0-XxN gesture's lattice delta; the reference pitch on Re0
    # shifts by its negative) and a SCALE change (JIMS_SCALE_CHANGE=
    # "<measure>:<collection_rotation>", e.g. -3 for the Grey-notes parallel
    # minor). JIMS_REFERENCE="<key_number>" binds Re0 to a MIDI-style key
    # number (the reference-pitch interop anchor) — required for a key
    # change to be expressible. All changes at one measure compose into a
    # single StaffTypeChange.
    key_change = os.environ.get("JIMS_KEY_CHANGE", "")
    scale_change = os.environ.get("JIMS_SCALE_CHANGE", "")
    reference = os.environ.get("JIMS_REFERENCE", "")
    change_measure, change_rotation, split_at = 0, 0, len(identities)
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
        measures = re.findall(r"<measure[^>]*>(.*?)</measure>", enriched, re.S)
        split_at = sum(len(re.findall(r'<jims:pitch ', m))
                       for m in measures[:change_measure - 1])

    token = os.environ.get("JIMS_TONIC_TOKEN", "") or derive_token(identities[:split_at], state_text)
    if token:
        mscx = mscx.replace(f"<jimsStateJson>{state_text}</jimsStateJson>",
                            f"<jimsStateJson>{with_token(state_text, token)}</jimsStateJson>", 1)

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
        state2_text = json.dumps(state2, separators=(",", ":"))
        # Token for the new span: Kernel-classified when a runner is
        # available; otherwise JIMS_TONIC_TOKEN2, else the base token
        # (recorded as hand-set provenance for the fixture).
        token2 = derive_token(identities[split_at:], state2_text) \
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
        opens = list(re.finditer(r"<Measure[^>]*>\s*", mscx))
        if change_measure < 1 or change_measure > len(opens):
            sys.exit(f"ERROR: mode-change measure {change_measure} out of range")
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
