# Native JiMS MusicXML export — validator / round-trip matrix (2026-08-17)

Exports (and re-exports of the reimported documents) of every JiMS fixture in `src/importexport/musicxml/tests/data/jims/` plus the M6/M7 gate file, validated against the unchanged extension in the clean worktree `Temp/musicxml-claude-v3` (`gh-pages d720d2e8`): XSD (`xmllint --schema schema/musicxml.xsd`, catalog), Schematron (`tests/schematron.py validations/jims.xsl` on DOCTYPE-stripped copies — saxon tries to fetch MuseScore's standard `partwise.dtd` DOCTYPE; the XSD pass on the same files shows the documents themselves are valid), tuple oracle (`tests/jims_change_events.py` vs `tests/vectors/jims_change_events_v3.json` — finite, at-least-one-change; `N/A` where out of domain per FINAL §6.5), V2/standard projection (`schema/jims-to-standard.xsl` → no `jims:` element / `xmlns:jims` remains, well-formed; the stylesheet's own `jims-lossy-projection` provenance note is by design), native round trip (`MusicXml_JiMS_Tests.exportWritesV3AndRoundTripsThroughTheNativeImporter` / `exportOfANativeJiMSScoreCarriesStatesChangesAndIdentities`: Kernel-canonical staff states, carrier ticks, ordered note identities equal after export→import and after re-export→import).

| File | XSD | Schematron | Tuple / applicability | Standard projection | Round trip | States | Carriers | Identities |
|---|---|---|---|---|---|---|---|---|
| jims-v3-m5-mode | pass | pass | pass | pass | pass | equal | equal | equal |
| jims-v3-m5-key-up | pass | pass | pass | pass | pass | equal | equal | equal |
| jims-v3-m5-key-down | pass | pass | pass | pass | pass | equal | equal | equal |
| jims-v3-m5-scale | pass | pass | pass | pass | pass | equal | equal | equal |
| jims-v3-m5-key-mode | pass | pass | pass | pass | pass | equal | equal | equal |
| jims-v3-m5-syshead | pass | pass | pass | pass | pass | equal | equal | equal |
| jims-reference-none | pass | pass | N/A — no classified change in the document | pass | pass | equal | equal | equal |
| jims-reference-none-explicit | pass | pass | N/A — no classified change in the document | pass | pass | equal | equal | equal |
| jims-reference-pitch | pass | pass | N/A — no classified change in the document | pass | pass | equal | equal | equal |
| jims-reference-pitch-class | pass | pass | N/A — no classified change in the document | pass | pass | equal | equal | equal |
| jims-reference-hertz | pass | pass | N/A — no classified change in the document | pass | pass | equal | equal | equal |
| jims-mid-score-state-change | pass | pass | pass | pass | pass | equal | equal | equal |
| jims-multi-staff | pass | pass | N/A — no classified change in the document | pass | pass | equal | equal | equal |
| jims-12tet-diatonic | pass | pass | N/A — no classified change in the document | pass | pass | equal | equal | equal |
| jims-v2-mode-change | pass | pass | N/A — tuple outside the pinned finite vector set (V2-profile fixture; XSD/Schematron/round trip still pass) | pass | pass | equal | equal | equal |
| m7-gate (mscz) | pass | pass | pass (key+mode, the accepted worked example) | pass | pass | equal | equal | equal |

Totals over the 32 exported documents (16 exports + 16 re-exports): XSD 32/32, Schematron 32/32, projection 32/32, tuple 13 pass / 17 N/A (no change) / 2 N/A (outside domain).

Stock export: 39 stock test scores exported before and after the change — SHA-256 manifests identical (`stock-export-manifest.sha256` vs `stock-export-after.sha256`); no exported stock document contains `jims`.

Renders: `jims-v3-m5-key-mode` original vs round-tripped PNG byte-identical; `m7-gate.mscz` original vs its exported/reimported document visually identical (434 sub-pixel differences from measure-width relayout — the `.mscz` carries its own style container while the reimported MusicXML uses defaults; formatting-only).
