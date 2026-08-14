# Milestone 2 acceptance evidence — provenance

Every variant here was entered exclusively through the provenance-disciplined converter pipeline (Milestone 1 Decision 7a): canonical MusicXML source → Kernel enrichment (`jims_api::jims_musicxml_enrich_v1`) → stock MuseScore import of the same arrangement → `tools/jims/enriched_to_jims_mscx.py` → fork render. No `.mscx` was hand-authored; no musical fact was computed outside the Kernel.

## Pieces and tunings

| Variant | Piece | Tuning (M5 width) | Tonic-extent token |
|---|---|---|---|
| `ode-to-joy-12tet` / `-19tet` | Ode to Joy, Do-mode | 700¢ / 1200·11⁄19 ≈ 694.74¢ | tonic-bounded |
| `grym-12tet` / `-17tet` | God Rest Ye Merry Gentlemen, La-mode | 700¢ / 1200·10⁄17 ≈ 705.88¢ | tonic-centered |
| `acc-chromatic-12tet` / `-17tet` | Five-class fixture + full chromatic run | 700¢ / ≈705.88¢ | tonic-bounded / tonic-centered (tuning-true classifier) |
| `collision-12tet` / `-19tet` | D♯/E♭ collision dyad + dense second-cluster chord | 700¢ / ≈694.74¢ | tonic-bounded |
| `mode-change-12tet` / `-19tet` | Eight bars, Do-mode → La-mode at bar 5 (`JIMS_MODE_CHANGE=5:5`), indicator relocating across the system break | 700¢ / ≈694.74¢ | tonic-bounded per span |

Coverage rule satisfied: 12-TET ×5, 17-TET ×2, 19-TET ×3 — both non-12 tunings exercised more than once.

## Per-variant files

- `enriched.musicxml` — the canonical Kernel-enriched source (`urn:jims:musicxml:1` identities).
- `<variant>.mscx` — the converter's output (JiMStaffStateV2 state JSON with first-class `tonic_extent`; no side tag).
- `initial-1.png` — the fork's render of that file.
- `invariants.json` — machine-readable report: every note's `(nPer, nGen)` identity and Kernel cents above the lower Do, round-trip identity survival, no-accidentals-after-reload, and the initial-vs-reloaded render SHA-256 byte comparison (all ten variants byte-identical).

## Command shape (per variant)

```
enrich-runner <source>.musicxml state.json > enriched.musicxml
mscore <source>.musicxml -o stock.mscx
JIMS_RUNNER=enrich-runner [JIMS_MODE_CHANGE=m:r] python3 tools/jims/enriched_to_jims_mscx.py enriched.musicxml stock.mscx <variant>.mscx <lines>
mscore <variant>.mscx -o initial.png
mscore <variant>.mscx -o roundtrip.mscx && mscore roundtrip.mscx -o reloaded.png
shasum -a 256 initial-1.png reloaded-1.png
```

Renders were executed from a clean working directory (no adjacent style files; see the Phase 4 investigation note about `score_style.mss`).
