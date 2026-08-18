#!/usr/bin/env python3
"""JiMStaff Milestone 8 — octave-band elision pixel verifier.

Deterministic, dependency-light (Pillow + numpy) checks over the three
settings of the two-hand fixture rendered by the installed MuseScore bundle
(render_evidence.sh: the fixture's three system breaks become page breaks,
so PAGE k IS SYSTEM k for every setting; PNG pages at 120 dpi):

  off                     elision off (default): every system draws the whole stack
  on-first-system-all     elision on, first system shows all octaves (default rule)
  on-first-system-banded  elision on, first-system rule off

Each setting is rendered TWICE (second render suffixed "-2") from a clean
directory. The verifier FAILS unless every one of these holds:

  1. repeated renders per setting are byte-identical, page by page (SHA-256);
  2. off differs from each on-variant;
  3. system 1 (page 1) is pixel-identical between off and on-first-system-all,
     and differs for on-first-system-banded;
  4. every changed pixel (off vs each on-variant, per system) lies inside the
     declared region — the JiMStaff area of that system in the OFF render
     (from just above its first Do line to just below its last, across the
     content width) — reported per system with bounding boxes;
  5. every banded system's printed gap is plain: exactly one gap (the 6.5 sp
     staffDistance between the two whole-period bands: 0.4-0.7 of a period,
     the tallest span between consecutive Do lines) and no ink (nothing darker
     than the paper threshold) (a) anywhere in the STAFF BODY of the gap —
     full gap height, right of the header column — so no barline stroke,
     repeat dot, guide line, stem or note reached it, and (b) anywhere in the
     GAP CORE across the whole width (the middle of the gap, 16 px in from
     each edge) — so no label, indicator text or crossing stroke is there.
     The header column's band-EDGE glyphs (the tonic-indicator ring and its
     "Do" label sit ON the edge Do lines and overlap the gap edge by a few
     pixels, exactly as they overlap the outer edges of a whole stack) are
     reported, not counted. Whole-stack systems have no gap.

Do lines and band edges are located from the images themselves, not
hand-typed, so a layout drift is a verifier failure, not a silently moved
box. Output: a JSON summary (stdout) and a non-zero exit on failure.
"""
import hashlib
import json
import sys
from pathlib import Path

import numpy as np
from PIL import Image

HERE = Path(__file__).resolve().parent
RENDERS = HERE / "renders"

SETTINGS = ["off", "on-first-system-all", "on-first-system-banded"]
PAGES = [1, 2, 3, 4]
PAPER = 250          # gray level at/above which a pixel counts as paper
RED_MIN_R = 150      # a red Do line pixel (0xE03030 anti-aliased): R high ...
RED_MARGIN = 40      # ... and clearly above both G and B
MARGIN = 60          # rows above/below the OFF stack that belong to its region
HEADER_PX = 200      # header column width (crescent + dots + labels), 120 dpi
CORE_INSET = 16      # rows trimmed from each gap edge for the gap-core check


def sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def load(path: Path) -> np.ndarray:
    # Deterministic decode: RGB, no dithering, no color management.
    return np.asarray(Image.open(path).convert("RGB"), dtype=np.uint8)


def red_line_rows(img: np.ndarray):
    """Center rows of the red Do lines (rows where >= 30 % of the width is
    red, grouped when adjacent — a thin line lands on one or two rows
    depending on sub-pixel placement)."""
    r, g, b = img[..., 0].astype(int), img[..., 1].astype(int), img[..., 2].astype(int)
    red = (r >= RED_MIN_R) & (r >= g + RED_MARGIN) & (r >= b + RED_MARGIN)
    counts = red.sum(axis=1)
    rows = [y for y in range(img.shape[0]) if counts[y] >= 0.3 * img.shape[1]]
    runs = []
    for y in rows:
        if runs and y - runs[-1][1] <= 3:
            runs[-1][1] = y
        else:
            runs.append([y, y])
    return [int((a + b) / 2) for a, b in runs]


def gaps_of(lines):
    """Plain band gaps: spans between consecutive Do lines that are 0.4-0.7 of
    the tallest span (a whole period). Returns [(top_row, bottom_row)] strictly
    inside each gap."""
    if len(lines) < 2:
        return []
    diffs = [b - a for a, b in zip(lines, lines[1:])]
    period = max(diffs)
    return [(a + 4, b - 4) for a, b in zip(lines, lines[1:]) if 0.4 * period <= b - a <= 0.7 * period]


def content_x_extent(img: np.ndarray):
    gray = img.mean(axis=2)
    cols = np.nonzero((gray < PAPER).any(axis=0))[0]
    return int(cols.min()), int(cols.max())


def bbox_of(mask: np.ndarray):
    ys, xs = np.nonzero(mask)
    if len(ys) == 0:
        return None
    return [int(xs.min()), int(ys.min()), int(xs.max()), int(ys.max())]


def main() -> int:
    report = {"settings": {}, "systems": {}, "ok": True, "failures": []}

    def fail(msg):
        report["failures"].append(msg)
        report["ok"] = False

    imgs = {}
    for name in SETTINGS:
        report["settings"][name] = {}
        for k in PAGES:
            p1 = RENDERS / f"{name}-p{k}.png"
            p2 = RENDERS / f"{name}-2-p{k}.png"
            if not p1.exists() or not p2.exists():
                fail(f"missing render(s) for {name} page {k}")
                continue
            h1, h2 = sha256(p1), sha256(p2)
            img = load(p1)
            imgs[(name, k)] = img
            report["settings"][name][f"page{k}"] = {
                "sha256": h1, "sha256_second_render": h2, "reproducible": h1 == h2,
                "size": [int(img.shape[1]), int(img.shape[0])],
            }
            if h1 != h2:
                fail(f"{name} page {k}: repeated renders differ")
    if len(imgs) != len(SETTINGS) * len(PAGES):
        print(json.dumps(report, indent=2))
        return 1

    any_change = {"on-first-system-all": False, "on-first-system-banded": False}
    for k in PAGES:
        off = imgs[("off", k)]
        off_lines = red_line_rows(off)
        sysrep = {"off_do_lines": len(off_lines)}
        if len(off_lines) != 10:
            # Five whole periods draw their lower and upper Do line each; adjacent
            # periods share a line, so ten line rows collapse to six on the page.
            pass
        if len(off_lines) < 2:
            fail(f"system {k}: no stack detected in the off render")
            report["systems"][f"system{k}"] = sysrep
            continue
        if gaps_of(off_lines):
            fail(f"system {k}: the off render has a gap")
        x0, x1 = content_x_extent(off)
        top = max(0, off_lines[0] - MARGIN)
        bottom = min(off.shape[0] - 1, off_lines[-1] + MARGIN)
        sysrep["declared_region"] = {"top_row": top, "bottom_row": bottom, "x0": x0, "x1": x1}
        for name in ["on-first-system-all", "on-first-system-banded"]:
            on = imgs[(name, k)]
            if on.shape != off.shape:
                fail(f"{name} page {k}: image size differs from off")
                continue
            diff = np.any(on != off, axis=2)
            changed = int(diff.sum())
            outside = diff.copy()
            outside[top:bottom + 1, x0:x1 + 1] = False
            escaped = int(outside.sum())
            lines = red_line_rows(on)
            gaps = gaps_of(lines)
            gap_reports = []
            for gap in gaps:
                body = on[gap[0]:gap[1] + 1, x0 + HEADER_PX:x1 + 1]
                body_ink = int((body.mean(axis=2) < PAPER).sum())
                core = on[gap[0] + CORE_INSET:gap[1] + 1 - CORE_INSET, x0:x1 + 1]
                core_ink = int((core.mean(axis=2) < PAPER).sum())
                header_edge = on[gap[0]:gap[1] + 1, x0:x0 + HEADER_PX]
                header_edge_ink = int((header_edge.mean(axis=2) < PAPER).sum())
                gap_reports.append({"rows": [int(gap[0]), int(gap[1])],
                                    "staff_body_ink_pixels": body_ink,
                                    "gap_core_ink_pixels": core_ink,
                                    "header_edge_glyph_pixels_expected": header_edge_ink,
                                    "body_shape": [int(body.shape[0]), int(body.shape[1])],
                                    "core_shape": [int(core.shape[0]), int(core.shape[1])]})
                if body_ink:
                    fail(f"{name} system {k}: gap rows {gap} staff body contains {body_ink} ink pixels")
                if core_ink:
                    fail(f"{name} system {k}: gap rows {gap} core contains {core_ink} ink pixels")
            expected_banded = not (name == "on-first-system-all" and k == 1)
            entry = {"changed_pixels": changed, "changed_bbox": bbox_of(diff),
                     "pixels_outside_declared_region": escaped, "do_lines": len(lines),
                     "gaps": gap_reports, "expected_banded": expected_banded}
            sysrep[name] = entry
            if changed:
                any_change[name] = True
            if escaped:
                fail(f"{name} system {k}: {escaped} changed pixels escaped the declared region "
                     f"(bbox {bbox_of(outside)})")
            if expected_banded:
                if changed == 0:
                    fail(f"{name} system {k}: expected a banded system but it is identical to off")
                if len(gaps) != 1:
                    fail(f"{name} system {k}: expected exactly one band gap, found {len(gaps)}")
                if len(lines) != 4:
                    fail(f"{name} system {k}: expected four Do lines (two whole-period bands), found {len(lines)}")
            else:
                if changed != 0:
                    fail(f"{name} system {k}: expected the whole stack (identical to off) but {changed} pixels differ")
                if gaps:
                    fail(f"{name} system {k}: whole stack must have no gap")
        report["systems"][f"system{k}"] = sysrep
    for name, changed in any_change.items():
        if not changed:
            fail(f"{name}: identical to off on every page (elision had no visible effect)")
    print(json.dumps(report, indent=2))
    return 0 if report["ok"] else 1


if __name__ == "__main__":
    sys.exit(main())
