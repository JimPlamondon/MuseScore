# JiMStaff Milestone 10 — song-wide/per-staff sensory evidence

This bundle is the automated visual evidence for the accepted Milestone 10 plan. It renders the shipped empty SATB JiMStaff template and the Old Hundredth four-part excerpt twice each, from fresh working directories, using the task's Qt 6.10.2 MuseScore bundle.

`render_evidence.sh` writes the repeated page PNGs and `renders/SHA256SUMS`. `verify_m10_satb.py` requires every hash to match, each repeated page to be byte-identical, every detected SATB system to retain four JiMStaff frames and broken vocal barlines, and the four empty template extents to equal the Kernel-pinned Soprano, Alto, Tenor, and Bass defaults. The musical centre oracle itself is independently exercised through the live Kernel bridge by `Engraving_JiMStaffM10SATBTests.emptyVocalStavesUseKernelRangeDefaultsAndBassTracksTheTonic`.

Renderer: `build.m10.install/mscore.app`, MuseScore Studio Development 4.7.5, Qt 6.10.2 from `/Users/jim/Qt/6.10.2/macos`, with the bridge linked from `/Users/jim/Developer/JiMS/Temp/jims-codex-m10-songwide`. Command: `QT_QPA_PLATFORM=offscreen mscore -r 120 -o page.png score.mscx`.

Visual scope is deliberate: Milestone 10 has no audio acceptance dimension. The final full-resolution inspection checks every page for stray accidental-class shaped noteheads; the diatonic Old Hundredth excerpt must use plain oval noteheads throughout.

Execution result (2026-08-24): `render_evidence.sh` produced six PNGs. `verify_m10_satb.py` exited 0; all six SHA-256 entries verified; the three repeated pages were byte-identical; the pixel/layout checks passed on every page; and the four template extents matched the Kernel-pinned defaults. Full-resolution inspection of all six PNGs found no stray shaped notehead; every Old Hundredth notehead is a plain oval. The installed app passed `codesign --verify --deep --strict` with `Authority=JiMS Dev` and CDHash `64e75b54011c57906df2e7034fcfb3e7e4c1b6dd`.

Manual acceptance: pending Jim.
