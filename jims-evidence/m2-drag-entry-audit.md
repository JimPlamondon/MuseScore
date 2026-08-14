# Milestone 2 Phase 3 — drag/entry site audit (named binding deliverable)

One physical line per row description. Disposition legend: REROUTED = a JiMS branch intercepts before the stock arithmetic; GUARDED = unreachable for JiMS notes because an earlier JiMS branch returns first; STOCK-OK = legitimately shared with non-JiMS staves and carries no JiMS-note pitch decision.

| Site | File | Disposition | Verification |
|---|---|---|---|
| Note::verticalDrag (diatonic branch) | dom/note.cpp | REROUTED — JiMS branch quantizes via Kernel nearest_pitch (current-identity tie retention) and returns before step arithmetic | code inspection + Phase-6 gtest planned |
| Score::noteValForPosition STANDARD branch | dom/noteentry.cpp | REROUTED — JiMS branch converts click height to cents, Kernel picks pitch + compatibility spelling, stock absStep/step2pitch skipped | code inspection + Phase-6 gtest planned |
| Note::setNval | dom/note.cpp | REROUTED (identity attach) — entered notes without a lattice identity get one from the Kernel entry conversion derived from the just-set spelling | code inspection |
| Note::updateAccidental | dom/note.cpp | GUARDED — JiMS branch (M1) removes accidentals, updates state, calls updateRelLine, returns | M1 audit carried forward |
| Note::updateRelLine / updateLine | dom/note.cpp | GUARDED — JiMS branch places by Kernel cents through Note::jimsPosY | M1 audit carried forward |
| ChordLayout second-writer site | rendering/score/chordlayout.cpp | GUARDED — routes through Note::jimsPosY | M1 audit carried forward |
| absStep/relStep/step2pitch/step2tpcByKey etc. | dom/utils.cpp, dom/pitchspelling.cpp | STOCK-OK — reachable for JiMS notes only through the guarded callers above; pure functions with no staff knowledge | grep audit (Phase-0 baseline vs post) |
| Score::putNote / cmdAddPitch / addPitch | dom/noteentry.cpp | STOCK-OK — consume the NoteVal produced by the rerouted noteValForPosition; identity attaches in setNval | code inspection |
| NotationNoteInput::setInputNote | notation layer | STOCK-OK — passes NoteVal through; no pitch arithmetic | grep |
| Tied-note propagation | dom/note.cpp (verticalDrag loop, tie code) | REROUTED — the JiMS drag branch iterates tiedNotes() applying identity + compatibility pitch together | code inspection |
| Compatibility-spelling transport (tpc2step/tpc2alter in setNval JiMS attach; step2pitch/step2tpc on Kernel-supplied spelling) | dom/note.cpp, dom/noteentry.cpp | STOCK-OK (transport) — converts between equivalent representations of the Kernel-supplied spelling; no musical decision is made fork-side | code inspection |
