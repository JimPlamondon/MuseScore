/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 *
 * JiMStaff Milestone 10 — song-wide versus per-staff state.
 */

#include <gtest/gtest.h>

#include "engraving/dom/chord.h"
#include "engraving/dom/instrument.h"
#include "engraving/dom/masterscore.h"
#include "engraving/dom/measure.h"
#include "engraving/dom/note.h"
#include "engraving/dom/part.h"
#include "engraving/dom/score.h"
#include "engraving/dom/segment.h"
#include "engraving/dom/staff.h"
#include "engraving/dom/stafftype.h"
#include "engraving/editing/editscoreproperties.h"
#include "engraving/jims/jimsbridge.h"
#include "engraving/jims/jimschange.h"
#include "engraving/jims/jimschangecontroller.h"

#include "utils/scorerw.h"

using namespace mu::engraving;

namespace {
muse::String forkRoot()
{
    return ScoreRW::rootPath() + u"/../../..";
}

muse::String satbTemplatePath()
{
    return forkRoot() + u"/share/templates/02-Choral/12-SATB_(JiMStaff)/12-SATB_(JiMStaff).mscx";
}

std::vector<Note*> notesOn(Score* score, staff_idx_t staffIdx)
{
    std::vector<Note*> out;
    for (Segment* segment = score->firstSegment(SegmentType::ChordRest); segment;
         segment = segment->next1(SegmentType::ChordRest)) {
        for (track_idx_t track = staffIdx * VOICES; track < (staffIdx + 1) * VOICES; ++track) {
            EngravingItem* item = segment->element(track);
            if (item && item->isChord()) {
                for (Note* note : toChord(item)->notes()) {
                    out.push_back(note);
                }
            }
        }
    }
    return out;
}

muse::String melodyJson(const std::vector<Note*>& notes)
{
    muse::String json = u"{\"notes\":[";
    for (size_t i = 0; i < notes.size(); ++i) {
        if (i) {
            json += u",";
        }
        json += muse::String(u"{\"nPer\":%1,\"nGen\":%2}")
                .arg(notes[i]->jimsNPer()).arg(notes[i]->jimsNGen());
    }
    return json + u"]}";
}

muse::String extentXml(const muse::String& state)
{
    muse::String xml;
    muse::String error;
    if (!jims::musicxmlStaffStateV3Xml(state, 0, xml, &error)) {
        return muse::String();
    }
    const size_t begin = xml.indexOf(u"<jims:extent");
    const size_t end = begin == muse::nidx ? muse::nidx : xml.indexOf(u"/>", begin);
    return begin == muse::nidx || end == muse::nidx ? muse::String() : xml.mid(begin, end + 2 - begin);
}
}

TEST(Engraving_JiMStaffM10SATBTests, everyEmptyVocalStaffUsesItsKernelRangeCentre)
{
    MasterScore* score = ScoreRW::readScore(satbTemplatePath(), true);
    ASSERT_TRUE(score);
    ASSERT_EQ(score->nstaves(), 4u);
    EXPECT_EQ(jims::reconcileExtents(score), 0) << "native load must already reconcile every empty vocal extent";
    const char* roles[4] = { "soprano", "alto", "tenor", "bass" };
    const double expectedDoOrigins[4] = { 900.0, 200.0, 700.0, 400.0 };
    for (staff_idx_t i = 0; i < 4; ++i) {
        ASSERT_TRUE(notesOn(score, i).empty()) << "rests do not make a written extent";
        Staff* staff = score->staff(i);
        const StaffType* type = staff->staffType(Fraction(0, 1));
        ASSERT_TRUE(type && type->isJiMS());
        const muse::String role = muse::String::fromAscii(roles[i]);
        EXPECT_TRUE(staff->part()->instrumentId() == role || staff->part()->instrumentId() == u"voice." + role)
            << staff->part()->instrumentId().toStdString();
        const Instrument* instrument = staff->part()->instrument();
        muse::String expected;
        ASSERT_TRUE(jims::defaultVocalExtent(type->jimsStateJson(), instrument->minPitchA(),
                                             instrument->maxPitchA(), roles[i], expected));
        EXPECT_TRUE(extentXml(type->jimsStateJson()) == extentXml(expected))
            << i << " actual=" << extentXml(type->jimsStateJson()).toStdString()
            << " expected=" << extentXml(expected).toStdString()
            << " range=" << instrument->minPitchA() << ".." << instrument->maxPitchA();
        EXPECT_FALSE(extentXml(type->jimsStateJson()).empty());
        jims::PeriodicOrigins origins;
        ASSERT_TRUE(jims::periodicOrigins(type->jimsStateJson(), origins));
        EXPECT_DOUBLE_EQ(origins.doCentsAboveExtentLower, expectedDoOrigins[i]);
        EXPECT_DOUBLE_EQ(origins.tonicCentsAboveExtentLower, expectedDoOrigins[i]);
    }

    Measure* second = score->firstMeasure()->nextMeasure();
    ASSERT_TRUE(second);
    muse::String error;
    ASSERT_TRUE(jims::applyChangeToAllJimsParts(score, second, { u"key:-1:3" }, error)) << error.toStdString();
    for (staff_idx_t i = 0; i < 4; ++i) {
        Staff* staff = score->staff(i);
        const StaffType* type = staff->staffType(second->tick());
        const Instrument* instrument = staff->part()->instrument();
        muse::String expected;
        ASSERT_TRUE(jims::defaultVocalExtent(type->jimsStateJson(), instrument->minPitchA(),
                                             instrument->maxPitchA(), roles[i], expected));
        const muse::String after = extentXml(type->jimsStateJson());
        EXPECT_TRUE(after == extentXml(expected)) << "each transposed empty staff must use its Kernel default";
    }
    delete score;
}

TEST(Engraving_JiMStaffM10SATBTests, extentGrowsOnlyUntilSaveAndContractsOnlyOnReload)
{
    MasterScore* score = ScoreRW::readScore(u"jimstaff_data/m9-satb-mixed.mscx");
    ASSERT_TRUE(score);
    score->setJimsMelodyPart(jims::MelodyPart::Bass); // isolate extent from melody analysis
    std::vector<Note*> notes = notesOn(score, 0);
    ASSERT_FALSE(notes.empty());
    StaffType* type = score->staff(0)->staffType(Fraction(0, 1));
    ASSERT_TRUE(type && type->isJiMS());

    muse::String unchanged;
    ASSERT_TRUE(jims::widenExtent(type->jimsStateJson(), notes.front()->jimsNPer(),
                                  notes.front()->jimsNGen(), unchanged));
    EXPECT_TRUE(unchanged == type->jimsStateJson()) << "in-range entry is a no-op";

    const int originalNPer = notes.front()->jimsNPer();
    const int originalNGen = notes.front()->jimsNGen();
    muse::String widened;
    ASSERT_TRUE(jims::widenExtent(type->jimsStateJson(), -10, originalNGen, widened));
    notes.front()->setJimsPitch(-10, originalNGen);
    ASSERT_TRUE(jims::widenExtentForNote(notes.front()));
    EXPECT_TRUE(type->jimsStateJson() == widened);

    notes.front()->setJimsPitch(originalNPer, originalNGen);
    EXPECT_FALSE(jims::widenExtentForNote(notes.front()));
    EXPECT_TRUE(type->jimsStateJson() == widened) << "an inward move must not contract in-session";

    const muse::String expectedMelody = melodyJson(notesOn(score, 0));
    muse::String expectedFit;
    ASSERT_TRUE(jims::fitExtent(widened, expectedMelody, expectedFit));
    const muse::String saved = forkRoot() + u"/build.m10/m10-extent-lifecycle.mscx";
    ASSERT_TRUE(ScoreRW::saveScore(score, saved));
    EXPECT_TRUE(type->jimsStateJson() == widened) << "save must not contract";
    delete score;

    MasterScore* reloaded = ScoreRW::readScore(saved, true);
    ASSERT_TRUE(reloaded);
    EXPECT_TRUE(reloaded->staff(0)->staffType(Fraction(0, 1))->jimsStateJson() == expectedFit)
        << "reload is the sole contraction transition";
    delete reloaded;
}

TEST(Engraving_JiMStaffM10SATBTests, melodyDesignationDefaultsOverridesAndUndoRedoDrivesOneSongWideAmbit)
{
    MasterScore* score = ScoreRW::readScore(u"jimstaff_data/m9-satb-hymn.mscx");
    ASSERT_TRUE(score);
    ASSERT_EQ(score->jimsMelodyPart(), jims::MelodyPart::Soprano);
    std::vector<Note*> soprano = notesOn(score, 0);
    std::vector<Note*> tenor = notesOn(score, 2);
    ASSERT_FALSE(soprano.empty());
    ASSERT_GE(tenor.size(), 3u);
    for (Note* note : soprano) {
        note->setJimsPitch(0, 0);
    }
    static const int centered[][2] = { { -1, -1 }, { -2, 1 }, { 0, -2 }, { -1, 0 }, { 1, -3 } };
    for (size_t i = 0; i < tenor.size(); ++i) {
        const size_t position = i < 5 ? i : 4;
        tenor[i]->setJimsPitch(centered[position][0], centered[position][1]);
    }
    muse::String sopranoToken;
    muse::String tenorToken;
    ASSERT_TRUE(jims::tonicAmbitForMelody(score->staff(0)->staffType(Fraction(0, 1))->jimsStateJson(),
                                          melodyJson(soprano), sopranoToken));
    ASSERT_TRUE(jims::tonicAmbitForMelody(score->staff(2)->staffType(Fraction(0, 1))->jimsStateJson(),
                                          melodyJson(tenor), tenorToken));
    ASSERT_FALSE(sopranoToken == tenorToken);
    ASSERT_GT(jims::deriveTonicAmbits(score), 0);
    for (staff_idx_t i = 0; i < 4; ++i) {
        EXPECT_TRUE(score->staff(i)->staffType(Fraction(0, 1))->jimsTonicAmbit() == sopranoToken);
    }

    score->startCmd(TranslatableString("undoableAction", "Test JiMS melody part"));
    score->undo(new ChangeJimsMelodyPart(score, jims::MelodyPart::Tenor));
    score->endCmd();
    EXPECT_EQ(score->jimsMelodyPart(), jims::MelodyPart::Tenor);
    for (staff_idx_t i = 0; i < 4; ++i) {
        EXPECT_TRUE(score->staff(i)->staffType(Fraction(0, 1))->jimsTonicAmbit() == tenorToken);
    }
    score->undoRedo(true, nullptr);
    EXPECT_EQ(score->jimsMelodyPart(), jims::MelodyPart::Soprano);
    EXPECT_TRUE(score->staff(0)->staffType(Fraction(0, 1))->jimsTonicAmbit() == sopranoToken);
    score->undoRedo(false, nullptr);
    EXPECT_EQ(score->jimsMelodyPart(), jims::MelodyPart::Tenor);
    EXPECT_TRUE(score->staff(0)->staffType(Fraction(0, 1))->jimsTonicAmbit() == tenorToken);

    const muse::String saved = forkRoot() + u"/build.m10/m10-melody-native.mscx";
    ASSERT_TRUE(ScoreRW::saveScore(score, saved));
    delete score;
    MasterScore* reloaded = ScoreRW::readScore(saved, true);
    ASSERT_TRUE(reloaded);
    EXPECT_EQ(reloaded->jimsMelodyPart(), jims::MelodyPart::Tenor);
    delete reloaded;
}
