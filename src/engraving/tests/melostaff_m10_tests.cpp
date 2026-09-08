/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 *
 * JiMStaff Milestone 10 — song-wide versus per-staff state.
 */

#include <gtest/gtest.h>

#include <functional>

#include "engraving/dom/chord.h"
#include "engraving/dom/factory.h"
#include "engraving/dom/instrument.h"
#include "engraving/dom/input.h"
#include "engraving/dom/masterscore.h"
#include "engraving/dom/measure.h"
#include "engraving/dom/note.h"
#include "engraving/dom/part.h"
#include "engraving/dom/score.h"
#include "engraving/dom/segment.h"
#include "engraving/dom/staff.h"
#include "engraving/dom/stafflines.h"
#include "engraving/dom/stafftype.h"
#include "engraving/dom/stafftypechange.h"
#include "engraving/dom/system.h"
#include "engraving/editing/editscoreproperties.h"
#include "engraving/melo/melobridge.h"
#include "engraving/melo/melochange.h"
#include "engraving/melo/melochangecontroller.h"
#include "engraving/melo/melotuningcontroller.h"
#include "draw/bufferedpaintprovider.h"
#include "draw/painter.h"
#include "io/dir.h"

#include "utils/scorerw.h"

using namespace mu::engraving;
using namespace mu::engraving::rendering;
using namespace muse;
using namespace muse::draw;

namespace {
muse::String forkRoot()
{
    return ScoreRW::rootPath() + u"/../../..";
}

muse::String satbTemplatePath()
{
    return forkRoot() + u"/share/templates/02-Choral/12-SATB_(MeloPresto_Staff)/12-SATB_(MeloPresto_Staff).mscx";
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
                .arg(notes[i]->meloNPer()).arg(notes[i]->meloNGen());
    }
    return json + u"]}";
}

muse::String extentXml(const muse::String& state)
{
    muse::String xml;
    muse::String error;
    if (!melo::musicxmlStaffStateV3Xml(state, 0, xml, &error)) {
        return muse::String();
    }
    const size_t begin = xml.indexOf(u"<jims:extent");
    const size_t end = begin == muse::nidx ? muse::nidx : xml.indexOf(u"/>", begin);
    return begin == muse::nidx || end == muse::nidx ? muse::String() : xml.mid(begin, end + 2 - begin);
}

double generatorCents(const StaffType* type)
{
    double generator = 0.0;
    double period = 0.0;
    return type && melo::staffMetrics(type->meloStateJson(), generator, period) ? generator : -1.0;
}

std::vector<String> textsOf(const StaffLines* lines)
{
    std::shared_ptr<BufferedPaintProvider> provider = std::make_shared<BufferedPaintProvider>();
    Painter painter(provider, "m10");
    painter.setViewport(RectF(0, 0, 4000, 4000));
    PaintOptions options;
    lines->renderer()->drawItem(lines, &painter, options);
    painter.endDraw();
    std::vector<String> texts;
    std::function<void(const DrawData::Item&)> walk = [&](const DrawData::Item& item) {
        for (const DrawData::Data& data : item.datas) {
            for (const DrawText& text : data.texts) {
                texts.push_back(text.text);
            }
        }
        for (const DrawData::Item& child : item.chilren) {
            walk(child);
        }
    };
    walk(provider->drawData()->item);
    return texts;
}

size_t tuningLabelCount(const StaffLines* lines)
{
    size_t count = 0;
    for (const String& text : textsOf(lines)) {
        count += text.startsWith(u"M5=") ? 1 : 0;
    }
    return count;
}
}

TEST(Engraving_MeloStaffM10SATBTests, tuningUpdatesMidMeasureCarriersAndCancelRestoresThem)
{
    MasterScore* score = ScoreRW::readScore(satbTemplatePath(), true);
    ASSERT_TRUE(score);
    Measure* measure = score->firstMeasure();
    const Fraction tick = measure->tick() + Fraction(1, 4);
    String error;
    ASSERT_TRUE(melo::applyChange(score, 0, measure, tick, u"mode:1", error)) << error.toStdString();
    const StaffType* base = score->staff(0)->staffType(Fraction(0, 1));
    const StaffType* middle = score->staff(0)->staffType(tick);
    ASSERT_NE(base, middle);
    const String beforeBase = base->meloStateJson();
    const String beforeMiddle = middle->meloStateJson();
    melo::TuningController controller(score, 0);
    ASSERT_TRUE(controller.beginPreview());
    ASSERT_TRUE(controller.preview(696.0));
    EXPECT_DOUBLE_EQ(generatorCents(base), 696.0);
    EXPECT_DOUBLE_EQ(generatorCents(middle), 696.0);
    controller.cancel();
    EXPECT_EQ(base->meloStateJson(), beforeBase);
    EXPECT_EQ(middle->meloStateJson(), beforeMiddle);
    ASSERT_TRUE(controller.beginPreview());
    ASSERT_TRUE(controller.commit(710.0));
    EXPECT_DOUBLE_EQ(generatorCents(base), 710.0);
    EXPECT_DOUBLE_EQ(generatorCents(middle), 710.0);
    score->undoRedo(true, nullptr);
    EXPECT_EQ(base->meloStateJson(), beforeBase);
    EXPECT_EQ(middle->meloStateJson(), beforeMiddle);
    score->undoRedo(false, nullptr);
    EXPECT_DOUBLE_EQ(generatorCents(middle), 710.0);
    delete score;
}

TEST(Engraving_MeloStaffM10SATBTests, tuningCrossesNoteOrderWithoutChangingLatticeIdentities)
{
    MasterScore* score = ScoreRW::readScore(u"jimstaff_data/m9-satb-mixed.mscx");
    ASSERT_TRUE(score);
    melo::TuningController controller(score, 0);
    ASSERT_TRUE(controller.beginPreview());
    ASSERT_TRUE(controller.commit(696.0));
    const std::vector<Note*> notes = notesOn(score, 0);
    ASSERT_GE(notes.size(), 2u);
    for (size_t i = 0; i < notes.size(); ++i) {
        notes[i]->setMeloPitch(i % 2 ? 0 : -7, i % 2 ? 0 : 12);
    }
    melo::reconcileExtents(score);
    const String before = score->staff(0)->staffType(Fraction(0, 1))->meloStateJson();
    ASSERT_TRUE(controller.beginPreview());
    ASSERT_TRUE(controller.preview(710.0));
    for (size_t i = 0; i < notes.size(); ++i) {
        EXPECT_EQ(notes[i]->meloNPer(), i % 2 ? 0 : -7);
        EXPECT_EQ(notes[i]->meloNGen(), i % 2 ? 0 : 12);
    }
    ASSERT_TRUE(controller.commit(710.0));
    const auto* type = score->staff(0)->staffType(Fraction(0, 1));
    double first = 0.0;
    double second = 0.0;
    ASSERT_TRUE(melo::noteCentsAboveExtentLower(type->meloStateJson(), -7, 12, first));
    ASSERT_TRUE(melo::noteCentsAboveExtentLower(type->meloStateJson(), 0, 0, second));
    EXPECT_GT(first, second);
    score->undoRedo(true, nullptr);
    EXPECT_EQ(type->meloStateJson(), before);
    score->undoRedo(false, nullptr);
    EXPECT_DOUBLE_EQ(generatorCents(type), 710.0);
    delete score;
}

TEST(Engraving_MeloStaffM10SATBTests, tuningFromAnyVoiceIsSharedAndOneUndoStepPreservesEveryExtent)
{
    MasterScore* score = ScoreRW::readScore(satbTemplatePath(), true);
    ASSERT_TRUE(score);
    ASSERT_EQ(score->nstaves(), 4u);
    Measure* second = score->firstMeasure()->nextMeasure();
    ASSERT_TRUE(second);
    String error;
    ASSERT_TRUE(melo::applyChangeToAllMeloParts(score, second, { u"mode:1" }, error)) << error.toStdString();
    String originalStates[4][2];
    String originalExtents[4][2];
    for (staff_idx_t i = 0; i < 4; ++i) {
        const Fraction ticks[2] = { Fraction(0, 1), second->tick() };
        for (size_t span = 0; span < 2; ++span) {
            const StaffType* type = score->staff(i)->staffType(ticks[span]);
            ASSERT_TRUE(type && type->isMelo());
            originalStates[i][span] = type->meloStateJson();
            originalExtents[i][span] = extentXml(originalStates[i][span]);
            EXPECT_DOUBLE_EQ(generatorCents(type), 700.0);
        }
    }
    const size_t undoBefore = score->undoStack()->currentIndex();

    melo::TuningController controller(score, 3);
    ASSERT_TRUE(controller.beginPreview());
    ASSERT_TRUE(controller.preview(690.0));
    for (staff_idx_t i = 0; i < 4; ++i) {
        const Fraction ticks[2] = { Fraction(0, 1), second->tick() };
        for (size_t span = 0; span < 2; ++span) {
            const StaffType* type = score->staff(i)->staffType(ticks[span]);
            EXPECT_DOUBLE_EQ(generatorCents(type), 690.0) << "preview missed staff " << i << " span " << span;
            EXPECT_EQ(extentXml(type->meloStateJson()), originalExtents[i][span])
                << "preview changed staff " << i << " extent span " << span;
        }
    }
    EXPECT_EQ(score->undoStack()->currentIndex(), undoBefore);

    controller.cancel();
    for (staff_idx_t i = 0; i < 4; ++i) {
        EXPECT_EQ(score->staff(i)->staffType(Fraction(0, 1))->meloStateJson(), originalStates[i][0]);
        EXPECT_EQ(score->staff(i)->staffType(second->tick())->meloStateJson(), originalStates[i][1]);
    }

    ASSERT_TRUE(controller.beginPreview());
    ASSERT_TRUE(controller.preview(690.0));
    ASSERT_TRUE(controller.commit(690.0));
    EXPECT_EQ(score->undoStack()->currentIndex(), undoBefore + 1);
    for (staff_idx_t i = 0; i < 4; ++i) {
        const Fraction ticks[2] = { Fraction(0, 1), second->tick() };
        for (size_t span = 0; span < 2; ++span) {
            const StaffType* type = score->staff(i)->staffType(ticks[span]);
            EXPECT_DOUBLE_EQ(generatorCents(type), 690.0) << "commit missed staff " << i << " span " << span;
            EXPECT_EQ(extentXml(type->meloStateJson()), originalExtents[i][span])
                << "commit changed staff " << i << " extent span " << span;
        }
    }
    score->undoRedo(true, nullptr);
    for (staff_idx_t i = 0; i < 4; ++i) {
        EXPECT_EQ(score->staff(i)->staffType(Fraction(0, 1))->meloStateJson(), originalStates[i][0]);
        EXPECT_EQ(score->staff(i)->staffType(second->tick())->meloStateJson(), originalStates[i][1]);
    }
    score->undoRedo(false, nullptr);
    for (staff_idx_t i = 0; i < 4; ++i) {
        EXPECT_DOUBLE_EQ(generatorCents(score->staff(i)->staffType(Fraction(0, 1))), 690.0);
        EXPECT_DOUBLE_EQ(generatorCents(score->staff(i)->staffType(second->tick())), 690.0);
    }
    delete score;
}

TEST(Engraving_MeloStaffM10SATBTests, tuningIndicatorAppearsOnlyOnTheTopVisibleMeloStaff)
{
    MasterScore* score = ScoreRW::readScore(satbTemplatePath(), true);
    ASSERT_TRUE(score);
    score->doLayout();
    ASSERT_FALSE(score->systems().empty());
    System* system = nullptr;
    for (System* candidate : score->systems()) {
        if (candidate->firstMeasure()) {
            system = candidate;
            break;
        }
    }
    ASSERT_TRUE(system);

    const size_t expectedWithSoprano[4] = { 1, 0, 0, 0 };
    for (staff_idx_t i = 0; i < 4; ++i) {
        EXPECT_EQ(tuningLabelCount(system->firstMeasure()->staffLines(i)), expectedWithSoprano[i]) << "staff " << i;
    }

    system->staff(0)->setShow(false);
    const size_t expectedWithSopranoElided[4] = { 0, 1, 0, 0 };
    for (staff_idx_t i = 0; i < 4; ++i) {
        EXPECT_EQ(tuningLabelCount(system->firstMeasure()->staffLines(i)), expectedWithSopranoElided[i]) << "staff " << i;
    }
    delete score;
}

TEST(Engraving_MeloStaffM10SATBTests, referenceChangesPreserveWrittenAndEmptyStaffGeometry)
{
    const String paths[] = { satbTemplatePath(), ScoreRW::rootPath() + u"/jimstaff_data/m9-satb-mixed.mscx" };
    for (const String& path : paths) {
        for (double generator : { 686.0, 696.0, 720.0 }) {
            MasterScore* score = ScoreRW::readScore(path, true);
            ASSERT_TRUE(score);
            melo::TuningController controller(score, 0);
            ASSERT_TRUE(controller.beginPreview());
            ASSERT_TRUE(controller.commit(generator));
            std::vector<StaffType::MeloFrameView> before;
            std::vector<String> extents;
            for (staff_idx_t i = 0; i < score->nstaves(); ++i) {
                const StaffType* type = score->staff(i)->staffType(Fraction(0, 1));
                before.push_back(type->meloWholeFrameView(score, i));
                extents.push_back(extentXml(type->meloStateJson()));
            }
            const auto writtenNotes = notesOn(score, 0);
            std::vector<std::pair<int, int> > identities;
            for (const Note* note : writtenNotes) {
                identities.push_back({ note->meloNPer(), note->meloNGen() });
            }
            for (const String& key : { String(u"key:0:2"), String(u"key:-1:3"), String(u"key:1:-7") }) {
                String error;
                ASSERT_TRUE(melo::applyChangeToAllMeloParts(score, score->firstMeasure(), { key }, error))
                    << error.toStdString();
                score->doLayout();
                for (staff_idx_t i = 0; i < score->nstaves(); ++i) {
                    const StaffType* type = score->staff(i)->staffType(Fraction(0, 1));
                    EXPECT_EQ(extentXml(type->meloStateJson()), extents[i]);
                    const auto& after = type->meloWholeFrameView(score, i);
                    EXPECT_NEAR(after.bottomCents(), before[i].bottomCents(), 1e-6);
                    EXPECT_NEAR(after.topCents(), before[i].topCents(), 1e-6);
                }
                for (size_t i = 0; i < writtenNotes.size(); ++i) {
                    EXPECT_EQ(writtenNotes[i]->meloNPer(), identities[i].first);
                    EXPECT_EQ(writtenNotes[i]->meloNGen(), identities[i].second);
                }
                score->undoRedo(true, nullptr);
                score->doLayout();
            }
            delete score;
        }
    }
}

TEST(Engraving_MeloStaffM10SATBTests, firstNoteReplacesEmptyCentreAndUndoRestoresIt)
{
    MasterScore* score = ScoreRW::readScore(satbTemplatePath(), true);
    ASSERT_TRUE(score);
    StaffType* type = score->staff(0)->staffType(Fraction(0, 1));
    ASSERT_TRUE(type->meloExtentIsEmptyDefault());
    const String before = type->meloStateJson();
    String expected;
    ASSERT_TRUE(melo::fitExtent(before, u"{\"notes\":[{\"nPer\":0,\"nGen\":0}]}", expected));
    InputState& input = score->inputState();
    input.setTrack(0);
    input.setSegment(score->tick2segment(Fraction(0, 1), false, SegmentType::ChordRest));
    input.setDuration(DurationType::V_QUARTER);
    input.setNoteEntryMode(true);
    score->startCmd(TranslatableString::untranslatable("First MeloPresto note"));
    score->cmdAddPitch(5 * 7 + 1, false, false); // Host letter input for Re0's default D4.
    score->endCmd();
    score->doLayout();
    ASSERT_EQ(notesOn(score, 0).size(), 1u);
    EXPECT_EQ(extentXml(type->meloStateJson()), extentXml(expected));
    EXPECT_FALSE(type->meloExtentIsEmptyDefault());
    const auto& view = type->meloWholeFrameView(score, 0);
    EXPECT_NEAR(view.bottomCents(), -type->meloPeriodCents() / 4.0, 1e-6);
    EXPECT_NEAR(view.topCents(), type->meloPeriodCents() / 4.0, 1e-6);
    score->undoRedo(true, nullptr);
    EXPECT_EQ(type->meloStateJson(), before);
    EXPECT_TRUE(type->meloExtentIsEmptyDefault());
    EXPECT_TRUE(notesOn(score, 0).empty());
    score->undoRedo(false, nullptr);
    EXPECT_EQ(extentXml(type->meloStateJson()), extentXml(expected));
    EXPECT_FALSE(type->meloExtentIsEmptyDefault());
    delete score;
}

TEST(Engraving_MeloStaffM10SATBTests, pointerEntryKeepsEarlierNotesAtTheirPitchWhenTheFrameGrows)
{
    MasterScore* score = ScoreRW::readScore(satbTemplatePath(), true);
    ASSERT_TRUE(score);
    score->doLayout();
    StaffType* type = score->staff(0)->staffType(Fraction(0, 1));
    InputState& input = score->inputState();
    input.setTrack(0);
    input.setSegment(score->tick2segment(Fraction(0, 1), false, SegmentType::ChordRest));
    input.setDuration(DurationType::V_QUARTER);
    input.setNoteEntryMode(true);
    // The first two soprano notes of Bach 057: Mi1 followed by Do1.
    // Go through the same height-to-note seam as a pointer click.
    for (const auto& identity : { std::make_pair(0, 2), std::make_pair(2, -2) }) {
        double cents = 0.0;
        ASSERT_TRUE(melo::noteCentsAboveExtentLower(type->meloStateJson(), identity.first, identity.second, cents));
        type->meloEnsureFrame(score, 0);
        Position position;
        position.segment = input.segment();
        position.staffIdx = 0;
        position.line = int(std::lround(2.0 * (type->meloFrameTopCents() - cents)
                                        / StaffType::MELO_CENTS_PER_LINE_DISTANCE));
        position.fret = INVALID_FRET_INDEX;
        bool error = false;
        NoteVal value = score->noteValForPosition(position, AccidentalType::NONE, error);
        ASSERT_FALSE(error);
        score->startCmd(TranslatableString::untranslatable("Place note by height"));
        score->addPitch(value, false);
        score->endCmd();
        score->doLayout();
        auto notes = notesOn(score, 0);
        ASSERT_FALSE(notes.empty());
        EXPECT_EQ(notes.back()->meloNPer(), identity.first);
        EXPECT_EQ(notes.back()->meloNGen(), identity.second);
        for (Note* note : notes) {
            double expected = 0.0;
            ASSERT_TRUE(melo::noteCentsAboveExtentLower(type->meloStateJson(), note->meloNPer(), note->meloNGen(), expected));
            EXPECT_NEAR(note->meloCentsAboveDo(), expected, 1e-6);
        }
    }
    const auto notes = notesOn(score, 0);
    ASSERT_EQ(notes.size(), 2u);
    EXPECT_LT(notes[0]->ldata()->pos().y(), notes[1]->ldata()->pos().y());
    delete score;
}

TEST(Engraving_MeloStaffM10SATBTests, everyEmptyVocalStaffUsesItsKernelRangeCentre)
{
    MasterScore* score = ScoreRW::readScore(satbTemplatePath(), true);
    ASSERT_TRUE(score);
    ASSERT_EQ(score->nstaves(), 4u);
    EXPECT_EQ(melo::reconcileExtents(score), 0) << "native load must already reconcile every empty vocal extent";
    const char* roles[4] = { "soprano", "alto", "tenor", "bass" };
    const double expectedDoOrigins[4] = { 300.0, 800.0, 100.0, 1000.0 };
    for (staff_idx_t i = 0; i < 4; ++i) {
        ASSERT_TRUE(notesOn(score, i).empty()) << "rests do not make a written extent";
        Staff* staff = score->staff(i);
        const StaffType* type = staff->staffType(Fraction(0, 1));
        ASSERT_TRUE(type && type->isMelo());
        const muse::String role = muse::String::fromAscii(roles[i]);
        EXPECT_TRUE(staff->part()->instrumentId() == role || staff->part()->instrumentId() == u"voice." + role)
            << staff->part()->instrumentId().toStdString();
        const Instrument* instrument = staff->part()->instrument();
        muse::String expected;
        ASSERT_TRUE(melo::defaultVocalExtent(type->meloStateJson(), instrument->minPitchA(),
                                             instrument->maxPitchA(), roles[i], expected));
        EXPECT_TRUE(extentXml(type->meloStateJson()) == extentXml(expected))
            << i << " actual=" << extentXml(type->meloStateJson()).toStdString()
            << " expected=" << extentXml(expected).toStdString()
            << " range=" << instrument->minPitchA() << ".." << instrument->maxPitchA();
        EXPECT_FALSE(extentXml(type->meloStateJson()).empty());
        melo::PeriodicOrigins origins;
        ASSERT_TRUE(melo::periodicOrigins(type->meloStateJson(), origins));
        EXPECT_DOUBLE_EQ(origins.doCentsAboveExtentLower, expectedDoOrigins[i]);
        EXPECT_DOUBLE_EQ(origins.tonicCentsAboveExtentLower, expectedDoOrigins[i]);
    }

    Measure* second = score->firstMeasure()->nextMeasure();
    ASSERT_TRUE(second);
    muse::String error;
    ASSERT_TRUE(melo::applyChangeToAllMeloParts(score, second, { u"key:-1:3" }, error)) << error.toStdString();
    for (staff_idx_t i = 0; i < 4; ++i) {
        Staff* staff = score->staff(i);
        const StaffType* type = staff->staffType(second->tick());
        const Instrument* instrument = staff->part()->instrument();
        muse::String expected;
        ASSERT_TRUE(melo::defaultVocalExtent(type->meloStateJson(), instrument->minPitchA(),
                                             instrument->maxPitchA(), roles[i], expected));
        const muse::String after = extentXml(type->meloStateJson());
        EXPECT_TRUE(after == extentXml(expected)) << "each transposed empty staff must use its Kernel default";
    }
    delete score;
}

// A later empty section must not acquire the written range of an earlier one.
TEST(Engraving_MeloStaffM10SATBTests, eachStaffTypeSpanCollectsOnlyItsOwnNotes)
{
    MasterScore* score = ScoreRW::readScore(u"jimstaff_data/m9-satb-mixed.mscx");
    ASSERT_TRUE(score);
    Staff* staff = score->staff(0);
    Measure* second = score->firstMeasure()->nextMeasure();
    ASSERT_TRUE(second);
    const std::vector<Note*> notes = notesOn(score, 0);
    ASSERT_FALSE(notes.empty());
    score->startCmd(TranslatableString::untranslatable("Create an empty MeloPresto section"));
    std::vector<Chord*> later;
    for (Note* note : notes) {
        if (note->tick() >= second->tick()) {
            if (std::find(later.begin(), later.end(), note->chord()) == later.end()) {
                later.push_back(note->chord());
            }
        } else {
            note->setMeloPitch(-3, 0);
        }
    }
    for (Chord* chord : later) {
        score->deleteItem(chord);
    }
    score->endCmd();
    auto* carrier = Factory::createStaffTypeChange(second);
    carrier->setParent(second);
    carrier->setTrack(0);
    carrier->setStaffType(new StaffType(*staff->staffType(Fraction(0, 1))), true);
    score->addElement(carrier);
    melo::reconcileExtents(score);
    score->setLayoutAll();
    score->doLayout();
    const StaffType* empty = staff->staffType(second->tick());
    ASSERT_NE(empty, staff->staffType(Fraction(0, 1)));
    ASSERT_TRUE(empty->meloExtentIsEmptyDefault());
    for (bool elide : { false, true }) {
        score->style().set(Sid::meloElideEmptyOctaves, elide);
        score->style().set(Sid::meloShowAllOctavesInFirstSystem, false);
        score->setLayoutAll();
        score->doLayout();
        const auto& view = empty->meloFrameView(score, 0, second->system());
        ASSERT_EQ(view.bands.size(), 1u);
        EXPECT_NEAR(view.bottomCents(), -empty->meloPeriodCents() / 4.0, 1e-6);
        EXPECT_NEAR(view.topCents(), empty->meloPeriodCents() / 4.0, 1e-6);
    }
    delete score;
}

TEST(Engraving_MeloStaffM10SATBTests, extentGrowsOnlyUntilSaveAndContractsOnlyOnReload)
{
    MasterScore* score = ScoreRW::readScore(u"jimstaff_data/m9-satb-mixed.mscx");
    ASSERT_TRUE(score);
    score->setMeloMelodyPart(melo::MelodyPart::Bass); // isolate extent from melody analysis
    std::vector<Note*> notes = notesOn(score, 0);
    ASSERT_FALSE(notes.empty());
    StaffType* type = score->staff(0)->staffType(Fraction(0, 1));
    ASSERT_TRUE(type && type->isMelo());

    muse::String unchanged;
    ASSERT_TRUE(melo::widenExtent(type->meloStateJson(), notes.front()->meloNPer(),
                                  notes.front()->meloNGen(), unchanged));
    EXPECT_TRUE(unchanged == type->meloStateJson()) << "in-range entry is a no-op";

    const int originalNPer = notes.front()->meloNPer();
    const int originalNGen = notes.front()->meloNGen();
    muse::String widened;
    ASSERT_TRUE(melo::widenExtent(type->meloStateJson(), -10, originalNGen, widened));
    notes.front()->setMeloPitch(-10, originalNGen);
    ASSERT_TRUE(melo::widenExtentForNote(notes.front()));
    EXPECT_TRUE(type->meloStateJson() == widened);

    notes.front()->setMeloPitch(originalNPer, originalNGen);
    EXPECT_FALSE(melo::widenExtentForNote(notes.front()));
    EXPECT_TRUE(type->meloStateJson() == widened) << "an inward move must not contract in-session";

    const muse::String expectedMelody = melodyJson(notesOn(score, 0));
    muse::String expectedFit;
    ASSERT_TRUE(melo::fitExtent(widened, expectedMelody, expectedFit));
    const muse::String outputDir = forkRoot() + u"/build.m10";
    ASSERT_TRUE(muse::io::Dir::mkpath(outputDir));
    const muse::String saved = outputDir + u"/m10-extent-lifecycle.mscx";
    ASSERT_TRUE(ScoreRW::saveScore(score, saved));
    EXPECT_TRUE(type->meloStateJson() == widened) << "save must not contract";
    delete score;

    MasterScore* reloaded = ScoreRW::readScore(saved, true);
    ASSERT_TRUE(reloaded);
    EXPECT_TRUE(reloaded->staff(0)->staffType(Fraction(0, 1))->meloStateJson() == expectedFit)
        << "reload is the sole contraction transition";
    delete reloaded;
}

TEST(Engraving_MeloStaffM10SATBTests, ratioLineExtentSurvivesWhileLatticeCoverageReconciles)
{
    MasterScore* score = ScoreRW::readScore(u"jimstaff_data/m9-satb-mixed.mscx");
    ASSERT_TRUE(score);
    StaffType* type = score->staff(0)->staffType(Fraction(0, 1));
    ASSERT_TRUE(type && type->isMelo());
    const std::vector<Note*> notes = notesOn(score, 0);
    ASSERT_FALSE(notes.empty());

    String declared;
    ASSERT_TRUE(melo::widenExtent(type->meloStateJson(), -10, notes.front()->meloNGen(), declared));
    ASSERT_NE(declared, type->meloStateJson());
    type->setMeloStateJson(declared);
    const String ratioExtent
        = u"{\"lower\":{\"period\":-1,\"ratio\":\"3/2\"},"
          u"\"upper\":{\"period\":0,\"ratio\":\"1/1\"}}";
    type->setMeloRatioLineExtentJson(ratioExtent);

    EXPECT_EQ(melo::reconcileExtents(score), 1);
    EXPECT_NE(type->meloStateJson(), declared)
        << "lattice extent remains note-coverage data even with a fixed display extent";
    EXPECT_EQ(type->meloRatioLineExtentJson(), ratioExtent)
        << "reconciling note coverage must not alter the fixed ratio-line display extent";
    delete score;
}

TEST(Engraving_MeloStaffM10SATBTests, melodyDesignationDefaultsOverridesAndUndoRedoDrivesOneSongWideAmbit)
{
    MasterScore* score = ScoreRW::readScore(u"jimstaff_data/m9-satb-hymn.mscx");
    ASSERT_TRUE(score);
    ASSERT_EQ(score->meloMelodyPart(), melo::MelodyPart::Soprano);
    std::vector<Note*> soprano = notesOn(score, 0);
    std::vector<Note*> tenor = notesOn(score, 2);
    ASSERT_FALSE(soprano.empty());
    ASSERT_GE(tenor.size(), 3u);
    for (Note* note : soprano) {
        note->setMeloPitch(0, 0);
    }
    static const int centered[][2] = { { -1, -1 }, { -2, 1 }, { 0, -2 }, { -1, 0 }, { 1, -3 } };
    for (size_t i = 0; i < tenor.size(); ++i) {
        const size_t position = i < 5 ? i : 4;
        tenor[i]->setMeloPitch(centered[position][0], centered[position][1]);
    }
    muse::String sopranoToken;
    muse::String tenorToken;
    ASSERT_TRUE(melo::tonicAmbitForMelody(score->staff(0)->staffType(Fraction(0, 1))->meloStateJson(),
                                          melodyJson(soprano), sopranoToken));
    ASSERT_TRUE(melo::tonicAmbitForMelody(score->staff(2)->staffType(Fraction(0, 1))->meloStateJson(),
                                          melodyJson(tenor), tenorToken));
    ASSERT_FALSE(sopranoToken == tenorToken);
    // The fixture already carries the singleton soprano's bounded token. Seed
    // a different valid Kernel state so the positive-change assertion below
    // tests a real transition rather than an idempotent derivation.
    score->setMeloMelodyPart(melo::MelodyPart::Tenor);
    ASSERT_GT(melo::deriveTonicAmbits(score), 0);
    for (staff_idx_t i = 0; i < 4; ++i) {
        ASSERT_TRUE(score->staff(i)->staffType(Fraction(0, 1))->meloTonicAmbit() == tenorToken);
    }
    score->setMeloMelodyPart(melo::MelodyPart::Soprano);
    ASSERT_GT(melo::deriveTonicAmbits(score), 0);
    for (staff_idx_t i = 0; i < 4; ++i) {
        EXPECT_TRUE(score->staff(i)->staffType(Fraction(0, 1))->meloTonicAmbit() == sopranoToken);
    }

    score->startCmd(TranslatableString("undoableAction", "Test MeloPresto melody part"));
    score->undo(new ChangeMeloMelodyPart(score, melo::MelodyPart::Tenor));
    score->endCmd();
    EXPECT_EQ(score->meloMelodyPart(), melo::MelodyPart::Tenor);
    for (staff_idx_t i = 0; i < 4; ++i) {
        EXPECT_TRUE(score->staff(i)->staffType(Fraction(0, 1))->meloTonicAmbit() == tenorToken);
    }
    score->undoRedo(true, nullptr);
    EXPECT_EQ(score->meloMelodyPart(), melo::MelodyPart::Soprano);
    EXPECT_TRUE(score->staff(0)->staffType(Fraction(0, 1))->meloTonicAmbit() == sopranoToken);
    score->undoRedo(false, nullptr);
    EXPECT_EQ(score->meloMelodyPart(), melo::MelodyPart::Tenor);
    EXPECT_TRUE(score->staff(0)->staffType(Fraction(0, 1))->meloTonicAmbit() == tenorToken);

    const muse::String outputDir = forkRoot() + u"/build.m10";
    ASSERT_TRUE(muse::io::Dir::mkpath(outputDir));
    const muse::String saved = outputDir + u"/m10-melody-native.mscx";
    ASSERT_TRUE(ScoreRW::saveScore(score, saved));
    delete score;
    MasterScore* reloaded = ScoreRW::readScore(saved, true);
    ASSERT_TRUE(reloaded);
    EXPECT_EQ(reloaded->meloMelodyPart(), melo::MelodyPart::Tenor);
    delete reloaded;
}
