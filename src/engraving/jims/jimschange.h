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
#include "../types/types.h"

namespace mu::engraving {
class Measure;
class StaffType;
class StaffTypeChange;
}

namespace mu::engraving::jims {
/// The JiMS StaffTypeChange carried by `measure` for `staffIdx`, if any.
const StaffTypeChange* changeCarrier(const Measure* measure, staff_idx_t staffIdx);

/// The Kernel's change-indicator model for the change `measure` carries
/// on `staffIdx`, evaluated between the staff type in effect just before
/// the measure and the one it introduces. Returns false when the measure
/// carries no JiMS change, when the change sits at a system head (the
/// full header is the indication there — no enclosure), or when the
/// Kernel model is empty (tuning/extent/presentation-only differences).
bool midSystemChangeIndicator(const Measure* measure, staff_idx_t staffIdx,
                              ChangeIndicator& out, const StaffType** newStaffType = nullptr);

/// The terrain width to reserve at the start of `measure` (max over
/// staves that carry a mid-system indicator), or 0.
double changeTerrainWidth(const Measure* measure);
}

#endif
