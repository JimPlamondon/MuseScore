#!/usr/bin/env python3
import hashlib
import json
import pathlib
import sys
import xml.etree.ElementTree as ET

HERE = pathlib.Path(__file__).resolve().parent
ROOT = HERE.parent.parent
FIXTURE = ROOT / "src/engraving/tests/jimstaff_data/ws-jims-common-tone-projection.mscx"
RENDERS = HERE / "renders"


def main():
    failures = []
    root = ET.parse(FIXTURE).getroot()
    measures = root.findall(".//Staff/Measure")
    notes = root.findall(".//Staff/Measure/voice/Chord/Note")
    identities = [
        (note.findtext("jimsNPer"), note.findtext("jimsNGen"))
        for note in notes
    ]
    if len(measures) != 2:
        failures.append(f"fixture has {len(measures)} measures, expected 2")
    if len(notes) != 2:
        failures.append(f"fixture has {len(notes)} notes, expected 2")
    if len(set(identities)) != 2:
        failures.append("the continuation does not carry a visibly distinct new-state identity")
    tie_spanners = root.findall(".//Spanner[@type='Tie']")
    if len(tie_spanners) != 2 or len(root.findall(".//Spanner[@type='Tie']/Tie")) != 1 \
            or len(root.findall(".//Spanner[@type='Tie']/prev")) != 1:
        failures.append("fixture does not contain one complete cross-boundary tie")

    first = RENDERS / "common-tone-1-p1.png"
    second = RENDERS / "common-tone-2-p1.png"
    if not first.is_file() or not second.is_file():
        failures.append("both repeated page renders are required")
        deterministic = False
    else:
        deterministic = first.read_bytes() == second.read_bytes()
        if not deterministic:
            failures.append("repeated renders are not byte-identical")

    summary = {
        "ok": not failures,
        "fixture": str(FIXTURE.relative_to(ROOT)),
        "measures": len(measures),
        "notes": len(notes),
        "identities": identities,
        "deterministic_render": deterministic,
        "render_sha256": hashlib.sha256(first.read_bytes()).hexdigest() if first.is_file() else None,
        "signal_oracle": "Engraving_JiMStaffM7PlaybackTests.syntheticCommonToneHasOneAttackAtOneExactFrequency",
        "failures": failures,
    }
    print(json.dumps(summary, indent=2))
    return 0 if summary["ok"] else 1


if __name__ == "__main__":
    sys.exit(main())
