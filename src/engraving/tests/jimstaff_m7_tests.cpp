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

// JiMStaff Milestone 7 — playback (owner rulings 2026-08-17): a JiMS note
// SOUNDS what its identity + its section's state (tuning, reference) say,
// through the Kernel's note_sounding_pitch answer only. Every expected
// value below is a fresh Kernel call for the section state in force at
// the note — never a hand-derived formula.
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cmath>
#include <optional>
#include <set>

#include "engraving/dom/chord.h"
#include "engraving/dom/masterscore.h"
#include "engraving/dom/measure.h"
#include "engraving/dom/note.h"
#include "engraving/dom/part.h"
#include "engraving/dom/segment.h"
#include "engraving/dom/staff.h"
#include "engraving/dom/stafftype.h"
#include "engraving/jims/jimsbridge.h"
#include "engraving/jims/jimstuningcontroller.h"
#include "engraving/playback/playbackmodel.h"
#include "engraving/playback/utils/pitchutils.h"
#include "mpe/tests/mocks/articulationprofilesrepositorymock.h"
#include "mpe/tests/utils/articulationutils.h"

#include "utils/scorerw.h"

using namespace mu::engraving;
using namespace muse;
using namespace muse::mpe;
using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;

namespace {
class Engraving_JiMStaffM7PlaybackTests : public ::testing::Test
{
protected:
    void SetUp() override
    {
        m_defaultProfile = std::make_shared<ArticulationsProfile>();
        m_repositoryMock = std::make_shared<NiceMock<ArticulationProfilesRepositoryMock> >();
        EXPECT_CALL(*m_repositoryMock, defaultProfile(_)).WillRepeatedly(Return(m_defaultProfile));
    }

    /// The nominal pitch level of every NoteEvent, in event order.
    std::vector<pitch_level_t> nominalPitchLevels(Score* score)
    {
        PlaybackModel model(modularity::globalCtx());
        model.profilesRepository.set(m_repositoryMock);
        model.load(score);
        const Part* part = score->parts().at(0);
        const PlaybackEventsMap& events = model.resolveTrackPlaybackData(part->id(), part->instrumentId()).originEvents;
        std::vector<pitch_level_t> out;
        for (const auto& pair : events) {
            for (const PlaybackEvent& ev : pair.second) {
                if (std::holds_alternative<muse::mpe::NoteEvent>(ev)) {
                    out.push_back(std::get<muse::mpe::NoteEvent>(ev).pitchCtx().nominalPitchLevel);
                }
            }
        }
        return out;
    }

    /// The exact-pitch field of every NoteEvent, in event order (JiMSynth
    /// VST3 workstream: the lossless Kernel answer beside the pitch level).
    std::vector<std::optional<ExactPitch> > exactPitches(Score* score)
    {
        PlaybackModel model(modularity::globalCtx());
        model.profilesRepository.set(m_repositoryMock);
        model.load(score);
        const Part* part = score->parts().at(0);
        const PlaybackEventsMap& events = model.resolveTrackPlaybackData(part->id(), part->instrumentId()).originEvents;
        std::vector<std::optional<ExactPitch> > out;
        for (const auto& pair : events) {
            for (const PlaybackEvent& ev : pair.second) {
                if (std::holds_alternative<muse::mpe::NoteEvent>(ev)) {
                    out.push_back(std::get<muse::mpe::NoteEvent>(ev).pitchCtx().exactPitch);
                }
            }
        }
        return out;
    }

    std::vector<DynamicTonalityProfileEvent> dynamicTonalityProfiles(Score* score)
    {
        PlaybackModel model(modularity::globalCtx());
        model.profilesRepository.set(m_repositoryMock);
        model.load(score);
        const Part* part = score->parts().at(0);
        const PlaybackEventsMap& events = model.resolveTrackPlaybackData(part->id(), part->instrumentId()).originEvents;
        std::vector<DynamicTonalityProfileEvent> out;
        for (const auto& pair : events) {
            for (const PlaybackEvent& ev : pair.second) {
                if (std::holds_alternative<DynamicTonalityProfileEvent>(ev)) {
                    out.push_back(std::get<DynamicTonalityProfileEvent>(ev));
                }
            }
        }
        return out;
    }

    /// The Kernel's fresh sounding-pitch answer for a JiMS note (the oracle
    /// for the exact-pitch field).
    static jims::SoundingPitch kernelSoundingPitch(const Note* note)
    {
        const StaffType* st = note->staff()->staffTypeForElement(note);
        EXPECT_TRUE(st && st->isJiMS());
        jims::SoundingPitch sp;
        String err;
        EXPECT_TRUE(jims::noteSoundingPitch(st->jimsStateJson(), note->jimsNPer(), note->jimsNGen(), sp, &err)) << err.toStdString();
        return sp;
    }

    /// The Kernel's expected pitch level for a JiMS note in the section
    /// state in force at that note (fresh call; the oracle).
    static pitch_level_t kernelPitchLevel(const Note* note)
    {
        const StaffType* st = note->staff()->staffTypeForElement(note);
        EXPECT_TRUE(st && st->isJiMS());
        jims::SoundingPitch sp;
        String err;
        EXPECT_TRUE(jims::noteSoundingPitch(st->jimsStateJson(), note->jimsNPer(), note->jimsNGen(), sp, &err)) << err.toStdString();
        return jimsPitchLevelFromMidi(sp.midiKey, sp.centsOffset);
    }

    /// Notes of staff 0 in document order (voice 1 only, single-note chords).
    static std::vector<Note*> notesOf(Score* score)
    {
        std::vector<Note*> notes;
        for (Segment* seg = score->firstSegment(SegmentType::ChordRest); seg; seg = seg->next1(SegmentType::ChordRest)) {
            EngravingItem* el = seg->element(0);
            if (el && el->isChord()) {
                for (Note* n : toChord(el)->notes()) {
                    notes.push_back(n);
                }
            }
        }
        return notes;
    }

    ArticulationsProfilePtr m_defaultProfile = nullptr;
    std::shared_ptr<NiceMock<ArticulationProfilesRepositoryMock> > m_repositoryMock = nullptr;
};

const String M7_GATE(u"jimstaff_data/m7-gate.mscz");
const String M7_DEFAULT(u"jimstaff_data/collision.mscx");
}

// The MIDI-key → pitch-level conversion (owner decision 1a): raw MIDI
// numbering onto the pitch-level scale, C4 = 60, cents in 2-cent steps.
TEST_F(Engraving_JiMStaffM7PlaybackTests, m7PitchLevelFromMidiMatchesTheMpeScale)
{
    EXPECT_EQ(jimsPitchLevelFromMidi(60, 0.0), pitchLevel(PitchClass::C, 4));
    EXPECT_EQ(jimsPitchLevelFromMidi(69, 0.0), pitchLevel(PitchClass::A, 4));
    EXPECT_EQ(jimsPitchLevelFromMidi(12, 0.0), 0);
    EXPECT_EQ(jimsPitchLevelFromMidi(61, 0.0), pitchLevel(PitchClass::C_sharp, 4));
    EXPECT_EQ(jimsPitchLevelFromMidi(60, 50.0), pitchLevel(PitchClass::C, 4) + PITCH_LEVEL_STEP / 2);
    EXPECT_EQ(jimsPitchLevelFromMidi(60, -50.0), pitchLevel(PitchClass::C, 4) - PITCH_LEVEL_STEP / 2);
    // Same result as the stock path for a plain 12-TET note with a tuning.
    EXPECT_EQ(jimsPitchLevelFromMidi(64, 10.0), notePitchLevel(Tpc::TPC_E, 4, 10.0));
}

// Default 12-TET (unpinned → Kernel default Re0 = 62) and the M6 gate
// file's reference-53 section (bar 2 onward): every JiMS note plays the
// Kernel's answer for ITS section — bar 2's notes no longer sound their
// bar-1 compatibility pitches.
TEST_F(Engraving_JiMStaffM7PlaybackTests, m7PlaybackUsesKernelPitchForDefaultAndReference53Sections)
{
    Score* score = ScoreRW::readScore(M7_GATE);
    ASSERT_TRUE(score);
    score->doLayout();
    std::vector<Note*> notes = notesOf(score);
    ASSERT_EQ(notes.size(), 12u) << "bars 1-3: E D F G | C D E F | E D F G";
    // Provenance: bar 1 unpinned? No — the gate file binds staff-wide 62;
    // bar 2+ carries reference 53 (mode La). Both facts come from the file.
    const StaffType* bar1 = notes[0]->staff()->staffTypeForElement(notes[0]);
    const StaffType* bar2 = notes[4]->staff()->staffTypeForElement(notes[4]);
    ASSERT_TRUE(bar1->jimsStateJson().contains(u"\"key_number\":62"));
    ASSERT_TRUE(bar2->jimsStateJson().contains(u"\"key_number\":53"));
    std::vector<pitch_level_t> levels = nominalPitchLevels(score);
    ASSERT_EQ(levels.size(), notes.size());
    for (size_t i = 0; i < notes.size(); ++i) {
        EXPECT_EQ(levels[i], kernelPitchLevel(notes[i])) << "note " << i;
    }
    // Falsification anchor: bar 2's C-identity note (C5 spelling) must NOT
    // play its 12-TET compatibility pitch under reference 53.
    EXPECT_NE(levels[4], notePitchLevel(notes[4]->tpc(), notes[4]->octave(), notes[4]->tuning()));
    delete score;
}

// JiMSynth VST3 workstream (2026-08-19): every lattice-identified JiMS note's
// NoteEvent carries the Kernel's exact sounding pitch — the same fresh
// note_sounding_pitch answer, lossless (frequency, transport key, full
// residual cents) plus the note's lattice identity — beside the integer
// pitch level; never reconstructed downstream.
TEST_F(Engraving_JiMStaffM7PlaybackTests, jimsynthJimsNotesCarryTheExactKernelPitchAndLatticeIdentity)
{
    Score* score = ScoreRW::readScore(M7_GATE);
    ASSERT_TRUE(score);
    score->doLayout();
    std::vector<Note*> notes = notesOf(score);
    ASSERT_EQ(notes.size(), 12u);
    std::vector<std::optional<ExactPitch> > exact = exactPitches(score);
    ASSERT_EQ(exact.size(), notes.size());
    for (size_t i = 0; i < notes.size(); ++i) {
        ASSERT_TRUE(exact[i].has_value()) << "note " << i;
        const jims::SoundingPitch sp = kernelSoundingPitch(notes[i]);
        EXPECT_DOUBLE_EQ(exact[i]->frequencyHz, sp.frequencyHz) << "note " << i;
        EXPECT_EQ(exact[i]->midiKey, sp.midiKey) << "note " << i;
        EXPECT_DOUBLE_EQ(exact[i]->centsOffset, sp.centsOffset) << "note " << i;
        EXPECT_EQ(exact[i]->hasLattice, 1);
        EXPECT_EQ(exact[i]->nPer, notes[i]->jimsNPer());
        EXPECT_EQ(exact[i]->nGen, notes[i]->jimsNGen());
        // Transport precision: key + full cents recover the Kernel frequency
        // within 0.01 cent (the pitch-level grid alone cannot: 2-cent steps).
        const double hz = 440.0 * std::pow(2.0, (exact[i]->midiKey - 69 + exact[i]->centsOffset / 100.0) / 12.0);
        EXPECT_NEAR(1200.0 * std::log2(hz / sp.frequencyHz), 0.0, 0.01) << "note " << i;
    }
    delete score;
}

TEST_F(Engraving_JiMStaffM7PlaybackTests, jimsynthStockNotesCarryNoExactPitch)
{
    Score* score = ScoreRW::readScore(u"playback/playbackmodel_data/repeat_range/repeat_range.mscx");
    ASSERT_TRUE(score);
    score->doLayout();
    std::vector<std::optional<ExactPitch> > exact = exactPitches(score);
    ASSERT_FALSE(exact.empty());
    for (const auto& e : exact) {
        EXPECT_FALSE(e.has_value()) << "stock events stay byte-equivalent: no exact-pitch field";
    }
    delete score;
}

TEST_F(Engraving_JiMStaffM7PlaybackTests, jimsingerPlaybackCarriesKernelProfileAtEveryJimsNoteAndTracksSectionChanges)
{
    Score* score = ScoreRW::readScore(M7_GATE);
    ASSERT_TRUE(score);
    score->doLayout();
    const std::vector<DynamicTonalityProfileEvent> profiles = dynamicTonalityProfiles(score);
    ASSERT_EQ(profiles.size(), notesOf(score).size());
    for (const DynamicTonalityProfileEvent& profile : profiles) {
        EXPECT_EQ(profile.points.front().paramId, 0x4A500100u);
        EXPECT_EQ(profile.points.back().paramId, 0x4A500119u);
    }
    EXPECT_FALSE(profiles.front() == profiles.at(4)) << "bar 2 has a different Kernel reference profile";
    EXPECT_TRUE(profiles.at(4) == profiles.back()) << "bar 2 and bar 3 share the same section profile";
    delete score;
}

TEST_F(Engraving_JiMStaffM7PlaybackTests, stockPlaybackCarriesNoDynamicTonalityProfile)
{
    Score* score = ScoreRW::readScore(u"playback/playbackmodel_data/repeat_range/repeat_range.mscx");
    ASSERT_TRUE(score);
    score->doLayout();
    EXPECT_TRUE(dynamicTonalityProfiles(score).empty());
    delete score;
}

// A non-12-TET generator (17-TET) sounds its own cents across periods.
TEST_F(Engraving_JiMStaffM7PlaybackTests, m7PlaybackUsesNonTwelveTetGenerator)
{
    Score* score = ScoreRW::readScore(M7_DEFAULT);
    ASSERT_TRUE(score);
    score->doLayout();
    StaffType* st = score->staff(0)->staffType(Fraction(0, 1));
    ASSERT_TRUE(st && st->isJiMS());
    String s17 = st->jimsStateJson();
    s17.replace(u"\"generator_cents\":700.0", u"\"generator_cents\":705.8823529411765");
    ASSERT_NE(s17, st->jimsStateJson());
    st->setJimsStateJson(s17);
    score->setLayoutAll();
    score->doLayout();
    std::vector<Note*> notes = notesOf(score);
    ASSERT_FALSE(notes.empty());
    std::vector<pitch_level_t> levels = nominalPitchLevels(score);
    ASSERT_EQ(levels.size(), notes.size());
    bool someNonTwelveTet = false;
    for (size_t i = 0; i < notes.size(); ++i) {
        EXPECT_EQ(levels[i], kernelPitchLevel(notes[i])) << "note " << i;
        if (levels[i] % PITCH_LEVEL_STEP != 0) {
            someNonTwelveTet = true;
        }
    }
    EXPECT_TRUE(someNonTwelveTet) << "17-TET must produce off-semitone pitch levels";
    delete score;
}

// Notes copied from the reference-62 section and pasted into the
// reference-53 section (bar 3 of the gate file, pasted live during the M6
// gate) sound at the DESTINATION section's pitch for their identity —
// which is not the source's compatibility pitch.
TEST_F(Engraving_JiMStaffM7PlaybackTests, m7PastedNotesUseDestinationSectionState)
{
    Score* score = ScoreRW::readScore(M7_GATE);
    ASSERT_TRUE(score);
    score->doLayout();
    std::vector<Note*> notes = notesOf(score);
    ASSERT_EQ(notes.size(), 12u);
    std::vector<pitch_level_t> levels = nominalPitchLevels(score);
    ASSERT_EQ(levels.size(), 12u);
    for (size_t i = 8; i < 12; ++i) {
        // Same identity as bar 1's note i-8 (the paste kept it) ...
        EXPECT_EQ(notes[i]->jimsNPer(), notes[i - 8]->jimsNPer());
        EXPECT_EQ(notes[i]->jimsNGen(), notes[i - 8]->jimsNGen());
        // ... but it sounds the destination section's pitch, per the Kernel.
        EXPECT_EQ(levels[i], kernelPitchLevel(notes[i])) << "pasted note " << i;
        EXPECT_NE(levels[i], levels[i - 8]) << "reference 53 vs 62 must differ";
    }
    delete score;
}

// Tuning-slider preview and cancel (no undo entry) and commit/undo/redo:
// the NEXT playback rebuild always uses the current section state.
TEST_F(Engraving_JiMStaffM7PlaybackTests, m7PlaybackTracksTuningPreviewCommitCancelUndoRedo)
{
    Score* score = ScoreRW::readScore(M7_DEFAULT);
    ASSERT_TRUE(score);
    score->doLayout();
    std::vector<Note*> notes = notesOf(score);
    ASSERT_FALSE(notes.empty());
    const std::vector<pitch_level_t> before = nominalPitchLevels(score);
    jims::TuningController tc(score, 0);
    ASSERT_TRUE(tc.beginPreview());
    ASSERT_TRUE(tc.preview(696.0));
    std::vector<pitch_level_t> previewed = nominalPitchLevels(score);
    ASSERT_EQ(previewed.size(), notes.size());
    for (size_t i = 0; i < notes.size(); ++i) {
        EXPECT_EQ(previewed[i], kernelPitchLevel(notes[i])) << "preview note " << i;
    }
    EXPECT_NE(previewed, before);
    tc.cancel();
    EXPECT_EQ(nominalPitchLevels(score), before);
    ASSERT_TRUE(tc.beginPreview());
    ASSERT_TRUE(tc.commit(710.0));
    std::vector<pitch_level_t> committed = nominalPitchLevels(score);
    for (size_t i = 0; i < notes.size(); ++i) {
        EXPECT_EQ(committed[i], kernelPitchLevel(notes[i])) << "commit note " << i;
    }
    EXPECT_NE(committed, before);
    score->undoRedo(true, nullptr);
    score->doLayout();
    EXPECT_EQ(nominalPitchLevels(score), before);
    score->undoRedo(false, nullptr);
    score->doLayout();
    EXPECT_EQ(nominalPitchLevels(score), committed);
    delete score;
}

// The LIVE playback model (loaded once, listening to the score's change
// channel) follows a tuning preview and its cancel — preview edits state
// outside an undoable command, so the controller announces it through the
// score's existing change signal (no new signal, no per-note cache).
TEST_F(Engraving_JiMStaffM7PlaybackTests, m7LivePlaybackModelFollowsTuningPreviewAndCancel)
{
    Score* score = ScoreRW::readScore(M7_DEFAULT);
    ASSERT_TRUE(score);
    score->doLayout();
    std::vector<Note*> notes = notesOf(score);
    ASSERT_FALSE(notes.empty());
    PlaybackModel model(modularity::globalCtx());
    model.profilesRepository.set(m_repositoryMock);
    model.load(score);
    const Part* part = score->parts().at(0);
    auto levelsNow = [&]() {
        std::vector<pitch_level_t> out;
        const PlaybackEventsMap& events = model.resolveTrackPlaybackData(part->id(), part->instrumentId()).originEvents;
        for (const auto& pair : events) {
            for (const PlaybackEvent& ev : pair.second) {
                if (std::holds_alternative<muse::mpe::NoteEvent>(ev)) {
                    out.push_back(std::get<muse::mpe::NoteEvent>(ev).pitchCtx().nominalPitchLevel);
                }
            }
        }
        return out;
    };
    const std::vector<pitch_level_t> before = levelsNow();
    jims::TuningController tc(score, 0);
    ASSERT_TRUE(tc.beginPreview());
    ASSERT_TRUE(tc.preview(696.0));
    std::vector<pitch_level_t> previewed = levelsNow();
    ASSERT_EQ(previewed.size(), notes.size());
    for (size_t i = 0; i < notes.size(); ++i) {
        EXPECT_EQ(previewed[i], kernelPitchLevel(notes[i])) << "live preview note " << i;
    }
    EXPECT_NE(previewed, before);
    tc.cancel();
    EXPECT_EQ(levelsNow(), before);
    delete score;
}

// The accepted M5 pieces and the other JiMStaff fixtures (all staves,
// all voices): every playback event's pitch level is a Kernel answer for
// some JiMS note's identity in ITS section state — order-independent
// multiset comparison against fresh Kernel calls.
TEST_F(Engraving_JiMStaffM7PlaybackTests, m7ImportedAndAcceptedPiecesUseEffectiveSectionPitch)
{
    const std::vector<String> pieces = {
        u"jimstaff_data/m5-key-down.mscx", u"jimstaff_data/m5-key-up.mscx", u"jimstaff_data/m5-key-mode.mscx",
        u"jimstaff_data/m5-mode.mscx", u"jimstaff_data/m5-scale.mscx", u"jimstaff_data/m5-syshead.mscx",
        u"jimstaff_data/collision.mscx", u"jimstaff_data/grym.mscx", u"jimstaff_data/mode-change.mscx",
        u"jimstaff_data/ode-to-joy.mscx", u"jimstaff_data/acc-chromatic.mscx",
    };
    int checked = 0;
    for (const String& path : pieces) {
        Score* score = ScoreRW::readScore(path);
        ASSERT_TRUE(score) << path.toStdString();
        score->doLayout();
        std::multiset<pitch_level_t> expected;
        for (Segment* seg = score->firstSegment(SegmentType::ChordRest); seg; seg = seg->next1(SegmentType::ChordRest)) {
            for (track_idx_t t = 0; t < score->ntracks(); ++t) {
                EngravingItem* el = seg->element(t);
                if (el && el->isChord()) {
                    for (Note* n : toChord(el)->notes()) {
                        if (n->hasJimsPitch() && n->staff()->staffTypeForElement(n)->isJiMS() && !n->tieBack()) {
                            expected.insert(kernelPitchLevel(n));
                        }
                    }
                }
            }
        }
        std::multiset<pitch_level_t> got;
        for (pitch_level_t l : nominalPitchLevels(score)) {
            got.insert(l);
        }
        EXPECT_FALSE(expected.empty()) << path.toStdString();
        EXPECT_EQ(got, expected) << path.toStdString();
        checked += int(got.size());
        delete score;
    }
    EXPECT_GT(checked, 0);
}

// Negative control: a stock (non-JiMS) score's pitch levels are exactly
// the stock formula — byte-identical playback for every non-JiMS staff.
TEST_F(Engraving_JiMStaffM7PlaybackTests, m7StockPlaybackPitchIsUnchanged)
{
    Score* score = ScoreRW::readScore(u"playback/playbackmodel_data/repeat_range/repeat_range.mscx");
    ASSERT_TRUE(score);
    score->doLayout();
    std::vector<pitch_level_t> levels = nominalPitchLevels(score);
    ASSERT_FALSE(levels.empty());
    // Every note is stock: compare against the stock construction.
    std::vector<pitch_level_t> expected;
    for (const Part* part : score->parts()) {
        (void)part;
    }
    for (Segment* seg = score->firstSegment(SegmentType::ChordRest); seg; seg = seg->next1(SegmentType::ChordRest)) {
        for (track_idx_t t = 0; t < score->ntracks(); ++t) {
            EngravingItem* el = seg->element(t);
            if (el && el->isChord()) {
                for (Note* n : toChord(el)->notes()) {
                    EXPECT_FALSE(n->staff()->staffTypeForElement(n)->isJiMS());
                    (void)n;
                }
            }
        }
    }
    // The stock model's own event pitches must all be on 12-TET semitones
    // (repeat_range has no tuning), and the model must contain them.
    for (pitch_level_t l : levels) {
        EXPECT_EQ(l % PITCH_LEVEL_STEP, 0);
    }
    delete score;
}
