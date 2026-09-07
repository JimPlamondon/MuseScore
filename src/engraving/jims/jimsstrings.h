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
#include "types/translatablestring.h"

namespace mu::engraving::melo {
inline muse::TranslatableString changeActionName() { return muse::TranslatableString("action", "MeloPresto change…"); }
inline muse::String featureName() { return muse::mtrc("engraving", "MeloPresto Staff"); }
inline muse::String staffUserName() { return muse::mtrc("engraving", "MeloPresto Staff"); }
inline muse::String presetName() { return muse::mtrc("engraving", "MeloPresto Staff 12-TET"); }
inline muse::String crescentClefName() { return muse::mtrc("engraving", "crescent clef"); }
inline muse::String scaleDotsName() { return muse::mtrc("engraving", "scale dots"); }
inline muse::String tonicIndicatorName() { return muse::mtrc("engraving", "tonic indicator"); }
inline muse::String linkedNoteMissingState()
{
    return muse::mtrc("engraving", "a linked MeloPresto note has no effective MeloPresto state");
}

inline muse::String partialTieCrossesState()
{
    return muse::mtrc("engraving", "a path-dependent partial tie crosses the MeloPresto state span; the edit was not applied");
}

inline muse::String linkedNoteIdentityMismatch()
{
    return muse::mtrc("engraving", "a linked note disagrees about MeloPresto identity; the edit was not applied");
}

inline muse::String conflictingLinkedProjections()
{
    return muse::mtrc("engraving", "linked notes require conflicting MeloPresto projections; the edit was not applied");
}

inline muse::String notSystemStaff() { return muse::mtrc("engraving", "not a MeloPresto Staff"); }
inline muse::String positionHasOtherStaffChange()
{
    return muse::mtrc("engraving", "this position already carries a non-MeloPresto Staff type change on this staff");
}

inline muse::String emptyStaffCentreUnavailable()
{
    return muse::mtrc("engraving", "the MeloPresto Kernel could not derive the empty staff centre");
}

inline muse::TranslatableString bindReferenceAction() { return muse::TranslatableString("undoableAction", "Bind MeloPresto reference"); }
inline muse::TranslatableString insertChangeAction() { return muse::TranslatableString("undoableAction", "Insert MeloPresto change"); }
inline muse::String measureHasOtherStaffChange()
{
    return muse::mtrc("engraving", "this measure already carries a non-MeloPresto Staff type change on this staff");
}

inline muse::String staffStateUnavailable() { return muse::mtrc("engraving", "staff %1: no MeloPresto state in force at this position"); }
inline muse::String numberedStaffCentreUnavailable()
{
    return muse::mtrc("engraving", "staff %1: the MeloPresto Kernel could not derive the empty staff centre");
}

inline muse::String changeUnavailable() { return muse::mtrc("engraving", "no MeloPresto change at this position"); }
inline muse::String precedingStateUnavailable() { return muse::mtrc("engraving", "no preceding MeloPresto state can replace this change"); }
inline muse::TranslatableString removeChangeAction() { return muse::TranslatableString("undoableAction", "Remove MeloPresto change"); }
inline muse::TranslatableString normalizeStoredPitchesAction()
{
    return muse::TranslatableString("undoableAction", "Normalize MeloPresto stored pitches");
}

inline muse::TranslatableString changeTuningAction() { return muse::TranslatableString("undoableAction", "Change MeloPresto tuning"); }
inline muse::TranslatableString changeMelodyPartAction()
{
    return muse::TranslatableString("undoableAction", "Change MeloPresto melody part");
}

inline muse::TranslatableString chordNameAction() { return muse::TranslatableString("action", "MeloPresto chord &name"); }
inline muse::TranslatableString chordNameActionDescription()
{
    return muse::TranslatableString("action", "Add text: MeloPresto chord name (one name, without spaces or ~; text only)");
}

inline muse::TranslatableString chordName() { return muse::TranslatableString("engraving", "MeloPresto chord name"); }
inline muse::String exportChordFretDiagram()
{
    return muse::mtrc("iex_musicxml", "MeloPresto export: a MeloPresto chord name cannot be attached to a conventional fret diagram");
}

inline muse::String exportChordNameInvalid()
{
    return muse::mtrc("iex_musicxml",
                      "MeloPresto export: every MeloPresto harmony must carry exactly one nonempty whitespace-free canonical chord name and must not contain the superseded '~' marker");
}

inline muse::String exportBaseStateRefused()
{
    return muse::mtrc("iex_musicxml", "MeloPresto export: Kernel refused the base state of staff %1: %2");
}

inline muse::String exportMissingBaseState()
{
    return muse::mtrc("iex_musicxml",
                      "MeloPresto export: staff %1 carries a MeloPresto section at measure %2 without a MeloPresto base state at tick 0");
}

inline muse::String exportStateRefused()
{
    return muse::mtrc("iex_musicxml", "MeloPresto export: Kernel refused the state at tick %1, staff %2: %3");
}

inline muse::String exportChangeUnclassified()
{
    return muse::mtrc("iex_musicxml", "MeloPresto export: Kernel could not classify the change at tick %1, staff %2: %3");
}

inline muse::String exportTimelinesDiffer()
{
    return muse::mtrc("iex_musicxml",
                      "MeloPresto export: parts %1 and %2 carry different MeloPresto state timelines; every MeloPresto part of a document must share one state timeline");
}

inline muse::String exportMissingLatticeIdentity()
{
    return muse::mtrc("iex_musicxml", "MeloPresto export: a note on staff %1 at tick %2 has no lattice identity");
}

inline muse::String exportNoteRefused()
{
    return muse::mtrc("iex_musicxml", "MeloPresto export: Kernel refused the note at tick %1 on staff %2: %3");
}

inline muse::String warnWhenSavingNotation() { return muse::mtrc("project/save", "Warn when saving MeloPresto Notation"); }
/// Stock-MuseScore loss warning (silent unknown-tag destruction on resave).
inline muse::String stockLossWarning()
{
    return muse::mtrc("engraving",
                      "This MeloPresto-enabled version of MuseScore can save and open files that include MeloPresto Notation, "
                      "like the file you're saving now. However, if you open this file with the standard version of "
                      "MuseScore, the MeloPresto-specific information will be lost. Until MuseScore's standard version "
                      "embraces MeloPresto Notation, use *only* this MeloPresto-enabled version of MuseScore to avoid losing "
                      "MeloPresto-specific information.");
}
}

#endif
