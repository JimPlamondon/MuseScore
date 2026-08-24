#!/usr/bin/env python3
"""Verify M10 SATB renders and the empty-template extent centres."""

import hashlib
import importlib.util
import json
import pathlib
import sys
import xml.etree.ElementTree as ET

HERE = pathlib.Path(__file__).resolve().parent
ROOT = HERE.parent.parent
RENDERS = HERE / "renders"
TEMPLATE = ROOT / "share/templates/02-Choral/12-SATB_(JiMStaff)/12-SATB_(JiMStaff).mscx"
M9_VERIFIER = HERE.parent / "m9-satb/verify_satb_layout.py"

EXPECTED_EXTENTS = [
    {"lower": {"nPer": 3, "nGen": -5}, "upper": {"nPer": 4, "nGen": -5}},
    {"lower": {"nPer": 2, "nGen": -4}, "upper": {"nPer": 3, "nGen": -4}},
    {"lower": {"nPer": 1, "nGen": -3}, "upper": {"nPer": 2, "nGen": -3}},
    {"lower": {"nPer": -4, "nGen": 4}, "upper": {"nPer": -3, "nGen": 4}},
]


def load_m9_verifier():
    spec = importlib.util.spec_from_file_location("m9_satb_verifier", M9_VERIFIER)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def main():
    summary = {"ok": True, "failures": [], "renders_dir": "jims-evidence/m10-satb/renders"}
    pages = sorted(RENDERS.glob("*.png"))

    sums = RENDERS / "SHA256SUMS"
    checked = 0
    if not sums.is_file():
        summary["failures"].append("renders/SHA256SUMS is missing")
    else:
        for line in sums.read_text().splitlines():
            digest, name = line.split()
            path = RENDERS / name
            if not path.is_file() or hashlib.sha256(path.read_bytes()).hexdigest() != digest:
                summary["failures"].append(f"SHA-256 mismatch: {name}")
            checked += 1
    summary["sha256_checked"] = checked

    repeats = []
    for path in pages:
        if "-2-p" not in path.name:
            continue
        first = RENDERS / path.name.replace("-2-p", "-p")
        identical = first.is_file() and first.read_bytes() == path.read_bytes()
        repeats.append({"page": path.name, "against": first.name, "identical": identical})
        if not identical:
            summary["failures"].append(f"repeat differs: {path.name}")
    summary["repeat_renders"] = repeats
    if not repeats:
        summary["failures"].append("no repeat renders")

    m9 = load_m9_verifier()
    page_checks = [m9.check_page(path) for path in pages]
    summary["pages_checked"] = page_checks
    for check in page_checks:
        summary["failures"].extend(f"{check['page']}: {failure}" for failure in check["failures"])

    states = [json.loads(node.text) for node in ET.parse(TEMPLATE).findall(".//jimsStateJson")]
    actual = [state.get("extent") for state in states]
    summary["empty_staff_centres"] = {
        "source": str(TEMPLATE.relative_to(ROOT)),
        "voices": ["soprano", "alto", "tenor", "bass"],
        "actual_extents": actual,
        "expected_kernel_defaults": EXPECTED_EXTENTS,
        "centred_correctly": actual == EXPECTED_EXTENTS,
    }
    if actual != EXPECTED_EXTENTS:
        summary["failures"].append("empty SATB extents do not match the Kernel-pinned vocal defaults")

    summary["ok"] = not summary["failures"]
    print(json.dumps(summary, indent=2))
    return 0 if summary["ok"] else 1


if __name__ == "__main__":
    sys.exit(main())
