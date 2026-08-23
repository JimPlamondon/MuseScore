# Common-tone manual acceptance

1. Open `src/engraving/tests/jimstaff_data/ws-jims-common-tone-projection.mscx`.
2. Confirm the JiMS key/state change appears at the second measure.
3. Confirm the tied continuation immediately uses the visibly different new-state JiMS identity.
4. Play from the first measure and confirm one uninterrupted D4: one attack, no reattack, pitch jump, glide, bend, or audible seam at the barline.
5. Undo once and confirm the state plus the continuation's identity, MIDI pitch, spelling, octave, and residual cents revert together; redo once and confirm they return together.

Manual acceptance: pending Jim
