# JiMStaff Milestone 10 — song-wide/per-staff sensory evidence

This bundle is the automated visual evidence for the accepted Milestone 10 plan. It renders the shipped empty SATB JiMStaff template and the Old Hundredth four-part excerpt twice each, from fresh working directories, using the task's Qt 6.10.2 MuseScore bundle.

`render_evidence.sh` writes the repeated page PNGs and `renders/SHA256SUMS`. `verify_m10_satb.py` requires every hash to match, each repeated page to be byte-identical, every detected SATB system to retain four JiMStaff frames and broken vocal barlines, the four empty template extents to equal the Kernel-pinned Soprano, Alto, Tenor, and Bass defaults, and the rendered red Do row in every empty staff to land at the Kernel-pinned offset within its arbitrary extent. The musical centre and periodic-origin oracles are independently exercised through the live Kernel bridge by `Engraving_JiMStaffM10SATBTests.everyEmptyVocalStaffUsesItsKernelRangeCentre`.

Renderer: `build.satb-centers.install/mscore.app`, MuseScore Studio Development 4.7.5, Qt 6.10.2 from `/Users/jim/Qt/6.10.2/macos`, with the bridge linked from `/Users/jim/Developer/JiMS/Temp/jims-codex-empty-satb-range-centers`. Command: `QT_QPA_PLATFORM=offscreen mscore -r 120 -o page.png score.mscx`.

Visual scope is deliberate: Milestone 10 has no audio acceptance dimension. The final full-resolution inspection checks every page for stray accidental-class shaped noteheads; the diatonic Old Hundredth excerpt must use plain oval noteheads throughout.

Corrective execution result (2026-08-24): `render_evidence.sh` produced six PNGs. `verify_m10_satb.py` exited 0; all six SHA-256 entries verified; the three repeated pages were byte-identical; the pixel/layout checks passed on every page; the four template extents matched the uniform Kernel range-centred defaults; and every detected Do row matched its expected 900¢, 200¢, 700¢, or 400¢ offset for Soprano, Alto, Tenor, or Bass. Full-resolution inspection found the four distinct empty-staff positions and no stray shaped notehead; every Old Hundredth notehead is a plain oval. The installed app passed `codesign --verify --deep --strict` with `Authority=JiMS Dev` and CDHash `78d786d2bd2a8dd1e7e8eb6912b8bacab04aeb81`.

Manual acceptance: pending Jim.
