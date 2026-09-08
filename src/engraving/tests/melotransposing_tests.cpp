// SPDX-License-Identifier: GPL-3.0-only
// MuseScore-Studio-CLA-applies
#include <gtest/gtest.h>
#include <memory>
#include <fstream>
#include "engraving/dom/chord.h"
#include "engraving/dom/instrument.h"
#include "engraving/dom/instrtemplate.h"
#include "engraving/dom/masterscore.h"
#include "engraving/dom/note.h"
#include "engraving/dom/part.h"
#include "engraving/dom/segment.h"
#include "engraving/dom/staff.h"
#include "engraving/dom/stafftype.h"
#include "engraving/melo/melochangecontroller.h"
#include "engraving/melo/melochange.h"
#include "engraving/editing/editpart.h"
#include "engraving/dom/measure.h"
#include "engraving/editing/editdata.h"
#include "engraving/editing/undo.h"
#include "engraving/editing/transpose.h"
#include "engraving/dom/instrchange.h"
#include "utils/scorerw.h"

using namespace mu::engraving;

namespace {
// Inherited MuseScore definitions and conventional note handling are the oracle.
const char16_t* instruments[] = {
    u"flute", u"bb-clarinet", u"a-clarinet", u"soprano-saxophone",
    u"alto-saxophone", u"tenor-saxophone", u"baritone-saxophone",
    u"horn", u"english-horn", u"bb-trumpet", u"alto-flute", u"piccolo", u"double-bass",
};
Note* firstNote(Score* score)
{
    return toChord(score->firstSegment(SegmentType::ChordRest)->element(0))->notes().front();
}
}

TEST(Engraving_MeloTransposing, ConcertIdentityHasInstrumentWrittenSpelling)
{
    for (const auto& instrument : instruments) {
        SCOPED_TRACE(muse::String(instrument).toStdString());
        std::unique_ptr<MasterScore> score(ScoreRW::readScore(u"jimstaff_data/collision.mscx"));
        ASSERT_TRUE(score);
        Note* note = firstNote(score.get());
        const InstrumentTemplate* definition = searchTemplate(instrument);
        ASSERT_TRUE(definition);
        const Instrument inherited = Instrument::fromTemplate(definition);
        score->parts().front()->setInstrument(inherited);
        std::unique_ptr<MasterScore> traditional(ScoreRW::readScore(u"jimstaff_data/collision.mscx"));
        ASSERT_TRUE(traditional);
        traditional->parts().front()->setInstrument(inherited);
        traditional->staff(0)->setStaffType(Fraction(0, 1), *StaffType::preset(StaffTypes::STANDARD));
        Note* oracle = firstNote(traditional.get());
        oracle->setPitch(60);
        oracle->setTpcFromPitch();
        note->setMeloPitch(1, -2);
        note->setPitch(60, Tpc::TPC_C, Tpc::TPC_C);
        EXPECT_EQ(note->pitch(), 60);
        EXPECT_EQ(note->tpc1(), Tpc::TPC_C);
        EXPECT_EQ(note->tpc2(), oracle->tpc2());
        EXPECT_EQ(note->pitch() - note->transposition(), oracle->pitch() - oracle->transposition());
        EXPECT_EQ(note->meloNPer(), 1);
        EXPECT_EQ(note->meloNGen(), -2);
    }
}

TEST(Engraving_MeloTransposing, NormalizationRepairsWrittenSpellingAndThenIsIdempotent)
{
    std::unique_ptr<MasterScore> score(ScoreRW::readScore(u"jimstaff_data/collision.mscx"));
    ASSERT_TRUE(score);
    score->parts().front()->setInstrument(Instrument::fromTemplate(searchTemplate(u"bb-clarinet")));
    Note* note = firstNote(score.get());
    note->setMeloPitch(1, -2);
    note->setPitch(60, Tpc::TPC_C, Tpc::TPC_C);
    note->setTpc2(Tpc::TPC_C); // Deliberately reproduce a historical malformed field.
    size_t repairs = 0;
    muse::String error;
    ASSERT_TRUE(melo::normalizeStoredPitchesAfterLoad(score.get(), repairs, error, false)) << error.toStdString();
    EXPECT_GT(repairs, 0u);
    EXPECT_EQ(note->pitch(), 60);
    EXPECT_EQ(note->tpc2(), Tpc::TPC_D);
    ASSERT_TRUE(melo::normalizeStoredPitchesAfterLoad(score.get(), repairs, error, false)) << error.toStdString();
    EXPECT_EQ(repairs, 0u);
}

TEST(Engraving_MeloTransposing, OnlyJammersRequireMeloStaves)
{
    for (bool jammer : { false, true }) {
        std::unique_ptr<MasterScore> score(ScoreRW::readScore(u"jimstaff_data/collision.mscx"));
        ASSERT_TRUE(score);
        score->parts().front()->instrument()->setId(jammer ? u"melo-jammer" : u"bb-clarinet");
        Staff* staff = score->staff(0);
        ASSERT_TRUE(staff->staffType(Fraction(0, 1))->isMelo());
        score->startCmd(muse::TranslatableString::untranslatable("Change notation"));
        EditPart::setStaffType(score.get(), staff, StaffTypes::STANDARD);
        score->endCmd();
        EXPECT_EQ(staff->staffType(Fraction(0, 1))->isMelo(), jammer);
    }
}

TEST(Engraving_MeloTransposing, ChoosingMeloForTraditionalClarinetPreservesConcertMusic)
{
    std::unique_ptr<MasterScore> score(ScoreRW::readScore(u"note_data/tpc-transpose.mscx"));
    ASSERT_TRUE(score);
    Note* note = firstNote(score.get());
    const int pitch = note->pitch();
    const int written = note->tpc2();
    ASSERT_FALSE(note->hasMeloPitch());
    score->startCmd(muse::TranslatableString::untranslatable("Choose Melo notation"));
    EditPart::setStaffType(score.get(), score->staff(0), StaffTypes::MELO_12TET);
    score->endCmd();
    ASSERT_TRUE(note->hasMeloPitch());
    melo::StateChangeOptions options;
    ASSERT_TRUE(melo::stateChangeOptions(score->staff(0)->staffType(Fraction(0, 1))->meloStateJson(), options));
    EXPECT_TRUE(options.concertC);
    melo::SoundingPitch sounding;
    ASSERT_TRUE(melo::noteSoundingPitch(score->staff(0)->staffType(Fraction(0, 1))->meloStateJson(), note->meloNPer(), note->meloNGen(),
                                        sounding));
    EXPECT_EQ(sounding.midiKey, pitch);
    EXPECT_EQ(note->tpc2(), written);
    EditData data;
    score->undoStack()->undo(&data);
    EXPECT_FALSE(score->staff(0)->staffType(Fraction(0, 1))->isMelo());
    EXPECT_FALSE(note->hasMeloPitch());
    EXPECT_EQ(note->pitch(), pitch);
    score->undoStack()->redo(&data);
    EXPECT_TRUE(score->staff(0)->staffType(Fraction(0, 1))->isMelo());
    EXPECT_EQ(note->pitch(), pitch);
}

TEST(Engraving_MeloTransposing, RefusedConversionLeavesNotationAndSoundUntouched)
{
    std::unique_ptr<MasterScore> score(ScoreRW::readScore(u"note_data/tpc-transpose.mscx"));
    ASSERT_TRUE(score);
    Note* note = firstNote(score.get());
    note->setTuning(13.0);
    const auto before = std::make_tuple(note->pitch(), note->tpc1(), note->tpc2(), note->tuning());
    muse::String error;
    score->startCmd(muse::TranslatableString::untranslatable("Change notation"));
    EXPECT_FALSE(EditPart::setStaffType(score.get(), score->staff(0), StaffTypes::MELO_12TET, &error));
    score->endCmd();
    EXPECT_FALSE(error.isEmpty());
    EXPECT_FALSE(score->staff(0)->staffType(Fraction(0, 1))->isMelo());
    EXPECT_FALSE(note->hasMeloPitch());
    EXPECT_EQ(std::make_tuple(note->pitch(), note->tpc1(), note->tpc2(), note->tuning()), before);
}

TEST(Engraving_MeloTransposing, CatalogueJammerUsesMovableMeloWhenCreatedOrReplaced)
{
    ASSERT_TRUE(loadInstrumentTemplates(muse::String(u":/engraving/instruments/melo-instruments.xml")));
    const InstrumentTemplate* definition = searchTemplate(u"melo-jammer");
    ASSERT_TRUE(definition);
    const Instrument jammer = Instrument::fromTemplate(definition);
    ASSERT_TRUE(jammer.isMeloJammer());
    std::unique_ptr<MasterScore> score(ScoreRW::readScore(u"note_data/tpc-transpose.mscx"));
    ASSERT_TRUE(score);
    const int pitch = firstNote(score.get())->pitch();
    const int written = firstNote(score.get())->tpc2();
    score->startCmd(muse::TranslatableString::untranslatable("Choose Jammer"));
    EditPart::replacePartInstrument(score.get(), score->parts().front(), jammer);
    score->endCmd();
    ASSERT_TRUE(score->staff(0)->staffType(Fraction(0, 1))->isMelo());
    melo::StateChangeOptions options;
    ASSERT_TRUE(melo::stateChangeOptions(score->staff(0)->staffType(Fraction(0, 1))->meloStateJson(), options));
    EXPECT_FALSE(options.concertC);
    EXPECT_EQ(firstNote(score.get())->pitch(), pitch);
    EditData data;
    score->undoStack()->undo(&data);
    EXPECT_FALSE(score->staff(0)->staffType(Fraction(0, 1))->isMelo());
    EXPECT_EQ(firstNote(score.get())->tpc2(), written);
    score->undoStack()->redo(&data);
    score->staff(0)->init(definition, StaffType::preset(StaffTypes::STANDARD), 0);
    EXPECT_TRUE(score->staff(0)->staffType(Fraction(0, 1))->isMelo());
}

TEST(Engraving_MeloTransposing, LinkedTraditionalStaffUsesItsOwnInstrumentSpelling)
{
    std::unique_ptr<MasterScore> meloScore(ScoreRW::readScore(u"jimstaff_data/collision.mscx"));
    std::unique_ptr<MasterScore> traditional(ScoreRW::readScore(u"jimstaff_data/collision.mscx"));
    ASSERT_TRUE(meloScore && traditional);
    meloScore->parts().front()->setInstrument(Instrument::fromTemplate(searchTemplate(u"bb-clarinet")));
    traditional->parts().front()->setInstrument(Instrument::fromTemplate(searchTemplate(u"a-clarinet")));
    traditional->staff(0)->setStaffType(Fraction(0, 1), *StaffType::preset(StaffTypes::STANDARD));
    Note* source = firstNote(meloScore.get());
    Note* linked = firstNote(traditional.get());
    source->setMeloPitch(1, -2);
    source->setPitch(60, Tpc::TPC_C, Tpc::TPC_C);
    linked->setPitch(60);
    linked->setTpcFromPitch();
    linked->linkTo(source);
    meloScore->startCmd(muse::TranslatableString::untranslatable("Transpose linked notes"));
    ASSERT_TRUE(source->transpose(Interval(4, 7), true));
    meloScore->endCmd();
    EXPECT_EQ(linked->pitch(), source->pitch());
    const int stored = linked->tpc2();
    linked->setTpcFromPitch(); // The inherited standard-notation oracle.
    EXPECT_EQ(stored, linked->tpc2());
    EXPECT_NE(source->tpc2(), linked->tpc2());
}

TEST(Engraving_MeloTransposing, TimedInstrumentChangeKeepsConcertNotesAndUndoes)
{
    std::unique_ptr<MasterScore> score(ScoreRW::readScore(u"jimstaff_data/collision.mscx"));
    ASSERT_TRUE(score);
    Part* part = score->parts().front();
    part->setInstrument(Instrument::fromTemplate(searchTemplate(u"bb-clarinet")));
    size_t repairs = 0;
    muse::String error;
    ASSERT_TRUE(melo::normalizeStoredPitchesAfterLoad(score.get(), repairs, error, false));
    Segment* at = score->firstMeasure()->nextMeasure()->first(SegmentType::ChordRest);
    ASSERT_TRUE(at);
    Note* before = firstNote(score.get());
    Note* after = toChord(at->element(0))->notes().front();
    std::unique_ptr<MasterScore> linkedScore(ScoreRW::readScore(u"jimstaff_data/collision.mscx"));
    ASSERT_TRUE(linkedScore);
    linkedScore->parts().front()->setInstrument(Instrument::fromTemplate(searchTemplate(u"horn")));
    linkedScore->staff(0)->setStaffType(Fraction(0, 1), *StaffType::preset(StaffTypes::STANDARD));
    Note* linked = firstNote(linkedScore.get());
    linked->setPitch(after->pitch());
    linked->setTpcFromPitch();
    const int linkedWritten = linked->tpc2();
    linked->linkTo(after);
    const auto original = std::make_tuple(after->pitch(), after->tpc2(), after->meloNPer(), after->meloNGen());
    const int beforeWritten = before->tpc2();
    auto change = new InstrumentChange(at);
    change->setParent(at);
    change->setTrack(0);
    change->setInit(true);
    Instrument instrument = Instrument::fromTemplate(searchTemplate(u"a-clarinet"));
    score->startCmd(muse::TranslatableString::untranslatable("Change clarinet"));
    score->undoAddElement(change);
    change->setupInstrument(&instrument);
    score->endCmd();
    EXPECT_EQ(before->tpc2(), beforeWritten);
    EXPECT_EQ(after->pitch(), std::get<0>(original));
    EXPECT_EQ(after->meloNPer(), std::get<2>(original));
    EXPECT_EQ(after->meloNGen(), std::get<3>(original));
    Interval written = instrument.transpose();
    written.flip();
    EXPECT_EQ(after->tpc2(), Transpose::transposeTpc(after->tpc1(), written, true));
    EXPECT_EQ(linked->tpc2(), linkedWritten);
    EditData data;
    score->undoStack()->undo(&data);
    EXPECT_EQ(std::make_tuple(after->pitch(), after->tpc2(), after->meloNPer(), after->meloNGen()), original);
    EXPECT_EQ(linked->tpc2(), linkedWritten);
}

TEST(Engraving_MeloTransposing, SharedPassageHasIdenticalConcertProjectionAndGeometry)
{
    std::unique_ptr<MasterScore> score(ScoreRW::readScore(u"jimstaff_data/m9-satb-hymn.mscx"));
    ASSERT_TRUE(score);
    ASSERT_EQ(score->parts().size(), 4u);
    const char16_t* ids[] = { u"bb-clarinet", u"alto-saxophone", u"tenor-saxophone", u"horn" };
    muse::String error;
    for (size_t index = 0; index < 4; ++index) {
        score->startCmd(muse::TranslatableString::untranslatable("Choose acoustic instrument"));
        EditPart::replacePartInstrument(score.get(), score->parts()[index], Instrument::fromTemplate(searchTemplate(ids[index])));
        score->endCmd();
        ASSERT_TRUE(melo::applyChange(score.get(), index, score->firstMeasure(), u"notation:concert", error)) << error.toStdString();
    }
    const muse::String state = score->staff(0)->staffType(Fraction(0, 1))->meloStateJson();
    for (staff_idx_t index = 1; index < 4; ++index) {
        score->staff(index)->staffType(Fraction(0, 1))->setMeloStateJson(state);
    }
    std::ofstream events("shared-concert-passage-events.csv");
    events << "tick,staff,n_per,n_gen,frequency_hz,midi_key,cents\n";
    std::vector<std::pair<Note*, Note*> > geometryPairs;
    for (Segment* segment = score->firstSegment(SegmentType::ChordRest); segment;
         segment = segment->next1(SegmentType::ChordRest)) {
        EngravingItem* first = segment->element(0);
        if (!first || !first->isChord()) {
            continue;
        }
        Note* melody = toChord(first)->notes().front();
        melo::SoundingPitch expected;
        ASSERT_TRUE(melo::noteSoundingPitch(state, melody->meloNPer(), melody->meloNGen(), expected));
        for (staff_idx_t index = 0; index < 4; ++index) {
            EngravingItem* item = segment->element(index * VOICES);
            ASSERT_TRUE(item && item->isChord());
            for (Note* note : toChord(item)->notes()) {
                note->setMeloPitch(melody->meloNPer(), melody->meloNGen());
                note->setPitch(expected.midiKey, melody->tpc1(), note->writtenTpcForConcert(melody->tpc1()));
                note->setTuning(expected.centsOffset);
                melo::SoundingPitch actual;
                ASSERT_TRUE(melo::noteSoundingPitch(state, note->meloNPer(), note->meloNGen(), actual));
                EXPECT_DOUBLE_EQ(actual.frequencyHz, expected.frequencyHz);
                geometryPairs.emplace_back(note, melody);
                events << segment->tick().ticks() << ',' << index << ',' << note->meloNPer() << ',' << note->meloNGen()
                       << ',' << actual.frequencyHz << ',' << actual.midiKey << ',' << actual.centsOffset << '\n';
            }
        }
    }
    score->setMetaTag(u"workTitle", u"Shared concert passage");
    score->doLayout();
    for (const auto& pair : geometryPairs) {
        EXPECT_DOUBLE_EQ(pair.first->meloPosY(pair.first->staff()->staffType(Fraction(0, 1))),
                         pair.second->meloPosY(pair.second->staff()->staffType(Fraction(0, 1))));
    }
    ASSERT_TRUE(ScoreRW::saveScore(score.get(), u"shared-concert-passage.mscx"));
}

TEST(Engraving_MeloTransposing, ConcertReferenceSurvivesSaveAndTransposeUndo)
{
    std::unique_ptr<MasterScore> score(ScoreRW::readScore(u"jimstaff_data/collision.mscx"));
    ASSERT_TRUE(score);
    muse::String error;
    ASSERT_TRUE(melo::applyChange(score.get(), 0, score->firstMeasure(), u"notation:concert", error)) << error.toStdString();
    Note* note = firstNote(score.get());
    const int nPer = note->meloNPer();
    const int nGen = note->meloNGen();
    const int pitch = note->pitch();
    score->startCmd(muse::TranslatableString::untranslatable("Transpose music"));
    ASSERT_TRUE(note->transpose(Interval(4, 7), true));
    score->endCmd();
    EXPECT_EQ(note->meloNPer(), nPer);
    EXPECT_EQ(note->meloNGen(), nGen + 1);
    EXPECT_EQ(note->pitch(), pitch + 7);
    EditData data;
    score->undoStack()->undo(&data);
    EXPECT_EQ(note->meloNGen(), nGen);
    EXPECT_EQ(note->pitch(), pitch);
    ASSERT_TRUE(ScoreRW::saveScore(score.get(), u"concert-reference.mscx"));
    std::unique_ptr<MasterScore> reopened(ScoreRW::readScore(u"concert-reference.mscx", true));
    ASSERT_TRUE(reopened);
    melo::StateChangeOptions options;
    ASSERT_TRUE(melo::stateChangeOptions(reopened->staff(0)->staffType(Fraction(0, 1))->meloStateJson(), options));
    EXPECT_TRUE(options.concertC);
    EXPECT_EQ(firstNote(reopened.get())->meloNGen(), nGen);
    EXPECT_EQ(firstNote(reopened.get())->pitch(), pitch);
}

TEST(Engraving_MeloTransposing, EveryPartCarriesTheIndicatorButOnlyJammersChangeMapping)
{
    std::unique_ptr<MasterScore> score(ScoreRW::readScore(u"jimstaff_data/m9-satb-hymn.mscx"));
    ASSERT_TRUE(score);
    score->parts().front()->instrument()->setId(u"melo-jammer");
    muse::String error;
    for (staff_idx_t i = 1; i < score->nstaves(); ++i) {
        ASSERT_TRUE(melo::applyChange(score.get(), i, score->firstMeasure(), u"notation:concert", error)) << error.toStdString();
    }
    Measure* measure = score->firstMeasure()->nextMeasure();
    ASSERT_TRUE(measure);
    std::vector<std::tuple<Note*, int, int, int> > notes;
    for (Segment* segment = measure->first(SegmentType::ChordRest); segment;
         segment = segment->next1(SegmentType::ChordRest)) {
        for (staff_idx_t i = 0; i < score->nstaves(); ++i) {
            EngravingItem* item = segment->element(i * VOICES);
            if (item && item->isChord()) {
                for (Note* note : toChord(item)->notes()) {
                    notes.emplace_back(note, note->pitch(), note->meloNPer(), note->meloNGen());
                }
            }
        }
    }
    ASSERT_TRUE(melo::applyChangeToAllMeloParts(score.get(), measure, { u"key:-1:3" }, error)) << error.toStdString();
    score->doLayout();
    for (staff_idx_t i = 0; i < score->nstaves(); ++i) {
        ASSERT_TRUE(melo::changeCarrierAt(measure, i, measure->tick()));
        melo::ChangeIndicator indicator;
        const StaffType* oldType = score->staff(i)->staffType(Fraction(0, 1));
        const StaffType* newType = score->staff(i)->staffType(measure->tick());
        ASSERT_TRUE(melo::changeIndicator(oldType->meloStateJson(), newType->meloStateJson(), indicator));
        EXPECT_FALSE(indicator.empty());
        muse::String expectedShared, actualShared;
        ASSERT_TRUE(melo::musicxmlSharedStateV3Xml(score->staff(0)->staffType(measure->tick())->meloStateJson(), expectedShared));
        ASSERT_TRUE(melo::musicxmlSharedStateV3Xml(newType->meloStateJson(), actualShared));
        EXPECT_EQ(actualShared, expectedShared);
    }
    for (const auto& saved : notes) {
        Note* note = std::get<0>(saved);
        EXPECT_EQ(note->meloNPer(), std::get<2>(saved));
        EXPECT_EQ(note->meloNGen(), std::get<3>(saved));
        if (note->staffIdx() == 0) {
            EXPECT_NE(note->pitch(), std::get<1>(saved));
        } else {
            EXPECT_EQ(note->pitch(), std::get<1>(saved));
        }
    }
    EditData undo;
    score->undoStack()->undo(&undo);
    for (const auto& saved : notes) {
        EXPECT_EQ(std::get<0>(saved)->pitch(), std::get<1>(saved));
    }
}

TEST(Engraving_MeloTransposing, RejectsJammerWithConventionalOrFixedNotationOnLoad)
{
    for (bool conventional : { true, false }) {
        std::unique_ptr<MasterScore> score(ScoreRW::readScore(u"jimstaff_data/collision.mscx"));
        ASSERT_TRUE(score);
        muse::String error;
        if (conventional) {
            score->staff(0)->setStaffType(Fraction(0, 1), *StaffType::preset(StaffTypes::STANDARD));
        } else {
            ASSERT_TRUE(melo::applyChange(score.get(), 0, score->firstMeasure(), u"notation:concert", error));
        }
        score->parts().front()->instrument()->setId(u"melo-jammer");
        ASSERT_TRUE(ScoreRW::saveScore(score.get(), u"invalid-jammer-notation.mscx"));
        std::unique_ptr<MasterScore> reopened(ScoreRW::readScore(u"invalid-jammer-notation.mscx", true));
        EXPECT_FALSE(reopened);
    }
}
