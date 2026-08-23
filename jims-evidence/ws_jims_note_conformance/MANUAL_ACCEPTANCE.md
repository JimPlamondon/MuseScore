# Common-tone manual acceptance

1. Open `src/engraving/tests/jimstaff_data/ws-jims-common-tone-projection.mscx`.
2. Confirm the JiMS key/state change appears at the second measure.
3. Confirm the tied continuation immediately uses the visibly different new-state JiMS identity.
4. Play from the first measure and confirm one uninterrupted D4: one attack, no reattack, pitch jump, glide, bend, or audible seam at the barline.
5. A saved score has no undo history, so first create one current-session transaction: select the continuation in measure 2, open **Format > JiMS Staff (tuning & changes)...**, and click **Remove change at this bar**. Confirm the state and continuation revert together. Undo once and confirm the state plus the continuation's identity, MIDI pitch, spelling, octave, and residual cents return together; redo once and confirm they revert together again. Undo once more after the review to leave the fixture at its opened state.

Manual acceptance: pending Jim
