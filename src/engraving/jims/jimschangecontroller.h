/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 *
 * JiMStaff Milestone 6 — the change-insertion controller (owner decision
 * 1a, 2026-08-16). Given a selected staff and measure it reads the
 * effective JiMS state (the measure's own carrier if it has one, else the
 * staff type in force there), asks the Kernel what may change
 * (`state_change_options`), asks the Kernel for the complete new state
 * after one choice (`apply_state_change`), and edits the document —
 * creating or updating the measure's StaffTypeChange carrier as ONE
 * undoable command, or removing it. No musical fact is computed here.
 */
#ifndef MU_ENGRAVING_JIMSCHANGECONTROLLER_H
#define MU_ENGRAVING_JIMSCHANGECONTROLLER_H

#include "jimsbridge.h"
#include "../types/types.h"

namespace mu::engraving {
class Measure;
class Score;
class StaffType;
class StaffTypeChange;
}

namespace mu::engraving::jims {
/// The effective JiMS state at `measure` on `staffIdx`: the carrier's state
/// when the measure carries a JiMS change, otherwise the staff type in
/// force at the measure's tick. False when the staff is not a JiMStaff.
bool effectiveState(const Score* score, staff_idx_t staffIdx, const Measure* measure,
                    muse::String& stateJson, const StaffType** effective = nullptr);

/// The Kernel's change-panel choices for the effective state at `measure`.
bool changeOptions(const Score* score, staff_idx_t staffIdx, const Measure* measure,
                   StateChangeOptions& options);

/// May a JiMS change be inserted at `measure` on `staffIdx`? False with a
/// reason when the staff is not JiMS, when a NON-JiMS StaffTypeChange
/// already occupies the staff/measure, or when Measure::canAddStaffTypeChange
/// refuses.
bool canInsertChange(const Score* score, staff_idx_t staffIdx, const Measure* measure,
                     muse::String& reason);

/// Apply ONE Kernel-issued choice id at `measure`: the Kernel returns the
/// complete new state from the effective state; the controller creates the
/// StaffTypeChange carrier (a copy of the effective staff type carrying the
/// new state) or updates the existing carrier's state — one undo step.
/// A choice yielding a state identical to the effective one is a no-op
/// (returns true, edits nothing). False with `error` on refusal.
bool applyChange(Score* score, staff_idx_t staffIdx, Measure* measure,
                 const muse::String& choiceId, muse::String& error);

/// Remove the JiMS change carrier at `measure` (one undo step). False with
/// `error` when there is none.
bool removeChange(Score* score, staff_idx_t staffIdx, Measure* measure, muse::String& error);
}

#endif
