# JiMStaff Milestone 9 — SATB open score — sensory evidence provenance

Automated sensory evidence for M9 (owner-accepted plan `Plans/Add_JiMS_to_MuseScore/M9_SATB_OpenScore_FINAL.md`, 2026-08-22): the two fixtures of record rendered by the installed MuseScore bundle, twice each from clean scratch directories, and checked by a committed pixel verifier. Jim's by-eye acceptance in the app is the final gate; this folder is the machine evidence that precedes it.

Visual rendering is the whole sensory dimension of M9 — there is no audio dimension in this milestone.

## What is here

- `render_evidence.sh` — the committed render driver. Renders each fixture twice, from a fresh scratch directory per render, into `renders/` (`<fixture>-p<k>.png`, second render `<fixture>-2-p<k>.png`), then writes `renders/SHA256SUMS`. sha256 `a44a58113844e6a9f21800cfa732af230adc4fe74fa23e5ab579103bf990d314`.
- `verify_satb_layout.py` — the committed pixel verifier (Pillow + numpy from `/Users/jim/Developer/JiMS/.venv`, deterministic RGB decode). It fails unless: every render matches `SHA256SUMS`; every repeated render is byte-identical to its first; every rendered system draws four JiMStaff frames; every system has at least one measure barline running through all four frames; and **no measure barline carries any ink in any of the three inter-staff gaps**.
- `verify-summary.json` — the verifier's machine-readable verdict for the committed renders (`ok: true`).
- `renders/` — 6 page PNGs (2 fixtures × 2 renders, the template spilling to 2 pages) plus `SHA256SUMS`.

## Fixtures of record

- **`template`** — the shipped empty template `share/templates/02-Choral/12-SATB_(JiMStaff)/12-SATB_(JiMStaff).mscx` (sha256 `41ec94f6e3872e7899b3def22febeb33cfe92401d208b2e8280b15d8eafd7729`): four JiMStaff staves in open score, 18 bars of measure rests, constellation extents `lower_do_register` 4/4/3/3 with `period_count` 1.
- **`hymn`** — `src/engraving/tests/jimstaff_data/m9-satb-hymn.mscx` (sha256 `b7e7762b0b1b43e33a0dc5844d86e26f5549ffb9b1042e6a12700e7019ef7a7c`): the first phrase of the **Old Hundredth**, four bars of 4/4, entered in all four parts, with lyrics on the Soprano and one dynamic.

### Hymn provenance, stated exactly

**The tune is public domain.** OLD HUNDREDTH (also OLD 100th, GENEVAN 134) is a Long Metre hymn tune from the second edition of the Genevan Psalter, *Pseaumes Octante Trois de David* (1551), attributed to Louis Bourgeois (c. 1510 – c. 1560). A 1551 melody by a composer who died c. 1560 is unambiguously out of copyright everywhere.

**The melody transcribed here is sourced, not remembered.** Hymnary.org's tune entry for OLD HUNDREDTH gives the melodic incipit `11765 12333 32143` in Hymnary's movable-do numeric notation, metre 8.8.8.8 (<https://hymnary.org/tune/old_hundredth_bourgeois>). The first line of a Long Metre tune is its first eight notes, so the phrase entered here is scale degrees **1 1 7 6 5 1 2 3**, written with Do = C in the octave above Do0: C5 C5 B4 A4 G4 C5 D5 E5. Background on the tune's date, attribution and metre: <https://en.wikipedia.org/wiki/Old_100th>.

**The lyric is public domain.** "All people that on earth do dwell" is William Kethe's metrical psalm paraphrase, first printed 1561.

**The three lower parts are NOT a transcription.** Soprano is the Bourgeois melody as sourced above. Alto, Tenor and Bass are a plain functional harmonisation written for this fixture by the M9 executor — chosen only to be singable, in each stock vocal part's amateur range, and free of voice crossing. They are not taken from any edition, are not claimed to be any historical harmonisation, and carry no third-party rights. Anyone comparing this render with a published Old Hundredth setting should expect the inner parts to differ.

## Renderer and commands

- Bundle: `build.install/mscore.app` (MuseScore4Development 4.7.4), built from `build.release` and installed with `cmake --install build.release`. Fork branch `claude/m9-satb`, base `ce964a7d4a21aaa61b6f461cf4c44963a91f6716`; the exact head SHA the evidence was regenerated at is recorded in the M9 final report. Kernel `GitHub/jims` `main` at `a9ad21108c3acd531598d6d7bf4fbc0f3014d9d5`. Qt 6.11.1 (Homebrew). macOS 26.5.2. `QT_QPA_PLATFORM=offscreen`.
- **Signature: none was applied, and none was needed.** `codesign` is not invoked anywhere in this milestone (owner constraint). The bundle is *linker-signed ad hoc* by the build itself — `codesign -dv` reports `flags=0x20002(adhoc,linker-signed)`, `Signature=adhoc` — and runs unsigned offscreen. This differs from the M8 bundle, which was explicitly ad-hoc signed; M9 needed no such step.
- Qt deviation from the M8 precedent: M8 rendered with the Qt 6.10.2 bundle from `build.qt610`. That build directory can no longer be configured — its VST3 SDK cache under `Temp/jimsynth-vst3-sdk-cache/` was removed during repository housekeeping, so CMake fails at `src/framework/vst/sdk/CMakeLists.txt`. M9 therefore renders with `build.release` (Qt 6.11.1). The known Qt 6.11.1 problem is a crash in MuseScore's *dialogs*; headless page rendering opens no dialog, and every render here is reproducible and byte-identical on repeat.
- Command per render, inside a fresh scratch directory: `QT_QPA_PLATFORM=offscreen mscore -r 120 -o page.png score.mscx`; page k becomes `renders/<fixture>[-2]-p<k>.png`. Image size 1020 × 1320 px (A4 at 120 dpi).
- Verify: `/Users/jim/Developer/JiMS/.venv/bin/python jims-evidence/m9-satb/verify_satb_layout.py > jims-evidence/m9-satb/verify-summary.json`, run from the fork root (exit 0).

## The broken-barline assertion, and what it excludes

MuseScore's octavo convention — and `ScoreOrder::setBracketsAndBarlines` re-forcing `barLineSpan=false` for the `voices` section — is that a barline stroke is drawn inside each vocal staff and never between them. That claim is not visible to code inspection, so the verifier makes it from the rendered page: for each system it locates every column whose ink fills at least 80% of all four staff frames, and requires that every such column right of the first measure division carries **zero** ink in all three inter-staff gaps.

Two things are deliberately excluded, both on principle rather than convenience.

- **The system's leading verticals.** MuseScore draws a bracketed system's leading vertical (and the initial barline beside it) full height, through the gaps. This is system furniture, not a measure division. The **stock** `02-Choral/01-SATB` template renders identically — its system edge at x≈142/149 is one continuous ink run spanning all four staves, while its measure barline at x≈948 is twelve separate short runs (four staves × three systems). The verifier therefore classifies every full-height vertical left of the first measure division as the system edge and reports it, rather than asserting on it.
- **Ink that is not a barline.** Lyrics, note heads, stems and dynamics legitimately occupy the space between staves — the hymn's lyric line sits in the first gap. The gap assertion is therefore made per barline column, never over the whole gap area.

## Results (from `verify-summary.json`)

- `ok: true`, verifier exit code 0.
- SHA-256: 6 of 6 renders match `renders/SHA256SUMS`.
- Reproducibility: 3 of 3 repeat pages byte-identical to their first render.
- Systems checked: 8 (template page 1 = 1 system, template page 2 = 2 systems, hymn = 1 system, each fixture twice).
- Every system drew exactly 4 JiMStaff frames.
- Measure barlines per system: 7–11 columns; `gap_ink_in_barline_columns` is `[0]` for every system — no measure barline puts a single pixel between vocal staves.
- Stray spanning barlines: none, on any system, on any page.
- `broken_barlines_confirmed: true`.

## Live-render observations (by eye, from the committed renders)

- Four visible JiMStaff frames per system, labelled Soprano, Alto, Tenor, Bass.
- One bracket spanning all four staves.
- No clef drawn on any staff, and no key signature.
- Barlines visibly broken between every adjacent pair of vocal staves.
- On the empty template, the Kernel's tonic row labels read `C4:` on Soprano and Alto and `C3:` on Tenor and Bass — the constellation, realised in the v1 extent encoding as registers 4/4/3/3.
- On the hymn, each written staff shows its own melody-derived frame, so the four frames differ — correct behaviour, not a defect.
- Lyrics sit below the Soprano staff; the dynamic sits above it (the vocal-above rule).
- The lyric line clears the whole-period frame rather than hugging the note heads. This is the accepted cosmetic consequence of owner decision 3b and is recorded, not fixed, under this milestone.
- No obvious collisions.

**Range colouring is absent from these renders by design, not by omission.** MuseScore colours out-of-range note heads only when `!isPrinting`, so it is a screen-only affordance that can never appear in a printed page render. The seam's inputs are asserted instead by `Engraving_JiMStaffM9SATBTests.m9SweepRangeColouringInputsAreCorrectOnJimsVocalStaves`.

**Manual acceptance: pending Jim.**
