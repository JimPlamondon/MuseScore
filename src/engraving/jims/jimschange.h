/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 *
 * JiMStaff Milestone 5 — the change-indicator seam. Finds the JiMS staff-
 * type change a measure carries for a staff, hands the OLD and NEW state
 * to the Kernel (`change_indicator`), and answers "is there a mid-system
 * indicator to reserve room for and paint here?". No musical fact is
 * computed here: the fork never diffs states, classifies, or chooses.
 */
#ifndef MU_ENGRAVING_JIMSCHANGE_H
#define MU_ENGRAVING_JIMSCHANGE_H

#include "jimsbridge.h"
#include "../dom/stafftype.h"
#include "../types/types.h"

#include <vector>

namespace mu::engraving {
class Measure;
class Note;
class Staff;
class StaffType;
class StaffTypeChange;
}

namespace mu::engraving::jims {
/// The JiMS StaffTypeChange carried by `measure` for `staffIdx`, if any.
const StaffTypeChange* changeCarrier(const Measure* measure, staff_idx_t staffIdx);

/// The JiMS carrier at one exact absolute score tick, if any.
const StaffTypeChange* changeCarrierAt(const Measure* measure, staff_idx_t staffIdx, const Fraction& tick);

/// All JiMS carriers in `measure` for `staffIdx`, ordered by exact tick.
std::vector<const StaffTypeChange*> changeCarriers(const Measure* measure, staff_idx_t staffIdx);

/// The Kernel's change-indicator model for the change `measure` carries
/// on `staffIdx`, evaluated between the staff type in effect just before
/// the measure and the one it introduces. Returns false when the measure
/// carries no JiMS change, when the change sits at a system head (the
/// full header is the indication there — no enclosure), or when the
/// Kernel model is empty (tuning/extent/presentation-only differences).
bool midSystemChangeIndicator(const Measure* measure, staff_idx_t staffIdx, ChangeIndicator& out, const StaffType** newStaffType = nullptr);

/// The Kernel model for an exact carrier strictly inside its measure.
bool midBarChangeIndicator(const StaffTypeChange* carrier, ChangeIndicator& out, const StaffType** newStaffType = nullptr);

/// The terrain width to reserve at the start of `measure` (max over
/// staves that carry a mid-system indicator), or 0.
double changeTerrainWidth(const Measure* measure);

/// Terrain width for carriers at one exact absolute score tick.
double changeTerrainWidthAt(const Measure* measure, const Fraction& tick);

/// Courtesy indicator (owner ruling 2026-08-16, option 1a): when the NEXT
/// measure carries a JiMS change and `measure` is the last of its system,
/// the change is indicated at the END of `measure` — the closing barline
/// serves as the right stroke, one added stroke opens the terrain on the
/// left — because the new system's fresh header alone hides the change.
/// Model between the state in effect through `measure` and the state the
/// next measure introduces. Returns false when not applicable or empty.
bool courtesyChangeIndicator(const Measure* measure, staff_idx_t staffIdx, ChangeIndicator& out,
                             const StaffType** stateStaffType = nullptr);

/// The courtesy terrain width to reserve at the end of `measure`, or 0.
double courtesyTerrainWidth(const Measure* measure);

/// Owner ruling 2026-08-19: the change indicator (tonic indicators and the
/// key/mode arrow ends) is anchored on the Do-line of the drawn stave stack
/// that keeps the WHOLE indicator inside the staff — the lowest such Do-line;
/// when none does, the Do-line that overflows least (extending the staff to
/// fit is a separate follow-up). Returns the anchor as cents relative to
/// the frame origin (a multiple of the period); scale-change dot stacks are
/// instantiated per period and do not depend on it. Falls back to the
/// stack's lowest period for an empty view.
double changeAnchorPeriodCents(const StaffType::JimsFrameView& view, const ChangeIndicator& model, double periodCents,
                               double doCentsAboveExtentLower = 0.0);

/// The change indicator drawn against THIS staff type's frame — the one
/// whose NEW state is `newStaffType` (its own section start; mid-system or
/// courtesy alike). False when the staff type starts no JiMS section
/// (the base type) or the Kernel derives no indicator.
bool changeIndicatorIntoStaffType(const Score* score, staff_idx_t staffIdx, const StaffType* newStaffType, ChangeIndicator& out);

/// Owner rule 2026-08-19 (7b): the cents (frame coordinates) of every
/// indicator point that still falls outside the drawn stave after the
/// best Do-line anchor was chosen — empty when the indicator fits. The
/// frame is then re-derived covering them (Kernel `extra_cents`), so the
/// staff extends to include the whole indicator.
std::vector<double> changeIndicatorOverflowCents(const StaffType::JimsFrameView& view, const ChangeIndicator& model, double periodCents,
                                                 double doCentsAboveExtentLower = 0.0);

/// Derive one song-wide tonic ambit from the explicitly designated melody
/// part and repeat the Kernel token through every JiMS transport carrier.
int deriveTonicAmbits(Score* score);

/// Load-time extent reconciliation: exact written-note bounds, or a
/// Kernel-derived one-period default from the Part's declared amateur range.
int reconcileExtents(Score* score);

/// Reproject an empty vocal staff's default extent after its song state
/// changes. Written staves and non-vocal staves are returned unchanged.
bool defaultExtentForEmptyStaffSpan(const Staff* staff, const Fraction& start, const Fraction& stop, const muse::String& state,
                                    muse::String& updated);

/// Edit-time grow-only lifecycle transition for one entered or moved note.
bool widenExtentForNote(Note* note);

/// Recompute the song-wide tonic ambit after a note in the explicitly
/// designated melody Part changes. A note in any other Part is a no-op.
void designatedMelodyNoteChanged(Note* note);
}

#endif
