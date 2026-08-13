/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 *
 * JiMStaff Milestone 1 — the fork-side wrapper over the jims-musescore-bridge
 * C ABI (Apache-2.0 staticlib built from the JiMS Kernel repository). Every
 * musical fact used by JiMStaff rendering crosses this seam as JSON derived
 * by the Kernel; nothing here computes note classes, memberships, cents, or
 * notehead classes itself.
 */
#ifndef MU_ENGRAVING_JIMSBRIDGE_H
#define MU_ENGRAVING_JIMSBRIDGE_H

#include <vector>

#include "types/string.h"

namespace mu::engraving::jims {
/// Bridge availability: the linked C ABI answers version 1.
bool available();

/// A note's cents above the staff's lower Do boundary, composed from the
/// Kernel's note_placement (Do-relative ordinate + register) and the state's
/// own period/extent values — projection only, no fact derivation.
bool noteCentsAboveDo(const muse::String& stateJson, int nPer, int nGen, double& cents);

/// The Kernel's semantic notehead-class token for a generator coordinate
/// (e.g. "conventional", "triangle-vertex-up").
bool noteheadToken(const muse::String& stateJson, int nGen, muse::String& token);

/// One scale-dot stack: cents above the lower Do, plus member generator
/// coordinates front-to-back (Kernel collision order).
struct ScaleDotStack {
    double cents = 0.0;
    std::vector<int> frontToBack;
};

/// The Kernel's scale-dot stacks for one staff, in ascending order.
bool scaleDots(const muse::String& stateJson, std::vector<ScaleDotStack>& stacks);
}

#endif
