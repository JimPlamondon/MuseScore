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

/// The Kernel-owned tonic-indicator position: cents above Do for the
/// state's mode-selected (movable) tonic.
bool tonicCentsAboveDo(const muse::String& stateJson, double& cents);

/// One Kernel-owned Just Intonation staff line: exact just cents, prime
/// limit, and VTR-gated visibility for the state's current generator.
struct JiLine {
    double cents = 0.0;
    int limit = 5;
    bool visible = true;
};

/// The Kernel's JI staff-line scaffold (owner rulings 1a/2a, 2026-08-14).
bool jiLines(const muse::String& stateJson, std::vector<JiLine>& lines);

/// One stave segment of the Kernel-derived frame (partial-staves ruling
/// 2026-08-14): cents relative to the staff origin; partial segments get
/// a sliced, closed crescent at their cut edge.
struct StaveSegment {
    double lowerCents = 0.0;
    double upperCents = 0.0;
    bool whole = true;
};

/// The Kernel frame for a melody plus the DECLARED tonic-extent token
/// ("tonic-bounded" / "tonic-centered"), derived at authoring and saved.
bool frameForMelody(const muse::String& stateJson, const muse::String& melodyJson,
                    const muse::String& extentToken, std::vector<StaveSegment>& segments);
}

#endif
