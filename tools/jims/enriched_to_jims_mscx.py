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
        f'        <jimsStateJson>{state_text}</jimsStateJson>\n'
        '        </StaffType>'
    )
    mscx, n_st = stafftype_re.subn(jims_block, mscx, count=1)
    if n_st != 1:
        sys.exit("ERROR: expected exactly one pitched StaffType block")

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
