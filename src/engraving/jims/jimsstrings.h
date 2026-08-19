/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 *
 * JiMStaff Milestone 1 — the single translatable string table for every
 * user-visible JiMS name (owner Q22 answer, 2026-08-13: JiMS-based names
 * are likely to change for trademark reasons within months, so a rename
 * must be one-file work). No user-visible JiMS name may appear as a
 * literal anywhere else in this fork; tools/jims/check_jims_strings.py
 * enforces that.
 */
#ifndef MU_ENGRAVING_JIMSSTRINGS_H
#define MU_ENGRAVING_JIMSSTRINGS_H

#include "translation.h"

namespace mu::engraving::jims {
inline muse::String featureName() { return muse::mtrc("engraving", "JiMStaff"); }
inline muse::String staffUserName() { return muse::mtrc("engraving", "JiMS staff"); }
inline muse::String presetName() { return muse::mtrc("engraving", "JiMStaff 12-TET"); }
inline muse::String crescentClefName() { return muse::mtrc("engraving", "crescent clef"); }
inline muse::String scaleDotsName() { return muse::mtrc("engraving", "scale dots"); }
inline muse::String tonicIndicatorName() { return muse::mtrc("engraving", "tonic indicator"); }
/// Stock-MuseScore loss warning (silent unknown-tag destruction on resave).
inline muse::String stockLossWarning()
{
    return muse::mtrc("engraving",
                      "This JiMS-enabled version of MuseScore can save and open files that include JiMS notation. "
                      "However, if you open it with the standard version of MuseScore, the JiMS-specific information "
                      "will be lost. Until MuseScore accepts the JiMS features in its main version, use only this "
                      "JiMS-enabled version of MuseScore to avoid losing JiMS-related information.");
}
}

#endif
