#!/usr/bin/env bash
set -euo pipefail

here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
root="$(cd "$here/../.." && pwd)"
mscore="${1:-$root/build.release/src/app/mscore.app/Contents/MacOS/mscore}"
source_score="$root/src/engraving/tests/jimstaff_data/ws-jims-common-tone-projection.mscx"
renders="$here/renders"

[ -x "$mscore" ] || { echo >&2 "no MuseScore executable at $mscore"; exit 1; }
[ -f "$source_score" ] || { echo >&2 "missing generated common-tone fixture"; exit 1; }

rm -rf "$renders"
mkdir -p "$renders"
scratch_root="$(mktemp -d)"
trap 'rm -rf "$scratch_root"' EXIT

for run in 1 2; do
    scratch="$scratch_root/run-$run"
    mkdir -p "$scratch"
    cp "$source_score" "$scratch/score.mscx"
    (cd "$scratch" && QT_QPA_PLATFORM=offscreen "$mscore" -r 120 -o page.png score.mscx >/dev/null 2>&1)
    for page in "$scratch"/page-*.png; do
        number="$(basename "$page" .png)"
        number="${number#page-}"
        cp "$page" "$renders/common-tone-$run-p$number.png"
    done
done

(cd "$renders" && shasum -a 256 ./*.png | sed 's#\./##' > SHA256SUMS)
