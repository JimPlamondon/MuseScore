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
#include "engraving/jims/jimsbridge.h"
#include "engraving/jims/jimschange.h"
#include "engraving/jims/jimschangecontroller.h"
#include "engraving/jims/jimstuningcontroller.h"
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

double generatorCents(const StaffType* type)
{
    double generator = 0.0;
    double period = 0.0;
    return type && jims::staffMetrics(type->jimsStateJson(), generator, period) ? generator : -1.0;
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

TEST(Engraving_JiMStaffM10SATBTests, tuningUpdatesMidMeasureCarriersAndCancelRestoresThem)
{
    MasterScore* score = ScoreRW::readScore(satbTemplatePath(), true);
    ASSERT_TRUE(score);
    Measure* measure = score->firstMeasure();
    const Fraction tick = measure->tick() + Fraction(1, 4);
    String error;
    ASSERT_TRUE(jims::applyChange(score, 0, measure, tick, u"mode:1", error)) << error.toStdString();
    const StaffType* base = score->staff(0)->staffType(Fraction(0, 1));
    const StaffType* middle = score->staff(0)->staffType(tick);
    ASSERT_NE(base, middle);
    const String beforeBase = base->jimsStateJson();
    const String beforeMiddle = middle->jimsStateJson();
    jims::TuningController controller(score, 0);
    ASSERT_TRUE(controller.beginPreview());
    ASSERT_TRUE(controller.preview(696.0));
    EXPECT_DOUBLE_EQ(generatorCents(base), 696.0);
    EXPECT_DOUBLE_EQ(generatorCents(middle), 696.0);
    controller.cancel();
    EXPECT_EQ(base->jimsStateJson(), beforeBase);
    EXPECT_EQ(middle->jimsStateJson(), beforeMiddle);
    ASSERT_TRUE(controller.beginPreview());
    ASSERT_TRUE(controller.commit(710.0));
    EXPECT_DOUBLE_EQ(generatorCents(base), 710.0);
    EXPECT_DOUBLE_EQ(generatorCents(middle), 710.0);
    score->undoRedo(true, nullptr);
    EXPECT_EQ(base->jimsStateJson(), beforeBase);
    EXPECT_EQ(middle->jimsStateJson(), beforeMiddle);
    score->undoRedo(false, nullptr);
    EXPECT_DOUBLE_EQ(generatorCents(middle), 710.0);
    delete score;
}

TEST(Engraving_JiMStaffM10SATBTests, tuningCrossesNoteOrderWithoutChangingLatticeIdentities)
{
    MasterScore* score = ScoreRW::readScore(u"jimstaff_data/m9-satb-mixed.mscx");
    ASSERT_TRUE(score);
    jims::TuningController controller(score, 0);
    ASSERT_TRUE(controller.beginPreview());
    ASSERT_TRUE(controller.commit(696.0));
    const std::vector<Note*> notes = notesOn(score, 0);
    ASSERT_GE(notes.size(), 2u);
    for (size_t i = 0; i < notes.size(); ++i) {
        notes[i]->setJimsPitch(i % 2 ? 0 : -7, i % 2 ? 0 : 12);
    }
    jims::reconcileExtents(score);
    const String before = score->staff(0)->staffType(Fraction(0, 1))->jimsStateJson();
    ASSERT_TRUE(controller.beginPreview());
    ASSERT_TRUE(controller.preview(710.0));
    for (size_t i = 0; i < notes.size(); ++i) {
        EXPECT_EQ(notes[i]->jimsNPer(), i % 2 ? 0 : -7);
        EXPECT_EQ(notes[i]->jimsNGen(), i % 2 ? 0 : 12);
    }
    ASSERT_TRUE(controller.commit(710.0));
    const auto* type = score->staff(0)->staffType(Fraction(0, 1));
    double first = 0.0;
    double second = 0.0;
    ASSERT_TRUE(jims::noteCentsAboveExtentLower(type->jimsStateJson(), -7, 12, first));
    ASSERT_TRUE(jims::noteCentsAboveExtentLower(type->jimsStateJson(), 0, 0, second));
    EXPECT_GT(first, second);
    score->undoRedo(true, nullptr);
    EXPECT_EQ(type->jimsStateJson(), before);
    score->undoRedo(false, nullptr);
    EXPECT_DOUBLE_EQ(generatorCents(type), 710.0);
    delete score;
}

TEST(Engraving_JiMStaffM10SATBTests, tuningFromAnyVoiceIsSharedAndOneUndoStepPreservesEveryExtent)
{
    MasterScore* score = ScoreRW::readScore(satbTemplatePath(), true);
    ASSERT_TRUE(score);
    ASSERT_EQ(score->nstaves(), 4u);
    Measure* second = score->firstMeasure()->nextMeasure();
    ASSERT_TRUE(second);
    String error;
    ASSERT_TRUE(jims::applyChangeToAllJimsParts(score, second, { u"mode:1" }, error)) << error.toStdString();
    String originalStates[4][2];
    String originalExtents[4][2];
    for (staff_idx_t i = 0; i < 4; ++i) {
        const Fraction ticks[2] = { Fraction(0, 1), second->tick() };
        for (size_t span = 0; span < 2; ++span) {
            const StaffType* type = score->staff(i)->staffType(ticks[span]);
            ASSERT_TRUE(type && type->isJiMS());
            originalStates[i][span] = type->jimsStateJson();
            originalExtents[i][span] = extentXml(originalStates[i][span]);
            EXPECT_DOUBLE_EQ(generatorCents(type), 700.0);
        }
    }
    const size_t undoBefore = score->undoStack()->currentIndex();

    jims::TuningController controller(score, 3);
    ASSERT_TRUE(controller.beginPreview());
    ASSERT_TRUE(controller.preview(690.0));
    for (staff_idx_t i = 0; i < 4; ++i) {
        const Fraction ticks[2] = { Fraction(0, 1), second->tick() };
        for (size_t span = 0; span < 2; ++span) {
            const StaffType* type = score->staff(i)->staffType(ticks[span]);
            EXPECT_DOUBLE_EQ(generatorCents(type), 690.0) << "preview missed staff " << i << " span " << span;
            EXPECT_EQ(extentXml(type->jimsStateJson()), originalExtents[i][span])
                << "preview changed staff " << i << " extent span " << span;
        }
    }
    EXPECT_EQ(score->undoStack()->currentIndex(), undoBefore);

    controller.cancel();
    for (staff_idx_t i = 0; i < 4; ++i) {
        EXPECT_EQ(score->staff(i)->staffType(Fraction(0, 1))->jimsStateJson(), originalStates[i][0]);
        EXPECT_EQ(score->staff(i)->staffType(second->tick())->jimsStateJson(), originalStates[i][1]);
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
            EXPECT_EQ(extentXml(type->jimsStateJson()), originalExtents[i][span])
                << "commit changed staff " << i << " extent span " << span;
        }
    }
    score->undoRedo(true, nullptr);
    for (staff_idx_t i = 0; i < 4; ++i) {
        EXPECT_EQ(score->staff(i)->staffType(Fraction(0, 1))->jimsStateJson(), originalStates[i][0]);
        EXPECT_EQ(score->staff(i)->staffType(second->tick())->jimsStateJson(), originalStates[i][1]);
    }
    score->undoRedo(false, nullptr);
    for (staff_idx_t i = 0; i < 4; ++i) {
        EXPECT_DOUBLE_EQ(generatorCents(score->staff(i)->staffType(Fraction(0, 1))), 690.0);
        EXPECT_DOUBLE_EQ(generatorCents(score->staff(i)->staffType(second->tick())), 690.0);
    }
    delete score;
}

TEST(Engraving_JiMStaffM10SATBTests, tuningIndicatorAppearsOnlyOnTheTopVisibleJimsStaff)
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

TEST(Engraving_JiMStaffM10SATBTests, referenceChangesPreserveWrittenAndEmptyStaffGeometry)
{
    const String paths[] = { satbTemplatePath(), ScoreRW::rootPath() + u"/jimstaff_data/m9-satb-mixed.mscx" };
    for (const String& path : paths) {
        for (double generator : { 686.0, 696.0, 720.0 }) {
            MasterScore* score = ScoreRW::readScore(path, true);
            ASSERT_TRUE(score);
            jims::TuningController controller(score, 0);
            ASSERT_TRUE(controller.beginPreview());
            ASSERT_TRUE(controller.commit(generator));
            std::vector<StaffType::MeloFrameView> before;
            std::vector<String> extents;
            for (staff_idx_t i = 0; i < score->nstaves(); ++i) {
                const StaffType* type = score->staff(i)->staffType(Fraction(0, 1));
                before.push_back(type->jimsWholeFrameView(score, i));
                extents.push_back(extentXml(type->jimsStateJson()));
            }
            const auto writtenNotes = notesOn(score, 0);
            std::vector<std::pair<int, int> > identities;
            for (const Note* note : writtenNotes) {
                identities.push_back({ note->jimsNPer(), note->jimsNGen() });
            }
            for (const String& key : { String(u"key:0:2"), String(u"key:-1:3"), String(u"key:1:-7") }) {
                String error;
                ASSERT_TRUE(jims::applyChangeToAllJimsParts(score, score->firstMeasure(), { key }, error))
                    << error.toStdString();
                score->doLayout();
                for (staff_idx_t i = 0; i < score->nstaves(); ++i) {
                    const StaffType* type = score->staff(i)->staffType(Fraction(0, 1));
                    EXPECT_EQ(extentXml(type->jimsStateJson()), extents[i]);
                    const auto& after = type->jimsWholeFrameView(score, i);
                    EXPECT_NEAR(after.bottomCents(), before[i].bottomCents(), 1e-6);
                    EXPECT_NEAR(after.topCents(), before[i].topCents(), 1e-6);
                }
                for (size_t i = 0; i < writtenNotes.size(); ++i) {
                    EXPECT_EQ(writtenNotes[i]->jimsNPer(), identities[i].first);
                    EXPECT_EQ(writtenNotes[i]->jimsNGen(), identities[i].second);
                }
                score->undoRedo(true, nullptr);
                score->doLayout();
            }
            delete score;
        }
    }
}

TEST(Engraving_JiMStaffM10SATBTests, firstNoteReplacesEmptyCentreAndUndoRestoresIt)
{
    MasterScore* score = ScoreRW::readScore(satbTemplatePath(), true);
    ASSERT_TRUE(score);
    StaffType* type = score->staff(0)->staffType(Fraction(0, 1));
    ASSERT_TRUE(type->jimsExtentIsEmptyDefault());
    const String before = type->jimsStateJson();
    String expected;
    ASSERT_TRUE(jims::fitExtent(before, u"{\"notes\":[{\"nPer\":0,\"nGen\":0}]}", expected));
    InputState& input = score->inputState();
    input.setTrack(0);
    input.setSegment(score->tick2segment(Fraction(0, 1), false, SegmentType::ChordRest));
    input.setDuration(DurationType::V_QUARTER);
    input.setNoteEntryMode(true);
    score->startCmd(TranslatableString::untranslatable("First JiMS note"));
    score->cmdAddPitch(5 * 7 + 1, false, false); // Host letter input for Re0's default D4.
    score->endCmd();
    score->doLayout();
    ASSERT_EQ(notesOn(score, 0).size(), 1u);
    EXPECT_EQ(extentXml(type->jimsStateJson()), extentXml(expected));
    EXPECT_FALSE(type->jimsExtentIsEmptyDefault());
    const auto& view = type->jimsWholeFrameView(score, 0);
    EXPECT_NEAR(view.bottomCents(), -type->jimsPeriodCents() / 4.0, 1e-6);
    EXPECT_NEAR(view.topCents(), type->jimsPeriodCents() / 4.0, 1e-6);
    score->undoRedo(true, nullptr);
    EXPECT_EQ(type->jimsStateJson(), before);
    EXPECT_TRUE(type->jimsExtentIsEmptyDefault());
    EXPECT_TRUE(notesOn(score, 0).empty());
    score->undoRedo(false, nullptr);
    EXPECT_EQ(extentXml(type->jimsStateJson()), extentXml(expected));
    EXPECT_FALSE(type->jimsExtentIsEmptyDefault());
    delete score;
}

TEST(Engraving_JiMStaffM10SATBTests, everyEmptyVocalStaffUsesItsKernelRangeCentre)
{
    MasterScore* score = ScoreRW::readScore(satbTemplatePath(), true);
    ASSERT_TRUE(score);
    ASSERT_EQ(score->nstaves(), 4u);
    EXPECT_EQ(jims::reconcileExtents(score), 0) << "native load must already reconcile every empty vocal extent";
    const char* roles[4] = { "soprano", "alto", "tenor", "bass" };
    const double expectedDoOrigins[4] = { 300.0, 800.0, 100.0, 1000.0 };
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

// A later empty section must not acquire the written range of an earlier one.
TEST(Engraving_JiMStaffM10SATBTests, eachStaffTypeSpanCollectsOnlyItsOwnNotes)
{
    MasterScore* score = ScoreRW::readScore(u"jimstaff_data/m9-satb-mixed.mscx");
    ASSERT_TRUE(score);
    Staff* staff = score->staff(0);
    Measure* second = score->firstMeasure()->nextMeasure();
    ASSERT_TRUE(second);
    const std::vector<Note*> notes = notesOn(score, 0);
    ASSERT_FALSE(notes.empty());
    score->startCmd(TranslatableString::untranslatable("Create an empty JiMS section"));
    std::vector<Chord*> later;
    for (Note* note : notes) {
        if (note->tick() >= second->tick()) {
            if (std::find(later.begin(), later.end(), note->chord()) == later.end()) {
                later.push_back(note->chord());
            }
        } else {
            note->setJimsPitch(-3, 0);
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
    jims::reconcileExtents(score);
    score->setLayoutAll();
    score->doLayout();
    const StaffType* empty = staff->staffType(second->tick());
    ASSERT_NE(empty, staff->staffType(Fraction(0, 1)));
    ASSERT_TRUE(empty->jimsExtentIsEmptyDefault());
    for (bool elide : { false, true }) {
        score->style().set(Sid::jimsElideEmptyOctaves, elide);
        score->style().set(Sid::jimsShowAllOctavesInFirstSystem, false);
        score->setLayoutAll();
        score->doLayout();
        const auto& view = empty->jimsFrameView(score, 0, second->system());
        ASSERT_EQ(view.bands.size(), 1u);
        EXPECT_NEAR(view.bottomCents(), -empty->jimsPeriodCents() / 4.0, 1e-6);
        EXPECT_NEAR(view.topCents(), empty->jimsPeriodCents() / 4.0, 1e-6);
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
    const muse::String outputDir = forkRoot() + u"/build.m10";
    ASSERT_TRUE(muse::io::Dir::mkpath(outputDir));
    const muse::String saved = outputDir + u"/m10-extent-lifecycle.mscx";
    ASSERT_TRUE(ScoreRW::saveScore(score, saved));
    EXPECT_TRUE(type->jimsStateJson() == widened) << "save must not contract";
    delete score;

    MasterScore* reloaded = ScoreRW::readScore(saved, true);
    ASSERT_TRUE(reloaded);
    EXPECT_TRUE(reloaded->staff(0)->staffType(Fraction(0, 1))->jimsStateJson() == expectedFit)
        << "reload is the sole contraction transition";
    delete reloaded;
}

TEST(Engraving_JiMStaffM10SATBTests, ratioLineExtentSurvivesWhileLatticeCoverageReconciles)
{
    MasterScore* score = ScoreRW::readScore(u"jimstaff_data/m9-satb-mixed.mscx");
    ASSERT_TRUE(score);
    StaffType* type = score->staff(0)->staffType(Fraction(0, 1));
    ASSERT_TRUE(type && type->isJiMS());
    const std::vector<Note*> notes = notesOn(score, 0);
    ASSERT_FALSE(notes.empty());

    String declared;
    ASSERT_TRUE(jims::widenExtent(type->jimsStateJson(), -10, notes.front()->jimsNGen(), declared));
    ASSERT_NE(declared, type->jimsStateJson());
    type->setJimsStateJson(declared);
    const String ratioExtent
        = u"{\"lower\":{\"period\":-1,\"ratio\":\"3/2\"},"
          u"\"upper\":{\"period\":0,\"ratio\":\"1/1\"}}";
    type->setJimsRatioLineExtentJson(ratioExtent);

    EXPECT_EQ(jims::reconcileExtents(score), 1);
    EXPECT_NE(type->jimsStateJson(), declared)
        << "lattice extent remains note-coverage data even with a fixed display extent";
    EXPECT_EQ(type->jimsRatioLineExtentJson(), ratioExtent)
        << "reconciling note coverage must not alter the fixed ratio-line display extent";
    delete score;
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
    // The fixture already carries the singleton soprano's bounded token. Seed
    // a different valid Kernel state so the positive-change assertion below
    // tests a real transition rather than an idempotent derivation.
    score->setJimsMelodyPart(jims::MelodyPart::Tenor);
    ASSERT_GT(jims::deriveTonicAmbits(score), 0);
    for (staff_idx_t i = 0; i < 4; ++i) {
        ASSERT_TRUE(score->staff(i)->staffType(Fraction(0, 1))->jimsTonicAmbit() == tenorToken);
    }
    score->setJimsMelodyPart(jims::MelodyPart::Soprano);
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

    const muse::String outputDir = forkRoot() + u"/build.m10";
    ASSERT_TRUE(muse::io::Dir::mkpath(outputDir));
    const muse::String saved = outputDir + u"/m10-melody-native.mscx";
    ASSERT_TRUE(ScoreRW::saveScore(score, saved));
    delete score;
    MasterScore* reloaded = ScoreRW::readScore(saved, true);
    ASSERT_TRUE(reloaded);
    EXPECT_EQ(reloaded->jimsMelodyPart(), jims::MelodyPart::Tenor);
    delete reloaded;
}
