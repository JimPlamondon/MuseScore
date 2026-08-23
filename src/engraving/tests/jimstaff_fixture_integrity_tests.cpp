/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 *
 * MuseScore Studio
 * Music Composition & Notation
 *
 * Copyright (C) 2026 Jim Plamondon
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

// Does every note in every JiMS test file actually sound at the pitch that
// file claims for it?
//
// A JiMS note carries two things. Its lattice identity says which note of the
// tuning system it is. Its MuseScore pitch is the whole-semitone pitch the
// program plays and reports. They must agree, and nothing checked that.
//
// They can silently disagree. A test file's identities are written by hand,
// and if they are written against the wrong starting point every note ends up
// a fixed distance away from the pitch it claims. Every other test still
// passes, because no other test compares the two. It happened for real on
// 2026-08-22: the SATB hymn shipped with all thirty-two notes a whole tone
// out, which put eight accidentals into a hymn that has none. No automated
// check noticed. The owner did, by looking at the printed page and seeing
// note-head shapes that should not have been there.
//
// This walks EVERY JiMS test file, not a named list, so a file added later is
// covered without anyone remembering to add it.
//
// The agreement is asked of the Kernel, per staff, never computed here. A
// formula like "62 + cents/100" would hard-code the key to one place and make
// every score fixed-Do. JiMS is movable-Do with a La-based minor: where Do
// sounds depends on the key and mode that score states, and only the Kernel
// resolves that.

#include <gtest/gtest.h>

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

#include "engraving/dom/chord.h"
#include "engraving/dom/masterscore.h"
#include "engraving/dom/measure.h"
#include "engraving/dom/note.h"
#include "engraving/dom/score.h"
#include "engraving/dom/segment.h"
#include "engraving/dom/staff.h"
#include "engraving/dom/stafftype.h"
#include "engraving/jims/jimsbridge.h"

#include "io/dir.h"
#include "io/fileinfo.h"

#include "utils/scorerw.h"

using namespace mu::engraving;

namespace {
struct Mismatch {
    std::string file;
    staff_idx_t staff = 0;
    int measure = 0;
    int nPer = 0;
    int nGen = 0;
    int claimed = 0;
    int sounds = 0;
    int re0 = 0;
    std::string anchor;

    std::string describe() const
    {
        std::ostringstream o;
        o << file << " staff " << staff << " bar " << measure
          << ": identity (" << nPer << "," << nGen << ") sounds at MIDI " << sounds
          << " with Re0=" << re0 << " (" << anchor << "), but the note claims pitch " << claimed;
        return o.str();
    }
};

/// Every JiMS test file under jimstaff_data, discovered rather than listed.
std::vector<muse::String> jimsFixtures()
{
    const muse::String dir = ScoreRW::rootPath() + u"/jimstaff_data";
    std::vector<muse::String> out;
    auto found = muse::io::Dir::scanFiles(dir, { "*.mscx", "*.mscz" }, muse::io::ScanMode::FilesInCurrentDir);
    if (!found.ret) {
        return out;
    }
    for (const muse::io::path_t& p : found.val) {
        out.push_back(p.toString());
    }
    std::sort(out.begin(), out.end());
    return out;
}

/// True when the key in force at `tick` differs from the key the score opens
/// in — i.e. a key change has happened earlier in this staff.
bool keyChangedSince(const Score* score, staff_idx_t s, const Fraction& tick)
{
    const StaffType* first = score->staff(s)->staffType(Fraction(0, 1));
    const StaffType* here = score->staff(s)->staffType(tick);
    if (!first || !here) {
        return false;
    }
    jims::SoundingPitch a, b;
    // Re0 is the score's key anchor; project the same identity under both
    // states and see whether the anchor moved. Asking the Kernel, not parsing.
    if (!jims::noteSoundingPitch(first->jimsStateJson(), 0, 0, a)
        || !jims::noteSoundingPitch(here->jimsStateJson(), 0, 0, b)) {
        return false;
    }
    return a.referenceKeyNumber != b.referenceKeyNumber;
}

void checkScore(const muse::String& path, MasterScore* score, std::vector<Mismatch>& bad,
                std::vector<Mismatch>& staleAfterKeyChange, size_t& checked)
{
    const std::string name = muse::io::FileInfo(path).fileName().toStdString();
    for (staff_idx_t s = 0; s < score->nstaves(); ++s) {
        for (Measure* m = score->firstMeasure(); m; m = m->nextMeasure()) {
            const StaffType* st = score->staff(s)->staffType(m->tick());
            if (!st || !st->isJiMS()) {
                continue;                       // a stock staff has nothing to agree about
            }
            for (Segment* seg = m->first(SegmentType::ChordRest); seg; seg = seg->next(SegmentType::ChordRest)) {
                for (voice_idx_t v = 0; v < VOICES; ++v) {
                    EngravingItem* e = seg->element(s * VOICES + v);
                    if (!e || !e->isChord()) {
                        continue;
                    }
                    for (Note* n : toChord(e)->notes()) {
                        if (!n->hasJimsPitch()) {
                            continue;           // a stock note carries no identity
                        }
                        jims::SoundingPitch sounding;
                        muse::String err;
                        if (!jims::noteSoundingPitch(st->jimsStateJson(), n->jimsNPer(), n->jimsNGen(),
                                                     sounding, &err)) {
                            ADD_FAILURE() << name << ": the Kernel could not project identity ("
                                          << n->jimsNPer() << "," << n->jimsNGen() << "): " << err.toStdString();
                            continue;
                        }
                        ++checked;
                        if (n->pitch() != sounding.midiKey) {
                            const Mismatch mm{ name, s, m->no() + 1, n->jimsNPer(), n->jimsNGen(),
                                               n->pitch(), sounding.midiKey, sounding.referenceKeyNumber,
                                               sounding.anchor.toStdString() };
                            // KNOWN OPEN DEFECT, pinned not ignored. When the key
                            // changes mid-score the note keeps its identity and so
                            // SOUNDS at the new key — playback follows the Kernel
                            // (jimstaff_m7_tests.cpp compares playback against the
                            // Kernel's projection, never against note->pitch()). The
                            // stored MuseScore pitch is not updated, so it goes stale.
                            // That stale value is what MuseScore's status bar, the
                            // stock MusicXML <pitch> fallback, and anything keyed on
                            // playback pitch (range colouring) will read. Awaiting the
                            // owner's ruling; counted below so it cannot spread
                            // unnoticed, and so this guard stays able to catch the
                            // error class it exists for.
                            (keyChangedSince(score, s, m->tick()) ? staleAfterKeyChange : bad).push_back(mm);
                        }
                    }
                }
            }
        }
    }
}
}

TEST(Engraving_JiMStaffFixtureIntegrity, everyJimsNoteSoundsAtThePitchItsFileClaims)
{
    const std::vector<muse::String> fixtures = jimsFixtures();
    ASSERT_FALSE(fixtures.empty()) << "no JiMS test files were discovered — the scan is broken, "
                                      "and a guard that scans nothing guards nothing";

    std::vector<Mismatch> bad;
    std::vector<Mismatch> staleAfterKeyChange;
    size_t checked = 0;
    size_t withNotes = 0;
    std::vector<std::string> unreadable;

    for (const muse::String& path : fixtures) {
        MasterScore* score = ScoreRW::readScore(path, true);
        if (!score) {
            unreadable.push_back(muse::io::FileInfo(path).fileName().toStdString());
            continue;
        }
        score->doLayout();
        const size_t before = checked;
        checkScore(path, score, bad, staleAfterKeyChange, checked);
        if (checked > before) {
            ++withNotes;
        }
        delete score;
    }

    for (const std::string& u : unreadable) {
        ADD_FAILURE() << "could not read JiMS test file " << u;
    }

    for (const Mismatch& m : bad) {
        ADD_FAILURE() << m.describe();
    }

    // Pin the known defect: it must not grow, and it must not silently vanish
    // either (if it is fixed, this number drops and the guard says so).
    for (const Mismatch& m : staleAfterKeyChange) {
        RecordProperty("stale_after_key_change", m.describe());
    }
    EXPECT_EQ(staleAfterKeyChange.size(), 32u)
        << "the count of notes whose stored pitch is stale after a key change changed. "
        << "It was 32, across m5-key-down, m5-key-up, m5-key-mode and m7-gate. "
        << "If it grew, new fixtures inherited an open defect; if it shrank, the defect was "
        << "fixed and this pin should come down.";

    EXPECT_TRUE(bad.empty())
        << bad.size() << " of " << checked << " JiMS notes disagree with the pitch their file claims. "
        << "A file whose identities were written against the wrong starting point looks correct to "
        << "every other test and wrong only on the printed page.";

    // The guard must actually be looking at something.
    EXPECT_GT(checked, 100u) << "only " << checked << " notes were checked across "
                             << fixtures.size() << " files — the walk is not reaching the notes";
    EXPECT_GT(withNotes, 5u) << "only " << withNotes << " files carried JiMS notes";
}
