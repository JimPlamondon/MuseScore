#!/bin/zsh
# JiMStaff Milestone 8 — render the three elision settings of the two-hand
# fixture with an installed MuseScore bundle, twice each from clean scratch
# directories, into ./renders (<setting>-p<k>.png, second render
# <setting>-2-p<k>.png; verified by verify_band_elision.py). Usage:
#   render_evidence.sh <path/to/mscore.app>
# The fixture of record is src/engraving/tests/jimstaff_data/m8-two-hand.mscx.
# All three variants are that file with its three system (line) breaks turned
# into PAGE breaks, so page k IS system k for every setting and the verifier
# can compare systems by page without any layout heuristics; the two ON
# variants additionally write the score style switches into a <Style> block.
# Elision off = the page-broken file with no style switch.
set -eu
BUNDLE="$1"
MSCORE="$BUNDLE/Contents/MacOS/mscore"
HERE="${0:A:h}"
FORK="${HERE:h:h}"
FIXTURE="$FORK/src/engraving/tests/jimstaff_data/m8-two-hand.mscx"
OUT="$HERE/renders"
SCRATCH="$(mktemp -d)"
export QT_QPA_PLATFORM=offscreen

python3 - "$FIXTURE" "$SCRATCH" <<'EOF'
import sys
fixture, scratch = sys.argv[1], sys.argv[2]
src = open(fixture).read()
def with_style(tags):
    block = "    <Style>\n" + "".join(f"      <{k}>{v}</{k}>\n" for k, v in tags) + "      </Style>\n"
    return src.replace("    <Division>480</Division>\n", "    <Division>480</Division>\n" + block, 1)
# One system per page: the fixture's line breaks become page breaks.
assert src.count("<subtype>line</subtype>") == 3, "fixture drift: expected three line breaks"
src = src.replace("<subtype>line</subtype>", "<subtype>page</subtype>")
open(f"{scratch}/off.mscx", "w").write(src)
open(f"{scratch}/on-first-system-all.mscx", "w").write(with_style([("jimsElideEmptyOctaves", 1)]))
open(f"{scratch}/on-first-system-banded.mscx", "w").write(
    with_style([("jimsElideEmptyOctaves", 1), ("jimsShowAllOctavesInFirstSystem", 0)]))
EOF

rm -rf "$OUT"; mkdir -p "$OUT"
for setting in off on-first-system-all on-first-system-banded; do
  for pass in 1 2; do
    dir="$SCRATCH/run$pass-$setting"; rm -rf "$dir"; mkdir -p "$dir"
    "$MSCORE" -r 120 -o "$dir/page.png" "$SCRATCH/$setting.mscx" > "$dir/log.txt" 2>&1
    suffix=""; [ "$pass" = 2 ] && suffix="-2"
    for k in 1 2 3 4; do
      cp "$dir/page-$k.png" "$OUT/$setting$suffix-p$k.png"
    done
  done
done
(cd "$OUT" && shasum -a 256 *.png | sort -k2 > SHA256SUMS)
rm -rf "$SCRATCH"
echo "renders in $OUT"; cat "$OUT/SHA256SUMS"
