# JiMStaff Milestone 8 — octave-band elision ("hollow stacks") — sensory evidence provenance

Automated sensory evidence for M8 (owner-approved plan `Plans/Add_JiMS_to_MuseScore/M8_OctaveBandElision_Plan.md`, 2026-08-18): the two-hand fixture rendered at three settings by the installed MuseScore bundle, twice each, and checked by a committed pixel verifier. Human by-eye acceptance in the app is the final gate; this folder is the machine evidence that precedes it.

## What is here

- `render_evidence.sh` — renders the fixture of record at the three settings, twice each from clean scratch directories, into `renders/` (`<setting>-p<k>.png`, second render `<setting>-2-p<k>.png`, `SHA256SUMS`). The fixture's three system breaks are turned into page breaks so page k is system k for every setting (no layout heuristics needed to compare systems); the ON variants add the score style switches to a `<Style>` block. sha256 `fe423d696bb817280e6285300dc7e0a659cbc25e1743a4297b0afcbda7f82306`.
- `verify_band_elision.py` — the verifier (Pillow 12.3.0 + numpy 2.5.1 from `/Users/jim/Developer/JiMS/.venv`, deterministic RGB decode). It fails unless: repeated renders are byte-identical per page; off differs from each on-variant; system 1 is pixel-identical between off and on-first-system-all and differs for on-first-system-banded; every changed pixel lies inside the declared region (the OFF stack's area on that page, reported per system with bounding boxes); every banded system has exactly one gap (four Do lines, two whole-period bands) with zero ink in the gap's staff body (full height, right of the header column) and zero ink in the gap core across the full width — so no barline stroke, repeat dot, guide line, stem, label or the screen-only indicator reached the printed page. The header column's band-edge glyphs (the tonic-indicator ring and its "Do" label sit ON the edge Do lines and overlap the gap edge by a few pixels, exactly as they overlap the outer edges of a whole stack) are reported as expected, not counted. sha256 `cf384d0b6ca3310d71cbbafa69e06f6de4af2fc71f3c0fa378bda4675214dbd6`.
- `verify-summary.json` — the verifier's machine-readable result for the committed renders (`ok: true`).
- `renders/` — 24 page PNGs (3 settings × 2 renders × 4 pages) + `SHA256SUMS`.

## Fixture of record

`src/engraving/tests/jimstaff_data/m8-two-hand.mscx` (sha256 `150efd6749249458069f5f5c07968a49e3c1fed36331044be90a643d18c36347`): one JiMStaff (12-TET, extent lower Do register 4, Re0 = 62), eight 4/4 bars in four two-bar systems; voice 1 (RH) alternates D6 `(2,0)` and B6 `(1,3)` in frame period 2, voice 2 (LH) alternates D2 `(−2,0)` and A2 `(−2,1)` in frame period −2. Whole-piece frame = periods −2..2 (five whole periods, [−2400, 3600) ¢); with elision on, each system's Kernel banded frame is two whole-period bands ([−2400, −1200] ¢ labelled `C2`, [2400, 3600] ¢ labelled `C6`) with three periods omitted.

## Settings

| setting | score style |
|---|---|
| `off` | defaults (`jimsElideEmptyOctaves` false) — every system draws the whole stack |
| `on-first-system-all` | `jimsElideEmptyOctaves` 1 (first-system rule at its default: on) — system 1 whole, systems 2–4 banded |
| `on-first-system-banded` | `jimsElideEmptyOctaves` 1, `jimsShowAllOctavesInFirstSystem` 0 — every system banded |

## Renderer and commands

- Bundle: `build.install610.next/mscore.app` (Qt 6.10.2, `build.qt610`, MuseScore4Development 4.7.4), fork branch `claude/jimstaff-m8-band-elision` at the commit that adds this folder (see the final report for the head SHA), Kernel `main` `2ab8f3a477d24f9229fa457fa063aa7ec1d0e022` (PR 224 merged), macOS 26.5.2, `QT_QPA_PLATFORM=offscreen`.
- Signature: ad-hoc (`codesign --force --deep --sign -`; `codesign --verify --deep --strict --verbose=2` → "valid on disk / satisfies its Designated Requirement"). The `JiMS Dev` identity signing blocked on a keychain-access dialog in the unattended session — see the final report's blocker list.
- Command per render (inside a fresh scratch directory): `mscore -r 120 -o page.png <variant>.mscx`; page k → `renders/<setting>[-2]-p<k>.png`. Image size 1020 × 1320 px (A4 at 120 dpi).
- Verify: `/Users/jim/Developer/JiMS/.venv/bin/python jims-evidence/m8-band-elision/verify_band_elision.py > jims-evidence/m8-band-elision/verify-summary.json` (exit 0).

## Results (from `verify-summary.json`)

- Reproducibility: every page's second render is byte-identical to its first (12/12 page pairs).
- System 1: `off-p1.png` and `on-first-system-all-p1.png` are the same file (sha256 `eb8d2317a88be817…`, 0 changed pixels); `on-first-system-banded-p1.png` differs (55 936 changed pixels, bbox x 112–949, y 300–717, 0 outside the declared region).
- Systems 2–4: identical between the two ON variants (only system 1 differs between them, as designed); each differs from off by 59 048–60 119 pixels, all inside the declared region (bbox x 70–949, y 217–634); four Do lines and exactly one gap per banded system; gap staff-body ink 0, gap-core ink 0; header-edge glyph overlap 170 px (147 on the banded first system) — expected.
- The screen-only "n octaves elided" indicator (Phase 4) is absent from every printed page (gap-core ink 0), as the structural test `m8GapIndicatorIsScreenOnlyAndNeverPrints` also proves.

## Elision-off identity (separate from this folder)

The Phase-0 render manifest (27 PNG pages + 21 MusicXML exports of the pre-existing JiMS fixtures, 48 SHA-256, rendered by the pre-M8 bundle at `jims/main` `da21ec74ce`) was re-rendered twice by the final bundle: run 1 == run 2, and every hash equals its Phase-0 hash. Elision off is byte-identical to before M8 for every pre-existing fixture. (Manifests are recorded in the final report; the scratch renders themselves are not committed.)
