# JiMStaff Milestone 1 acceptance evidence — provenance

One physical line per paragraph.

## Happy Birthday arrangement

- Title: "Happy Birthday" (monophonic piano lead line, 25 pitched notes, C major, 3/4).
- Source: `keyinsight-core/assets/pieces/happy-birthday.musicxml` in https://github.com/larsbrubaker/KeyInSight (git blob d00b1e410645fefe010b4a103c888f4caaf86fcb, retrieved 2026-08-13T12:58Z).
- Arrangement license: MIT (repository LICENSE, Copyright (c) 2026 Lars Brubaker) — verified separately from the underlying song: the Hill sisters' 1893 melody is public domain (Warner/Chappell claim invalidated 2015).
- SHA-256 of the retrieved file: eb08bed0e1370b3471c9cc7bba82f4d1e8246021e0fee70e17341cf6937f6f70.
- Modifications: none to the source file; the JiMS profile copy (`happy-birthday-jims.musicxml`) was produced mechanically by the Kernel's `jims_musicxml_enrich_v1` converter (jims commit 6b5da61e / 1292b32b), which derived all 25 lattice identities; `hb-jims.mscx` is a pure format transcription of that enriched file over the fork's own stock MusicXML import (tools/jims/enriched_to_jims_mscx.py). No musical fact was computed outside the JiMS Kernel.

## Accidental fixture

- `accidental-fixture.musicxml` is an original 5-note test vector (C4, C#4, Bb4, F##4, Dbb4) written for this milestone; same enrichment/transcription pipeline.

## Renders

- `happy-birthday-jimstaff.png` and `accidental-fixture-jimstaff.png` are the fork's own CLI renders of the two `.mscx` scores; the Happy Birthday render is byte-identical (SHA-256 a6b9b297b9f63021989ef9813c822343b25d3a842409628a12df48e26f93463f) before and after a `.mscz` save/reload round-trip.

## Font

- `fonts/JiMSMusic.otf` + metadata are GENERATED artifacts (GEN-003): rebuild with `python3 tools/jims/jims_font_generator.py <jims>/Libraries/jims/crates/jims-staff/assets/glyphs jims-evidence/fonts` — deterministic (byte-identical rebuilds). Sources are the JiMS Kernel's rights-clean glyph SVGs only; no third-party outline. Not yet wired as the runtime engraving font (named follow-up); Milestone-1 rendering uses stock SMuFL shape noteheads plus construction-rule drawing for clef/indicator.
