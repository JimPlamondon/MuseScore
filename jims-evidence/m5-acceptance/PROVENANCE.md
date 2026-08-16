# Milestone 5 acceptance pieces — change indicators (owner notation rulings 2026-08-16)

Every piece was produced through the converter pipeline from an existing M2 acceptance piece's `enriched.musicxml` (Kernel-enriched MusicXML, `urn:jims:musicxml:2`), with the change carried by the existing `StaffTypeChange` mechanism. New converter flags (this milestone): `JIMS_REFERENCE=<key_number>` binds Re0 to a MIDI-style key number in the base state (the reference-pitch interop anchor); `JIMS_KEY_CHANGE=<measure>:<nPer>:<nGen>` shifts the reference by minus the interval Do0→XxN (the SHIFT–Do0–XxN gesture's lattice delta; 12 key steps per period, 7 per generator); `JIMS_SCALE_CHANGE=<measure>:<collection_rotation>`; the pre-existing `JIMS_MODE_CHANGE=<measure>:<mode_rotation>`. All flags at one measure compose into a single `StaffTypeChange`. Tonic-extent tokens were hand-set (`JIMS_TONIC_TOKEN=tonic-bounded`; the second span reuses it) because the M2 `enrich-runner` scratch binary is not part of either repository — recorded here as fixture provenance.

| Piece | Source | Flags | Expected indicator |
| --- | --- | --- | --- |
| `m5-mode` | collision (3 bars, one system) | `JIMS_MODE_CHANGE=2:5` | mode: tonic indicators Do, La; arrow down (fewest degrees); no dots |
| `m5-key-up` | collision | `JIMS_REFERENCE=62 JIMS_KEY_CHANGE=2:0:1` | key Do0→So0: dots Do (lower Do-line), So; arrow up; no tonic indicator |
| `m5-key-down` | collision | `JIMS_REFERENCE=62 JIMS_KEY_CHANGE=2:-1:1` | key Do0→So−1: dots Do (upper Do-line), So; arrow down |
| `m5-scale` | collision | `JIMS_SCALE_CHANGE=2:-3` | scale (Grey-notes parallel minor): new stack Do Re Me Fa So Le Te with tonic indicator on Do; no arrows |
| `m5-key-mode` | collision | `JIMS_REFERENCE=62 JIMS_MODE_CHANGE=2:5 JIMS_KEY_CHANGE=2:-1:3` | owner's worked example: dots Do, La each wrapped by a tonic indicator; ONE arrow up (key trumps mode) |
| `m5-syshead` | grym (16 bars, 3 systems) | `JIMS_MODE_CHANGE=6:0` | change at a system head: full header, NO enclosure |

## Command shape (per piece)

```
mscore enriched.musicxml -o stock.mscx
JIMS_TONIC_TOKEN=tonic-bounded [flags] python3 tools/jims/enriched_to_jims_mscx.py enriched.musicxml stock.mscx <piece>.mscx 13
mscore <piece>.mscx -o initial.png        # from a clean directory (no adjacent style/container files)
```

`semantics.json` is the Kernel change-indicator model as transported by the fork (`JiMStaffTests.changeIndicatorSemanticsPerFixtureMatchTheOwnerRules`), never pixel-derived. Renders were produced twice in clean directories and are byte-identical.
