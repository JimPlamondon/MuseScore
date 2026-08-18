# JiMStaff Milestone 8 — octave-band elision ("hollow stacks") — sensory evidence provenance

Automated sensory evidence for M8 (owner-approved plan `Plans/Add_JiMS_to_MuseScore/M8_OctaveBandElision_Plan.md`, 2026-08-18): the two-hand fixture rendered at three settings by the installed MuseScore bundle, twice each, and checked by a committed pixel verifier. Human by-eye acceptance in the app is the final gate; this folder is the machine evidence that precedes it.

## What is here

- `render_evidence.sh` — renders the fixture of record at the three settings, twice each from clean scratch directories, into `renders/` (`<setting>-p<k>.png`, second render `<setting>-2-p<k>.png`, `SHA256SUMS`). The fixture's three system breaks are turned into page breaks so page k is system k for every setting (no layout heuristics needed to compare systems); the ON variants add the score style switches to a `<Style>` block. sha256 `fe423d696bb817280e6285300dc7e0a659cbc25e1743a4297b0afcbda7f82306`.
- `verify_band_elision.py` — the verifier (Pillow 12.3.0 + numpy 2.5.1 from `/Users/jim/Developer/JiMS/.venv`, deterministic RGB decode). It fails unless: repeated renders are byte-identical per page; off differs from each on-variant; system 1 is pixel-identical between off and on-first-system-all and differs for on-first-system-banded; every changed pixel lies inside the declared region (the OFF stack's area on that page, reported per system with bounding boxes); every banded system has exactly one gap (four Do lines, two whole-period bands) whose staff body (right of the header column) contains ink only in barline stroke columns — at least one continuous stroke through the gap per banded system, per the owner's keyboard-precedent ruling 3b of 2026-08-18 (a piano's barlines run between its staves) — and nothing else (no repeat dot, guide line, stem, note, label or the screen-only indicator); the header column's brace (which joins the bands, ruling 3b) and the band-edge tonic ring/label overlap are reported, not counted.
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

- Bundle: `build.install610.next/mscore.app` (Qt 6.10.2, `build.qt610`, MuseScore4Development 4.7.4), fork branch `claude/jimstaff-m8-findings` at the commit that regenerates this folder (see the final report for the head SHA), Kernel `main` `ea23bd32f8` (PR 224 + PR 233 merged), macOS 26.5.2, `QT_QPA_PLATFORM=offscreen`.
- Signature: ad-hoc (`codesign --force --deep --sign -`; `codesign --verify --deep --strict --verbose=2` → "valid on disk / satisfies its Designated Requirement"). The `JiMS Dev` identity signing blocked on a keychain-access dialog in the unattended session — see the final report's blocker list.
- Command per render (inside a fresh scratch directory): `mscore -r 120 -o page.png <variant>.mscx`; page k → `renders/<setting>[-2]-p<k>.png`. Image size 1020 × 1320 px (A4 at 120 dpi).
- Verify: `/Users/jim/Developer/JiMS/.venv/bin/python jims-evidence/m8-band-elision/verify_band_elision.py > jims-evidence/m8-band-elision/verify-summary.json` (exit 0).

## Results (from `verify-summary.json`, findings-batch renders)

- Reproducibility: every page's second render is byte-identical to its first (12/12 page pairs).
- System 1: `off-p1.png` and `on-first-system-all-p1.png` are the same file (0 changed pixels); `on-first-system-banded-p1.png` differs (65 500 changed pixels, 0 outside the declared region).
- Systems 2–4: identical between the two ON variants (only system 1 differs between them, as designed); each differs from off by 66 987–69 105 pixels, all inside the declared region; four Do lines and exactly one gap per banded system; 2 barline strokes run through each gap on systems 1–3 (mid-bar and end barline) and 3 on system 4 (final barline is double); 0 stray ink pixels in the gap's staff body; the header column carries the brace and the band-edge glyphs (290–315 px, expected).
- The screen-only "N empty octaves hidden" indicator (Phase 4; wording and placement per owner finding 2026-08-18) is absent from every printed page (0 stray gap ink), as the structural test `m8GapIndicatorIsScreenOnlyAndNeverPrints` also proves.
- Whole-stack pages label their lowest Do row `C2: Do` (owner finding 2: the label names the octave of the row it sits on); banded pages `C6:`/`C2:`.

## Elision-off renders versus the pre-M8 baseline (separate from this folder)

The Phase-0 render manifest (27 PNG pages + 21 MusicXML exports of the pre-existing JiMS fixtures, 48 SHA-256, rendered by the pre-M8 bundle at `jims/main` `da21ec74ce`) was byte-identical through M8 Phases 2–5 (`46b3102cb5`). The findings batch (owner decisions 1a/2a, 2026-08-18) deliberately re-baselines it: the new bundle's renders are reproducible (run 1 == run 2) and 16 of the 48 artifacts differ — the multi-octave stacks (proper minimum system/staff distances now that the JiMStaff's system height is its frame height) and stacks whose lowest period is below the extent's (their `[PitchN]:` now names its own row's octave); the three changed MusicXML exports (`m5-syshead`, `m6-gate`, `m7-gate`) differ only in `<system-distance>` / `<print new-page>` layout metadata, not in any JiMS content (MusicXML on/off byte-identity still holds: `m8ElisionSwitchesNeverChangeMusicXmlExport`). Manifests are recorded in the final report; the scratch renders themselves are not committed.
