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
#ifndef MU_ENGRAVING_MELOCHANGECONTROLLER_H
#define MU_ENGRAVING_MELOCHANGECONTROLLER_H

#include "melobridge.h"
#include "../dom/pitchspelling.h"
#include "../types/types.h"

namespace mu::engraving {
class Measure;
class Note;
class Chord;
class Score;
class StaffType;
class Staff;
struct NoteVal;
class StaffTypeChange;
}

namespace mu::engraving::melo {
/// A prepared occurrence edit. The Kernel has already projected every tied
/// and linked destination; callers can prepare a complete selection first.
struct NoteEdit {
    Note* note = nullptr;
    SoundingPitch projection;
    int tpc = Tpc::TPC_INVALID;
};
/// Validate a value at the chord and every linked copy before insertion.
bool prepareLinkedNoteValue(NoteVal& value, const Chord* chord);
Note* continuationNote(const Note* source, Chord* destination);
bool validateTieEndpoints(const Note* source, const Note* destination);
bool prepareContinuationValue(const NoteVal& source, const Staff* staff, const Fraction& sourceTick, const Fraction& targetTick,
                              NoteVal& result);
bool preparePitchEdit(Note* anchor, int nPer, int nGen, std::vector<NoteEdit>& edits, muse::String& error);
void commitPitchEdits(Score* score, const std::vector<NoteEdit>& edits, bool widenExtent = true);

/// The effective MeloPresto state at `measure` on `staffIdx`: the carrier's state
/// when the measure carries a MeloPresto change, otherwise the staff type in
/// force at the measure's tick. False when the staff is not a MeloPresto Staff.
bool effectiveState(const Score* score, staff_idx_t staffIdx, const Measure* measure, muse::String& stateJson,
                    const StaffType** effective = nullptr);
bool effectiveState(const Score* score, staff_idx_t staffIdx, const Measure* measure, const Fraction& tick, muse::String& stateJson,
                    const StaffType** effective = nullptr);

/// The Kernel's change-panel choices for the effective state at `measure`.
bool changeOptions(const Score* score, staff_idx_t staffIdx, const Measure* measure, StateChangeOptions& options);
bool changeOptions(const Score* score, staff_idx_t staffIdx, const Measure* measure, const Fraction& tick, StateChangeOptions& options);

/// May a MeloPresto change be inserted at `measure` on `staffIdx`? False with a
/// reason when the staff is not MeloPresto, when a NON-MeloPresto StaffTypeChange
/// already occupies the staff/measure, or when Measure::canAddStaffTypeChange
/// refuses.
bool canInsertChange(const Score* score, staff_idx_t staffIdx, const Measure* measure, muse::String& reason);
bool canInsertChange(const Score* score, staff_idx_t staffIdx, const Measure* measure, const Fraction& tick, muse::String& reason);

/// Apply ONE Kernel-issued choice id at `measure`: the Kernel returns the
/// complete new state from the effective state; the controller creates the
/// StaffTypeChange carrier (a copy of the effective staff type carrying the
/// new state) or updates the existing carrier's state — one undo step.
/// A choice yielding a state identical to the effective one is a no-op
/// (returns true, edits nothing). A `bind:` choice binds every unbound Melo staff and carrier in
/// the composition, preserving authored shared later references and extents
/// (never creates a carrier). False with `error` on refusal.
bool applyChange(Score* score, staff_idx_t staffIdx, Measure* measure, const muse::String& choiceId, muse::String& error);
bool applyChange(Score* score, staff_idx_t staffIdx, Measure* measure, const Fraction& tick, const muse::String& choiceId,
                 muse::String& error);

/// Milestone 9, owner decision 2a (2026-08-22): apply an ordered LIST of
/// Kernel-issued choice ids at `measure` to EVERY JiMS part of the score, as
/// ONE undoable action. The JiMS MusicXML interchange rule requires every
/// JiMS part of a document to carry the same musical state timeline, so a
/// key/mode/scale change that reached only one part would author exactly the
/// divergence that rule fails closed on.
///
/// The list exists because one user gesture can be several Kernel choices —
/// the tuning panel's scale operations are a cycle plus a rotation — and
/// "one undo step" has to cover the whole gesture.
///
/// Each target's complete new state is the Kernel's own answer for THAT
/// target, so per-staff data the change does not concern (each voice's frame
/// extent) survives untouched and no part's state is copied onto another.
/// Every target and every list step is validated BEFORE the undo transaction
/// opens; if any one is refused, nothing is mutated and `error` carries the
/// reason. Non-JiMS parts are ignored.
///
/// `bind:` uses the composition-wide binding operation in `applyChange`.
bool applyChangeToAllMeloParts(Score* score, Measure* measure, const std::vector<muse::String>& choiceIds, muse::String& error);
bool applyChangeToAllMeloParts(Score* score, Measure* measure, const Fraction& tick, const std::vector<muse::String>& choiceIds,
                               muse::String& error);

/// Remove the MeloPresto change carrier at `measure` (one undo step). False with
/// `error` when there is none.
bool removeChange(Score* score, staff_idx_t staffIdx, Measure* measure, muse::String& error);
bool removeChange(Score* score, staff_idx_t staffIdx, Measure* measure, const Fraction& tick, muse::String& error);

/// Read-only validation shared by native and interchange boundaries. Refuses
/// missing coordinates, contradictory full ties/links and shared references.
bool validateLatticeContent(const Score* score, muse::String& error);

/// Compare each part's Kernel-owned shared musical projection at every effective change
/// boundary. Redundant transport carriers and local drawing extents are allowed.
bool validateSharedStateTimeline(const Score* score, muse::String& error);

/// Enforce the persisted MeloPresto authority contract after native load or import.
/// Validated coordinates plus effective state replace contradictory ordinary
/// pitch fields in one undoable repair command. `repairs` counts notes, while
/// callers emit at most one document-level diagnostic.
bool normalizeStoredPitchesAfterLoad(Score* score, size_t& repairs, muse::String& error, bool undoable = true, bool commandOpen = false);
}

#endif
