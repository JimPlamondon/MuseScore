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

// Native JiMS MusicXML import (owner decision 1a, 2026-08-16): the fork's own
// importer reads jims:staff-state (urn:jims:musicxml:1 through 4), jims:pitch,
// and V4 opaque jims:chord-name carriers
// and builds the JiMStaff score directly — the same DOM the fixture converter
// tools/jims/enriched_to_jims_mscx.py produces. jims:change is never read;
// the Kernel validate op gates every state; an unrecognised JiMS namespace
// version is a fatal import error.

#include <gtest/gtest.h>

#include <algorithm>
#include <climits>
#include <functional>

#include "engraving/dom/masterscore.h"
#include "engraving/dom/fret.h"
#include "engraving/dom/harmony.h"
#include "engraving/dom/measure.h"
#include "engraving/dom/note.h"
#include "engraving/dom/part.h"
#include "engraving/dom/segment.h"
#include "engraving/dom/staff.h"
#include "engraving/dom/stafflines.h"
#include "engraving/dom/stafftype.h"
#include "engraving/dom/system.h"
#include "engraving/dom/stafftypechange.h"
#include "engraving/editing/transpose.h"
#include "engraving/jims/jimschange.h"
#include "engraving/jims/jimschangecontroller.h"
#include "engraving/style/style.h"

#include "importexport/musicxml/internal/import/importmusicxml.h"
#include "importexport/musicxml/internal/export/exportmusicxml.h"
#include "importexport/musicxml/imusicxmlconfiguration.h"
#include "engraving/jims/jimsbridge.h"
#include "engraving/rw/xmlwriter.h"
#include "engraving/rw/mscsaver.h"
#include "engraving/infrastructure/mscwriter.h"
#include "engraving/jims/jimsinterchange.h"
#include "io/buffer.h"
#include "io/file.h"
#include "io/fileinfo.h"
#include "io/dir.h"
#include "engraving/tests/utils/scorerw.h"
#include "draw/bufferedpaintprovider.h"
#include "draw/painter.h"

using namespace mu;
using namespace mu::engraving;
using namespace mu::engraving::rendering;
using namespace mu::iex::musicxml;
using namespace muse::draw;

static const String JIMS_DATA_DIR(u"data/jims/");

namespace {
String exportToScratch(MasterScore* score, const char* name);
String readAll(const String& path);
}

class MusicXml_JiMS_Tests : public ::testing::Test
{
public:
    MasterScore* readJims(const char* file)
    {
        auto importXml = [](MasterScore* score, const muse::io::path_t& path) -> engraving::Err {
            return importMusicXml(score, path.toQString(), false);
        };
        return ScoreRW::readScore(JIMS_DATA_DIR + String::fromUtf8(file), false, importXml);
    }

    static const StaffType* staffTypeAtStart(Score* score, staff_idx_t staffIdx = 0)
    {
        return score->staff(staffIdx)->staffType(Fraction(0, 1));
    }

    static Measure* measureNo(Score* score, int n)
    {
        Measure* m = score->firstMeasure();
        for (int i = 1; m && i < n; ++i) {
            m = m->nextMeasure();
        }
        return m;
    }

    static std::vector<Harmony*> harmoniesInOrder(Score* score)
    {
        std::vector<Harmony*> result;
        for (Segment* segment = score->firstSegment(SegmentType::ChordRest); segment;
             segment = segment->next1(SegmentType::ChordRest)) {
            for (EngravingItem* item : segment->annotations()) {
                if (item && item->isHarmony()) {
                    result.push_back(toHarmony(item));
                }
            }
        }
        return result;
    }

    static std::vector<Harmony*> harmoniesOnStaff(Score* score, staff_idx_t staffIdx)
    {
        std::vector<Harmony*> result;
        for (Harmony* harmony : harmoniesInOrder(score)) {
            if (harmony->staffIdx() == staffIdx) {
                result.push_back(harmony);
            }
        }
        return result;
    }

    static std::vector<const Note*> notesInOrder(Score* score, staff_idx_t staffIdx = 0)
    {
        std::vector<const Note*> out;
        for (Measure* m = score->firstMeasure(); m; m = m->nextMeasure()) {
            for (Segment* s = m->first(SegmentType::ChordRest); s; s = s->next(SegmentType::ChordRest)) {
                for (voice_idx_t v = 0; v < VOICES; ++v) {
                    EngravingItem* e = s->element(staffIdx * VOICES + v);
                    if (e && e->isChord()) {
                        for (const Note* n : toChord(e)->notes()) {
                            out.push_back(n);
                        }
                    }
                }
            }
        }
        return out;
    }
};

// The exact converter output for the accepted m5-key-mode piece
// (jims-evidence/m5-acceptance/m5-key-mode/m5-key-mode.mscx): byte-shape,
// key order, no spaces, tonic_ambit last.
static const char* KEY_MODE_STATE_1
    = "{\"scale\":[\"M2\",\"m2\",\"M2\",\"M2\",\"M2\",\"m2\",\"M2\"],\"collection_rotation\":0,\"mode_rotation\":0,"
      "\"generator_cents\":700.0,\"period_cents\":1200.0,\"embedding\":{\"large_steps\":5,\"small_steps\":2},"
      "\"extent\":{\"lower\":{\"nPer\":1,\"nGen\":-2},\"upper\":{\"nPer\":2,\"nGen\":-2}},\"reference\":{\"reference-pitch\":{\"key_number\":62}},"
      "\"tonic_ambit\":\"tonic-bounded\"}";
static const char* KEY_MODE_STATE_2
    = "{\"scale\":[\"M2\",\"m2\",\"M2\",\"M2\",\"M2\",\"m2\",\"M2\"],\"collection_rotation\":0,\"mode_rotation\":5,"
      "\"generator_cents\":700.0,\"period_cents\":1200.0,\"embedding\":{\"large_steps\":5,\"small_steps\":2},"
      "\"extent\":{\"lower\":{\"nPer\":1,\"nGen\":-2},\"upper\":{\"nPer\":2,\"nGen\":-2}},\"reference\":{\"reference-pitch\":{\"key_number\":53}},"
      "\"tonic_ambit\":\"tonic-bounded\"}";

static String sharedState(const String& state)
{
    String shared;
    String error;
    if (!jims::musicxmlSharedStateV3Xml(state, shared, &error)) {
        return String();
    }
    return shared;
}

TEST_F(MusicXml_JiMS_Tests, v3ImportBuildsTheJiMStaffLikeTheConverter)
{
    MasterScore* score = readJims("jims-v3-m5-key-mode.musicxml");
    ASSERT_TRUE(score);
    ASSERT_EQ(score->nstaves(), 1u);
    const StaffType* st = staffTypeAtStart(score);
    ASSERT_TRUE(st);
    EXPECT_TRUE(st->isJiMS());
    EXPECT_EQ(st->xmlName(), String(u"jims12tet"));
    EXPECT_EQ(st->lines(), 13);
    // Load-time reconciliation replaces the serialized extent with the exact
    // written-note bounds. All song-wide content remains the converter's.
    EXPECT_EQ(sharedState(st->jimsStateJson()), sharedState(String::fromUtf8(KEY_MODE_STATE_1)));
    EXPECT_TRUE(st->jimsJiLines());
    EXPECT_EQ(st->jimsTonicAmbit(), u"tonic-bounded");
    // The change measure carries the complete second state (never derived from jims:change).
    Measure* m2 = measureNo(score, 2);
    ASSERT_TRUE(m2);
    const StaffTypeChange* stc = jims::changeCarrier(m2, 0);
    ASSERT_TRUE(stc);
    ASSERT_TRUE(stc->staffType());
    EXPECT_EQ(sharedState(stc->staffType()->jimsStateJson()), sharedState(String::fromUtf8(KEY_MODE_STATE_2)));
    EXPECT_TRUE(stc->staffType()->jimsStateJson().contains(u"\"tonic_ambit\":\"tonic-"))
        << stc->staffType()->jimsStateJson().toStdString();
    EXPECT_FALSE(jims::changeCarrier(measureNo(score, 1), 0));
    EXPECT_FALSE(jims::changeCarrier(measureNo(score, 3), 0));
    // Every pitched note carries its Kernel identity from jims:pitch.
    auto notes = notesInOrder(score);
    ASSERT_EQ(notes.size(), 12u);
    for (const Note* n : notes) {
        EXPECT_TRUE(n->hasJimsPitch());
    }
    EXPECT_EQ(notes[0]->jimsNPer(), -4);
    EXPECT_EQ(notes[0]->jimsNGen(), 7);
    EXPECT_EQ(notes[1]->jimsNPer(), 3);
    EXPECT_EQ(notes[1]->jimsNGen(), -5);
    // Engraving font seam (Milestone 3).
    EXPECT_EQ(score->style().value(Sid::musicalSymbolFont).value<String>(), String(u"JiMSMusic"));
    EXPECT_FALSE(score->style().value(Sid::hideInstrumentNameIfOneInstrument).toBool());
    delete score;
}

TEST_F(MusicXml_JiMS_Tests, midBarStateChangeImportsAndExportsAtItsExactTick)
{
    MasterScore* score = readJims("jims-mid-bar-state-change.musicxml");
    ASSERT_TRUE(score);
    const std::vector<const Note*> notes = notesInOrder(score);
    ASSERT_EQ(notes.size(), 4u);
    const Fraction changeTick = notes[2]->tick();
    Measure* measure = measureNo(score, 1);
    ASSERT_TRUE(measure);
    ASSERT_GT(changeTick, measure->tick());
    ASSERT_LT(changeTick, measure->endTick());
    const StaffTypeChange* carrier = jims::changeCarrierAt(measure, 0, changeTick);
    ASSERT_TRUE(carrier);
    EXPECT_EQ(carrier->rtick(), changeTick - measure->tick());
    const StaffType* oldStaffType = notes[0]->staff()->staffTypeForElement(notes[0]);
    const StaffType* newStaffType = notes[2]->staff()->staffTypeForElement(notes[2]);
    ASSERT_TRUE(oldStaffType);
    ASSERT_TRUE(newStaffType);
    EXPECT_TRUE(oldStaffType->jimsStateJson().contains(u"\"mode_rotation\":0"));
    EXPECT_TRUE(notes[1]->staff()->staffTypeForElement(notes[1])->jimsStateJson().contains(u"\"mode_rotation\":0"));
    EXPECT_TRUE(newStaffType->jimsStateJson().contains(u"\"mode_rotation\":5"));
    EXPECT_TRUE(notes[3]->staff()->staffTypeForElement(notes[3])->jimsStateJson().contains(u"\"mode_rotation\":5"));
    EXPECT_TRUE(oldStaffType->jimsStateJson().contains(u"\"reference\":\"none\""));
    EXPECT_TRUE(newStaffType->jimsStateJson().contains(u"\"reference\":\"none\""));

    jims::ChangeIndicator indicator;
    ASSERT_TRUE(jims::midBarChangeIndicator(carrier, indicator));
    EXPECT_EQ(indicator.kinds, std::vector<String>({ u"mode" }));
    EXPECT_TRUE(indicator.dotStacks.empty());
    ASSERT_EQ(indicator.tonicIndicators.size(), 2u);
    EXPECT_EQ(indicator.tonicIndicators[0].label, u"Do");
    EXPECT_EQ(indicator.tonicIndicators[1].label, u"La");
    ASSERT_EQ(indicator.arrows.size(), 1u);
    EXPECT_EQ(indicator.arrows[0].kind, u"mode");
    EXPECT_TRUE(indicator.arrows[0].trumps.isEmpty());

    const String out = exportToScratch(score, "jims-mid-bar-state-change-roundtrip.musicxml");
    const String xml = readAll(out);
    EXPECT_EQ(xml.count(u"<jims:staff-state"), 2u);
    const size_t firstNote = xml.indexOf(u"<note");
    const size_t secondNote = xml.indexOf(u"<note", firstNote + 1);
    const size_t thirdNote = xml.indexOf(u"<note", secondNote + 1);
    const size_t firstState = xml.indexOf(u"<jims:staff-state");
    const size_t secondState = xml.indexOf(u"<jims:staff-state", firstState + 1);
    ASSERT_NE(secondState, muse::nidx);
    EXPECT_GT(secondState, secondNote);
    EXPECT_LT(secondState, thirdNote);

    auto importXml = [](MasterScore* target, const muse::io::path_t& path) -> engraving::Err {
        return importMusicXml(target, path.toQString(), false);
    };
    MasterScore* again = ScoreRW::readScore(out, true, importXml);
    ASSERT_TRUE(again);
    Measure* againMeasure = measureNo(again, 1);
    ASSERT_TRUE(againMeasure);
    EXPECT_TRUE(jims::changeCarrierAt(againMeasure, 0, changeTick));
    EXPECT_EQ(notesInOrder(again).size(), 4u);
    delete again;
    delete score;
}

TEST_F(MusicXml_JiMS_Tests, explicitTonicAmbitsSurviveNativeScoreReload)
{
    MasterScore* score = readJims("jims-mid-bar-state-change.musicxml");
    ASSERT_TRUE(score);
    StaffType* base = score->staff(0)->staffType(Fraction(0, 1));
    ASSERT_TRUE(base);
    String baseState = base->jimsStateJson();
    baseState.replace(u"\"tonic_ambit\":\"tonic-bounded\"", u"\"tonic_ambit\":\"tonic-centered\"");
    base->setJimsStateJson(baseState);
    ASSERT_EQ(base->jimsTonicAmbit(), u"tonic-centered");

    const String dir(u"jims-export-scratch");
    muse::io::Dir::mkpath(dir);
    const String mscz = dir + u"/explicit-tonic-ambit-roundtrip.mscz";
    muse::io::File::remove(mscz);
    {
        muse::io::File file(mscz);
        ASSERT_TRUE(file.open(muse::io::IODevice::WriteOnly));
        MscWriter::Params params;
        params.device = &file;
        params.filePath = mscz;
        params.mode = MscIoMode::Zip;
        MscWriter writer(params);
        ASSERT_TRUE(writer.open());
        MscSaver saver(score->iocContext());
        ASSERT_TRUE(saver.writeMscz(score, writer, false));
        writer.close();
        file.close();
    }
    delete score;

    MasterScore* reloaded = ScoreRW::readScore(mscz, true);
    ASSERT_TRUE(reloaded);
    const StaffType* reloadedBase = reloaded->staff(0)->staffType(Fraction(0, 1));
    ASSERT_TRUE(reloadedBase);
    EXPECT_EQ(reloadedBase->jimsTonicAmbit(), u"tonic-centered");
    const std::vector<const Note*> notes = notesInOrder(reloaded);
    ASSERT_EQ(notes.size(), 4u);
    const StaffTypeChange* change = jims::changeCarrierAt(measureNo(reloaded, 1), 0, notes[2]->tick());
    ASSERT_TRUE(change);
    ASSERT_TRUE(change->staffType());
    EXPECT_EQ(change->staffType()->jimsTonicAmbit(), u"tonic-bounded");
    delete reloaded;
}

TEST_F(MusicXml_JiMS_Tests, midBarIndicatorElementsAlignWithTheirDisplayedStaffNoteLines)
{
    MasterScore* score = readJims("jims-mid-bar-state-change.musicxml");
    ASSERT_TRUE(score);
    score->doLayout();
    Measure* measure = measureNo(score, 1);
    ASSERT_TRUE(measure);
    const std::vector<const Note*> notes = notesInOrder(score);
    ASSERT_EQ(notes.size(), 4u);
    const Fraction changeTick = notes[2]->tick();
    const StaffTypeChange* carrier = jims::changeCarrierAt(measure, 0, changeTick);
    ASSERT_TRUE(carrier);
    jims::ChangeIndicator indicator;
    const StaffType* changedStaffType = nullptr;
    ASSERT_TRUE(jims::midBarChangeIndicator(carrier, indicator, &changedStaffType));
    ASSERT_TRUE(changedStaffType);

    const StaffLines* lines = measure->staffLines(0);
    ASSERT_TRUE(lines);
    size_t doLineCount = 0;
    for (const StaffLines::JimsGuideLine& guide : lines->jimsGuideLines()) {
        if (!guide.dashed && guide.colorStyle == Sid::jimsDoLineColor) {
            ++doLineCount;
        }
    }
    EXPECT_EQ(doLineCount, 2u) << "one-period tonic-bounded JiMStaff must be Do-to-Do";
    const StaffType* displayedStaffType = score->staff(0)->staffType(measure->tick());
    ASSERT_TRUE(displayedStaffType);
    ASSERT_TRUE(displayedStaffType->isJiMS());
    ASSERT_NE(changedStaffType, displayedStaffType);
    const StaffType::JimsFrameView& wholeView = displayedStaffType->jimsFrameView(score, 0, nullptr);
    ASSERT_FALSE(wholeView.empty());
    EXPECT_NEAR(wholeView.bottomCents(), 0.0, 1e-6);
    EXPECT_NEAR(wholeView.topCents(), 1200.0, 1e-6);
    const StaffType::JimsFrameView& view
        = displayedStaffType->jimsFrameView(score, 0, measure->system());
    ASSERT_FALSE(view.empty());
    EXPECT_NEAR(view.bottomCents(), 0.0, 1e-6);
    EXPECT_NEAR(view.topCents(), 1200.0, 1e-6);
    ASSERT_FALSE(view.bands.front().segments.empty());
    EXPECT_NEAR(view.bands.front().segments.front().lowerCents, 0.0, 1e-6);
    EXPECT_NEAR(view.bands.back().segments.back().upperCents, 1200.0, 1e-6);
    jims::PeriodicOrigins origins;
    ASSERT_TRUE(jims::periodicOrigins(displayedStaffType->jimsStateJson(), origins));
    EXPECT_NEAR(origins.doCentsAboveExtentLower, 0.0, 1e-6);
    const double periodCents = displayedStaffType->jimsPeriodCents();
    ASSERT_GT(periodCents, 0.0);
    const double basePeriod = jims::changeAnchorPeriodCents(
        view, indicator, periodCents, origins.doCentsAboveExtentLower);
    std::vector<double> expectedTonicYs;
    for (const jims::ChangePoint& point : indicator.tonicIndicators) {
        const double cents = basePeriod + (point.periodOffset + point.ordinate) * periodCents;
        expectedTonicYs.push_back(lines->pos().y()
                                  + displayedStaffType->jimsYFromCents(cents, view) * lines->spatium());
    }
    std::sort(expectedTonicYs.begin(), expectedTonicYs.end());

    std::shared_ptr<BufferedPaintProvider> provider = std::make_shared<BufferedPaintProvider>();
    Painter painter(provider, "midbar-musicxml-lines");
    painter.setViewport(RectF(0, 0, 4000, 4000));
    PaintOptions options;
    lines->renderer()->drawItem(lines, &painter, options);
    painter.endDraw();

    const DrawDataPtr drawData = provider->drawData();
    const Color grey(128, 128, 128);
    std::vector<double> flankXs;
    std::vector<RectF> pathBounds;
    std::function<void(const DrawData::Item&)> walk = [&](const DrawData::Item& item) {
        for (const DrawData::Data& data : item.datas) {
            const DrawData::State& state = drawData->states.at(data.state);
            for (const DrawPolygon& polygon : data.polygons) {
                if (polygon.mode == PolygonMode::Polyline && polygon.polygon.size() == 2
                    && std::abs(polygon.polygon[0].x() - polygon.polygon[1].x()) < 1e-6
                    && state.pen.style() == PenStyle::DashLine && state.pen.color() == grey) {
                    flankXs.push_back(polygon.polygon[0].x());
                }
            }
            for (const DrawPath& path : data.paths) {
                pathBounds.push_back(path.path.boundingRect());
            }
        }
        for (const DrawData::Item& child : item.chilren) {
            walk(child);
        }
    };
    walk(drawData->item);
    ASSERT_EQ(flankXs.size(), 2u);
    std::sort(flankXs.begin(), flankXs.end());
    const Segment* anchor = measure->findSegmentR(Segment::CHORD_REST_OR_TIME_TICK_TYPE, carrier->rtick());
    ASSERT_TRUE(anchor);
    const double expectedNoteGap = score->style().styleMM(Sid::barNoteDistance);
    EXPECT_GE(anchor->x() - flankXs.back(), expectedNoteGap - 1e-6)
        << "the note after a mid-bar indicator must clear its right dashed flank";
    std::vector<double> paintedTonicYs;
    for (const RectF& bounds : pathBounds) {
        if (bounds.center().x() > flankXs.front() && bounds.center().x() < flankXs.back()
            && std::abs(bounds.width() - bounds.height()) < 1e-6) {
            paintedTonicYs.push_back(bounds.center().y());
        }
    }
    std::sort(paintedTonicYs.begin(), paintedTonicYs.end());
    ASSERT_EQ(paintedTonicYs.size(), expectedTonicYs.size());
    for (size_t i = 0; i < expectedTonicYs.size(); ++i) {
        EXPECT_NEAR(paintedTonicYs[i], expectedTonicYs[i], 1e-6)
            << "mid-bar tonic indicator is not aligned with its displayed staff note-line";
    }

    delete score;
}

TEST_F(MusicXml_JiMS_Tests, everyReferenceFormTranscribesVerbatimAndOlderProfilesReadAsNone)
{
    struct Case {
        const char* file;
        const char* reference;
    };
    const Case cases[] = {
        { "jims-reference-none.musicxml", "\"reference\":\"none\"" },
        { "jims-reference-none-explicit.musicxml", "\"reference\":\"none\"" },
        { "jims-reference-pitch.musicxml", "\"reference\":{\"reference-pitch\":{\"key_number\":62}}" },
        { "jims-reference-pitch-class.musicxml", "\"reference\":{\"pitch-class\":{\"pitch_class\":2}}" },
        { "jims-reference-hertz.musicxml", "\"reference\":{\"frequency-hz\":{\"hertz\":293.665}}" },
        { "jims-v1-collision.musicxml", "\"reference\":\"none\"" },
        { "jims-v2-mode-change.musicxml", "\"reference\":\"none\"" },
    };
    for (const Case& c : cases) {
        MasterScore* score = readJims(c.file);
        ASSERT_TRUE(score) << c.file;
        const StaffType* st = staffTypeAtStart(score);
        ASSERT_TRUE(st && st->isJiMS()) << c.file;
        EXPECT_TRUE(st->jimsStateJson().contains(String::fromUtf8(c.reference))) << c.file << " " << st->jimsStateJson().toStdString();
        EXPECT_FALSE(st->jimsStateJson().contains(u"\"tonic_ambit\":\"\"")) << c.file;
        delete score;
    }
    // A genuine V2 document with two states and no reference: the second
    // state (mode-rotation 5) rides a StaffTypeChange at measure 2.
    MasterScore* v2 = readJims("jims-v2-mode-change.musicxml");
    ASSERT_TRUE(v2);
    const StaffTypeChange* stc = jims::changeCarrier(measureNo(v2, 2), 0);
    ASSERT_TRUE(stc);
    EXPECT_TRUE(stc->staffType()->jimsStateJson().contains(u"\"mode_rotation\":5"));
    delete v2;
    // V1 (no tonic-ambit in the profile): the field is simply absent.
    MasterScore* v1 = readJims("jims-v1-collision.musicxml");
    ASSERT_TRUE(v1);
    EXPECT_FALSE(staffTypeAtStart(v1)->jimsStateJson().contains(u"tonic_ambit"));
    EXPECT_EQ(notesInOrder(v1).size(), 12u);
    delete v1;
}

TEST_F(MusicXml_JiMS_Tests, namespaceIsResolvedByUriNotByPrefix)
{
    MasterScore* a = readJims("jims-v3-m5-mode.musicxml");
    MasterScore* b = readJims("jims-v3-other-prefix.musicxml");
    ASSERT_TRUE(a && b);
    EXPECT_EQ(staffTypeAtStart(a)->jimsStateJson(), staffTypeAtStart(b)->jimsStateJson());
    ASSERT_TRUE(jims::changeCarrier(measureNo(b, 2), 0));
    EXPECT_EQ(jims::changeCarrier(measureNo(a, 2), 0)->staffType()->jimsStateJson(),
              jims::changeCarrier(measureNo(b, 2), 0)->staffType()->jimsStateJson());
    EXPECT_EQ(notesInOrder(b).size(), 12u);
    EXPECT_TRUE(notesInOrder(b)[0]->hasJimsPitch());
    delete a;
    delete b;
}

TEST_F(MusicXml_JiMS_Tests, unknownJimsNamespaceVersionIsAFatalImportError)
{
    // A document that declares itself JiMS with a version this fork does not
    // know must not silently import as a plain five-line staff.
    MasterScore* score = readJims("jims-unknown-version-invalid.musicxml");
    EXPECT_FALSE(score);
    delete score;
}

TEST_F(MusicXml_JiMS_Tests, ChordNameV4ImportsAsOpaquePerObjectHarmonyBesideStandardHarmony)
{
    MasterScore* score = readJims("jims-chord-name-v4.musicxml");
    ASSERT_TRUE(score);
    ASSERT_EQ(score->nstaves(), 2u);
    ASSERT_TRUE(staffTypeAtStart(score, 0));
    ASSERT_TRUE(staffTypeAtStart(score, 1));
    EXPECT_TRUE(staffTypeAtStart(score, 0)->isJiMS());
    EXPECT_FALSE(staffTypeAtStart(score, 1)->isJiMS());
    const std::vector<const Note*> jimsNotes = notesInOrder(score, 0);
    const std::vector<const Note*> stockNotes = notesInOrder(score, 1);
    ASSERT_EQ(jimsNotes.size(), 2u);
    ASSERT_EQ(stockNotes.size(), 2u);
    EXPECT_TRUE(jimsNotes[0]->hasJimsPitch());
    EXPECT_TRUE(jimsNotes[1]->hasJimsPitch());
    EXPECT_EQ(jimsNotes[0]->jimsNPer(), 1);
    EXPECT_EQ(jimsNotes[0]->jimsNGen(), -2);
    EXPECT_EQ(jimsNotes[1]->jimsNPer(), 2);
    EXPECT_EQ(jimsNotes[1]->jimsNGen(), -2);
    EXPECT_TRUE(staffTypeAtStart(score, 0)->jimsStateJson().contains(
                    u"\"extent\":{\"lower\":{\"nPer\":1,\"nGen\":-2},\"upper\":{\"nPer\":2,\"nGen\":-2}}"));
    EXPECT_FALSE(stockNotes[0]->hasJimsPitch());
    EXPECT_FALSE(stockNotes[1]->hasJimsPitch());
    score->doLayout();
    const std::vector<Harmony*> harmonies = harmoniesInOrder(score);
    const std::vector<Harmony*> jimsHarmonies = harmoniesOnStaff(score, 0);
    const std::vector<Harmony*> stockHarmonies = harmoniesOnStaff(score, 1);
    ASSERT_EQ(harmonies.size(), 4u);
    ASSERT_EQ(jimsHarmonies.size(), 2u);
    ASSERT_EQ(stockHarmonies.size(), 2u);
    EXPECT_EQ(jimsHarmonies[0]->harmonyType(), HarmonyType::JIMS);
    EXPECT_EQ(jimsHarmonies[0]->harmonyName(), u"!So7/Ti");
    EXPECT_EQ(jimsHarmonies[0]->tick(), Fraction(0, 1));
    EXPECT_EQ(jimsHarmonies[0]->staffIdx(), 0u);
    EXPECT_EQ(jimsHarmonies[0]->placement(), PlacementV::ABOVE);
    EXPECT_FALSE(jimsHarmonies[0]->isPlayable());
    EXPECT_FALSE(jimsHarmonies[0]->isRealizable());
    ASSERT_EQ(jimsHarmonies[0]->chords().size(), 1u);
    EXPECT_EQ(jimsHarmonies[0]->chords().front()->textName(), u"!So7/Ti");
    EXPECT_EQ(jimsHarmonies[0]->chords().front()->rootTpc(), Tpc::TPC_INVALID);
    EXPECT_GT(jimsHarmonies[0]->ldata()->bbox().width(), 0.0);
    EXPECT_EQ(jimsHarmonies[0]->ldata()->renderItemList().size(), 1u);
    EXPECT_EQ(jimsHarmonies[1]->harmonyType(), HarmonyType::JIMS);
    EXPECT_EQ(jimsHarmonies[1]->harmonyName(), u"Re:So7");
    EXPECT_EQ(jimsHarmonies[1]->tick(), Fraction(1, 1));
    EXPECT_EQ(jimsHarmonies[1]->staffIdx(), 0u);
    EXPECT_EQ(jimsHarmonies[1]->placement(), PlacementV::ABOVE);
    EXPECT_GT(jimsHarmonies[1]->ldata()->bbox().width(), 0.0);
    EXPECT_EQ(jimsHarmonies[1]->ldata()->renderItemList().size(), 1u);
    EXPECT_EQ(stockHarmonies[0]->harmonyType(), HarmonyType::STANDARD);
    EXPECT_EQ(stockHarmonies[1]->harmonyType(), HarmonyType::STANDARD);
    EXPECT_TRUE(tpcIsValid(stockHarmonies[0]->rootTpc()));
    EXPECT_TRUE(tpcIsValid(stockHarmonies[1]->rootTpc()));
    delete score;
}

TEST_F(MusicXml_JiMS_Tests, ChordNameEditingKeepsTheWholeOpaqueStringAndRefusesTilde)
{
    MasterScore* score = readJims("jims-chord-name-v4.musicxml");
    ASSERT_TRUE(score);
    const std::vector<Harmony*> harmonies = harmoniesOnStaff(score, 0);
    ASSERT_EQ(harmonies.size(), 2u);
    Harmony* jims = harmonies.front();
    const String names[] = { u"Do5", u"Fa5", u"Do:La7", u"!So7/Ti", u"Do5|Fa5", u"Fi@Te:M3²+La,Ti/Re" };
    for (const String& name : names) {
        jims->setHarmony(name);
        ASSERT_EQ(jims->chords().size(), 1u) << name.toStdString();
        EXPECT_EQ(jims->chords().front()->textName(), name) << name.toStdString();
        EXPECT_EQ(jims->harmonyName(), name) << name.toStdString();
        EXPECT_EQ(jims->chords().front()->rootTpc(), Tpc::TPC_INVALID) << name.toStdString();
    }
    const String before = jims->harmonyName();
    jims->setHarmony(u"~So7/Ti");
    EXPECT_EQ(jims->harmonyName(), before);
    delete score;
}

TEST_F(MusicXml_JiMS_Tests, ChordNameTranspositionLeavesJiMSOpaqueAndTransposesStandardHarmony)
{
    MasterScore* score = readJims("jims-chord-name-v4.musicxml");
    ASSERT_TRUE(score);
    const std::vector<Harmony*> beforeJims = harmoniesOnStaff(score, 0);
    const std::vector<Harmony*> beforeStock = harmoniesOnStaff(score, 1);
    ASSERT_EQ(beforeJims.size(), 2u);
    ASSERT_EQ(beforeStock.size(), 2u);
    const String firstJims = beforeJims[0]->harmonyName();
    const String secondJims = beforeJims[1]->harmonyName();
    const int firstStandardRoot = beforeStock[0]->rootTpc();
    const int secondStandardRoot = beforeStock[1]->rootTpc();

    score->cmdSelectAll();
    score->startCmd(TranslatableString::untranslatable("Test JiMS chord-name transposition"));
    Transpose::transpose(score, TransposeMode::BY_INTERVAL, TransposeDirection::UP, Key::C, 4,
                         true, true, true);
    score->endCmd();

    const std::vector<Harmony*> afterJims = harmoniesOnStaff(score, 0);
    const std::vector<Harmony*> afterStock = harmoniesOnStaff(score, 1);
    ASSERT_EQ(afterJims.size(), 2u);
    ASSERT_EQ(afterStock.size(), 2u);
    EXPECT_EQ(afterJims[0]->harmonyName(), firstJims);
    EXPECT_EQ(afterJims[1]->harmonyName(), secondJims);
    EXPECT_EQ(afterJims[0]->chords().front()->rootTpc(), Tpc::TPC_INVALID);
    EXPECT_EQ(afterJims[1]->chords().front()->rootTpc(), Tpc::TPC_INVALID);
    EXPECT_NE(afterStock[0]->rootTpc(), firstStandardRoot);
    EXPECT_NE(afterStock[1]->rootTpc(), secondStandardRoot);
    delete score;
}

TEST_F(MusicXml_JiMS_Tests, ChordNameV4SurvivesNativeAndMusicXmlRoundTripsExactly)
{
    MasterScore* score = readJims("jims-chord-name-v4.musicxml");
    ASSERT_TRUE(score);
    const String dir(u"jims-export-scratch");
    muse::io::Dir::mkpath(dir);
    MasterScore* native = nullptr;
    for (const String& extension : { String(u"mscx"), String(u"mscz") }) {
        const String nativePath = dir + u"/jims-chord-name-native." + extension;
        if (extension == u"mscz") {
            muse::io::File::remove(nativePath);
            muse::io::File file(nativePath);
            ASSERT_TRUE(file.open(muse::io::IODevice::WriteOnly));
            MscWriter::Params params;
            params.device = &file;
            params.filePath = nativePath;
            params.mode = MscIoMode::Zip;
            MscWriter writer(params);
            ASSERT_TRUE(writer.open());
            MscSaver saver(score->iocContext());
            ASSERT_TRUE(saver.writeMscz(score, writer, false));
            writer.close();
            file.close();
        } else {
            ASSERT_TRUE(ScoreRW::saveScore(score, nativePath));
        }
        MasterScore* loaded = ScoreRW::readScore(nativePath, true);
        ASSERT_TRUE(loaded) << extension.toStdString();
        ASSERT_EQ(loaded->nstaves(), 2u) << extension.toStdString();
        EXPECT_TRUE(staffTypeAtStart(loaded, 0)->isJiMS()) << extension.toStdString();
        EXPECT_FALSE(staffTypeAtStart(loaded, 1)->isJiMS()) << extension.toStdString();
        ASSERT_EQ(harmoniesInOrder(loaded).size(), 4u) << extension.toStdString();
        ASSERT_EQ(harmoniesOnStaff(loaded, 0).size(), 2u) << extension.toStdString();
        ASSERT_EQ(harmoniesOnStaff(loaded, 1).size(), 2u) << extension.toStdString();
        EXPECT_EQ(harmoniesOnStaff(loaded, 0)[0]->harmonyType(), HarmonyType::JIMS) << extension.toStdString();
        EXPECT_EQ(harmoniesOnStaff(loaded, 0)[0]->harmonyName(), u"!So7/Ti") << extension.toStdString();
        if (extension == u"mscz") {
            native = loaded;
        } else {
            delete loaded;
        }
    }
    ASSERT_TRUE(native);

    const String out = exportToScratch(native, "jims-chord-name-roundtrip.musicxml");
    const String xml = readAll(out);
    EXPECT_TRUE(xml.contains(u"xmlns:jims=\"urn:jims:musicxml:4\""));
    EXPECT_EQ(xml.count(u"<jims:chord-name>!So7/Ti</jims:chord-name>"), 1);
    EXPECT_EQ(xml.count(u"<jims:chord-name>Re:So7</jims:chord-name>"), 1);
    auto importXml = [](MasterScore* s, const muse::io::path_t& path) -> engraving::Err {
        return importMusicXml(s, path.toQString(), false);
    };
    MasterScore* again = ScoreRW::readScore(out, true, importXml);
    ASSERT_TRUE(again);
    ASSERT_EQ(again->nstaves(), 2u);
    EXPECT_TRUE(staffTypeAtStart(again, 0)->isJiMS());
    EXPECT_FALSE(staffTypeAtStart(again, 1)->isJiMS());
    const std::vector<Harmony*> againJims = harmoniesOnStaff(again, 0);
    const std::vector<Harmony*> againStock = harmoniesOnStaff(again, 1);
    ASSERT_EQ(harmoniesInOrder(again).size(), 4u);
    ASSERT_EQ(againJims.size(), 2u);
    ASSERT_EQ(againStock.size(), 2u);
    EXPECT_EQ(againJims[0]->harmonyType(), HarmonyType::JIMS);
    EXPECT_EQ(againJims[0]->harmonyName(), u"!So7/Ti");
    EXPECT_EQ(againJims[1]->harmonyType(), HarmonyType::JIMS);
    EXPECT_EQ(againJims[1]->harmonyName(), u"Re:So7");
    EXPECT_EQ(againStock[0]->harmonyType(), HarmonyType::STANDARD);
    EXPECT_EQ(againStock[1]->harmonyType(), HarmonyType::STANDARD);
    delete score;
    delete native;
    delete again;
}

TEST_F(MusicXml_JiMS_Tests, ChordNameV4PreservesOffsetStaffAndSupportedFormatting)
{
    MasterScore* score = readJims("jims-chord-name-offset-staff-format-v4.musicxml");
    ASSERT_TRUE(score);
    ASSERT_EQ(score->nstaves(), 2u);
    EXPECT_FALSE(staffTypeAtStart(score, 0)->isJiMS());
    EXPECT_TRUE(staffTypeAtStart(score, 1)->isJiMS());
    const std::vector<Harmony*> imported = harmoniesInOrder(score);
    ASSERT_EQ(imported.size(), 1u);
    Harmony* harmony = imported.front();
    EXPECT_EQ(harmony->harmonyType(), HarmonyType::JIMS);
    EXPECT_EQ(harmony->harmonyName(), u"La:So7");
    EXPECT_EQ(harmony->tick(), Fraction(1, 4));
    EXPECT_EQ(harmony->staffIdx(), 1u);
    EXPECT_EQ(harmony->placement(), PlacementV::BELOW);
    EXPECT_FALSE(harmony->visible());
    EXPECT_EQ(harmony->color(), Color::fromString("#112233"));
    EXPECT_EQ(harmony->family(), u"Edwin");
    EXPECT_DOUBLE_EQ(harmony->size(), 13.0);
    EXPECT_TRUE(harmony->fontStyle() & FontStyle::Italic);
    EXPECT_TRUE(harmony->fontStyle() & FontStyle::Bold);
    EXPECT_EQ(harmony->propertyFlags(Pid::OFFSET), PropertyFlags::UNSTYLED);
    EXPECT_NEAR(harmony->offset().x() / harmony->spatium(), 2.0, 0.01);
    EXPECT_NEAR(harmony->offset().y() / harmony->spatium(), 1.0, 0.01);

    auto musicXmlConfiguration = muse::modularity::globalIoc()->resolve<IMusicXmlConfiguration>("iex_musicxml");
    ASSERT_TRUE(musicXmlConfiguration);
    musicXmlConfiguration->setExportLayout(true);
    const String out = exportToScratch(score, "jims-chord-name-offset-staff-format-roundtrip.musicxml");
    musicXmlConfiguration->setExportLayout(false);
    const String xml = readAll(out);
    EXPECT_TRUE(xml.contains(u"<offset>"));
    EXPECT_TRUE(xml.contains(u"<staff>2</staff>"));
    EXPECT_TRUE(xml.contains(u"font-family=\"Edwin\""));
    EXPECT_TRUE(xml.contains(u"font-size=\"13\""));
    EXPECT_TRUE(xml.contains(u"font-style=\"italic\""));
    EXPECT_TRUE(xml.contains(u"font-weight=\"bold\""));
    EXPECT_TRUE(xml.contains(u"relative-x=\"20"));
    EXPECT_TRUE(xml.contains(u"relative-y=\"-10"));

    auto importXml = [](MasterScore* s, const muse::io::path_t& path) -> engraving::Err {
        return importMusicXml(s, path.toQString(), false);
    };
    MasterScore* again = ScoreRW::readScore(out, true, importXml);
    ASSERT_TRUE(again);
    const std::vector<Harmony*> roundTripped = harmoniesInOrder(again);
    ASSERT_EQ(roundTripped.size(), 1u);
    const Harmony* roundTrip = roundTripped.front();
    EXPECT_EQ(roundTrip->harmonyType(), HarmonyType::JIMS);
    EXPECT_EQ(roundTrip->harmonyName(), u"La:So7");
    EXPECT_EQ(roundTrip->tick(), Fraction(1, 4));
    EXPECT_EQ(roundTrip->staffIdx(), 1u);
    EXPECT_EQ(roundTrip->placement(), PlacementV::BELOW);
    EXPECT_FALSE(roundTrip->visible());
    EXPECT_EQ(roundTrip->color(), Color::fromString("#112233"));
    EXPECT_EQ(roundTrip->family(), u"Edwin");
    EXPECT_DOUBLE_EQ(roundTrip->size(), 13.0);
    EXPECT_TRUE(roundTrip->fontStyle() & FontStyle::Italic);
    EXPECT_TRUE(roundTrip->fontStyle() & FontStyle::Bold);
    EXPECT_NEAR(roundTrip->offset().x() / roundTrip->spatium(), 2.0, 0.01);
    EXPECT_NEAR(roundTrip->offset().y() / roundTrip->spatium(), 1.0, 0.01);

    const String nativePath = u"jims-export-scratch/jims-chord-name-offset-staff-format-roundtrip.mscz";
    muse::io::File::remove(nativePath);
    muse::io::File nativeFile(nativePath);
    ASSERT_TRUE(nativeFile.open(muse::io::IODevice::WriteOnly));
    MscWriter::Params params;
    params.device = &nativeFile;
    params.filePath = nativePath;
    params.mode = MscIoMode::Zip;
    MscWriter writer(params);
    ASSERT_TRUE(writer.open());
    MscSaver saver(again->iocContext());
    ASSERT_TRUE(saver.writeMscz(again, writer, false));
    writer.close();
    nativeFile.close();

    MasterScore* native = ScoreRW::readScore(nativePath, true);
    ASSERT_TRUE(native);
    const std::vector<Harmony*> nativeHarmonies = harmoniesInOrder(native);
    ASSERT_EQ(nativeHarmonies.size(), 1u);
    const Harmony* nativeHarmony = nativeHarmonies.front();
    EXPECT_EQ(nativeHarmony->harmonyType(), HarmonyType::JIMS);
    EXPECT_EQ(nativeHarmony->harmonyName(), u"La:So7");
    EXPECT_EQ(nativeHarmony->tick(), Fraction(1, 4));
    EXPECT_EQ(nativeHarmony->staffIdx(), 1u);
    EXPECT_EQ(nativeHarmony->placement(), PlacementV::BELOW);
    EXPECT_FALSE(nativeHarmony->visible());
    EXPECT_EQ(nativeHarmony->color(), Color::fromString("#112233"));
    EXPECT_EQ(nativeHarmony->family(), u"Edwin");
    EXPECT_DOUBLE_EQ(nativeHarmony->size(), 13.0);
    EXPECT_TRUE(nativeHarmony->fontStyle() & FontStyle::Italic);
    EXPECT_TRUE(nativeHarmony->fontStyle() & FontStyle::Bold);
    EXPECT_NEAR(nativeHarmony->offset().x() / nativeHarmony->spatium(), 2.0, 0.01);
    EXPECT_NEAR(nativeHarmony->offset().y() / nativeHarmony->spatium(), 1.0, 0.01);
    delete score;
    delete again;
    delete native;
}

TEST_F(MusicXml_JiMS_Tests, ChordNameV4PreservesOffsetBetweenNoteOnsets)
{
    MasterScore* score = readJims("jims-chord-name-unaligned-offset-v4.musicxml");
    ASSERT_TRUE(score);
    const std::vector<Harmony*> imported = harmoniesInOrder(score);
    ASSERT_EQ(imported.size(), 2u);
    EXPECT_EQ(imported[1]->harmonyName(), u"So5");
    EXPECT_EQ(imported[1]->tick(), Fraction(7, 16));

    const String out = exportToScratch(score, "jims-chord-name-unaligned-offset-roundtrip.musicxml");
    auto importXml = [](MasterScore* s, const muse::io::path_t& path) -> engraving::Err {
        return importMusicXml(s, path.toQString(), false);
    };
    MasterScore* again = ScoreRW::readScore(out, true, importXml);
    ASSERT_TRUE(again);
    const std::vector<Harmony*> roundTripped = harmoniesInOrder(again);
    ASSERT_EQ(roundTripped.size(), 2u);
    EXPECT_EQ(roundTripped[1]->harmonyName(), u"So5");
    EXPECT_EQ(roundTripped[1]->tick(), Fraction(7, 16));
    delete score;
    delete again;
}

TEST_F(MusicXml_JiMS_Tests, ChordNameV4RejectsSupersededTildeMarker)
{
    MasterScore* score = readJims("jims-chord-name-tilde-invalid.musicxml");
    EXPECT_FALSE(score);
    delete score;
}

TEST_F(MusicXml_JiMS_Tests, ChordNameV4RefusesNestedFretDiagramCarrierOnExport)
{
    MasterScore* score = ScoreRW::readScore(u"../../../engraving/tests/chordsymbol_data/add-to-fret.mscz");
    ASSERT_TRUE(score);
    Segment* segment = score->firstMeasure()->findFirstR(SegmentType::ChordRest, Fraction(0, 1));
    ASSERT_TRUE(segment);
    FretDiagram* fretDiagram = toFretDiagram(segment->findAnnotation(ElementType::FRET_DIAGRAM, 0, 0));
    ASSERT_TRUE(fretDiagram);
    fretDiagram->setHarmony(u"!So7/Ti");
    ASSERT_TRUE(fretDiagram->harmony());
    fretDiagram->harmony()->setHarmonyType(HarmonyType::JIMS);

    muse::io::Buffer buffer;
    buffer.open(muse::io::IODevice::WriteOnly);
    EXPECT_FALSE(saveXml(score, &buffer));
    EXPECT_TRUE(buffer.data().empty());
    delete score;
}

TEST_F(MusicXml_JiMS_Tests, numberedStatesLandOnTheirStavesAndMidScoreStatesRideChangeCarriers)
{
    MasterScore* multi = readJims("jims-multi-staff.musicxml");
    ASSERT_TRUE(multi);
    ASSERT_EQ(multi->nstaves(), 2u);
    const StaffType* s1 = staffTypeAtStart(multi, 0);
    const StaffType* s2 = staffTypeAtStart(multi, 1);
    ASSERT_TRUE(s1 && s2);
    EXPECT_TRUE(s1->isJiMS());
    EXPECT_TRUE(s2->isJiMS());
    EXPECT_NE(s1->jimsStateJson(), s2->jimsStateJson());
    delete multi;

    MasterScore* mid = readJims("jims-mid-score-state-change.musicxml");
    ASSERT_TRUE(mid);
    const StaffTypeChange* stc = jims::changeCarrier(measureNo(mid, 2), 0);
    ASSERT_TRUE(stc);
    EXPECT_TRUE(stc->staffType()->jimsStateJson().contains(u"\"generator_cents\":696.578"));
    EXPECT_TRUE(staffTypeAtStart(mid)->jimsStateJson().contains(u"\"generator_cents\":700.0"));
    delete mid;
}

TEST_F(MusicXml_JiMS_Tests, allSixAcceptedPiecesImportWithTheirChangeCarrier)
{
    struct Case {
        const char* file;
        int changeMeasure;
        const char* marker;
    };
    const Case cases[] = {
        { "jims-v3-m5-mode.musicxml", 2, "\"mode_rotation\":5" },
        { "jims-v3-m5-key-up.musicxml", 2, "\"key_number\":55" },
        { "jims-v3-m5-key-down.musicxml", 2, "\"key_number\":67" },
        { "jims-v3-m5-scale.musicxml", 2, "\"collection_rotation\":-3" },
        { "jims-v3-m5-key-mode.musicxml", 2, "\"key_number\":53" },
        { "jims-v3-m5-syshead.musicxml", 6, "\"mode_rotation\":0" },
    };
    for (const Case& c : cases) {
        MasterScore* score = readJims(c.file);
        ASSERT_TRUE(score) << c.file;
        EXPECT_TRUE(staffTypeAtStart(score)->isJiMS()) << c.file;
        const StaffTypeChange* stc = jims::changeCarrier(measureNo(score, c.changeMeasure), 0);
        ASSERT_TRUE(stc) << c.file;
        EXPECT_TRUE(stc->staffType()->jimsStateJson().contains(String::fromUtf8(c.marker))) << c.file;
        for (const Note* n : notesInOrder(score)) {
            EXPECT_TRUE(n->hasJimsPitch()) << c.file;
        }
        delete score;
    }
}

TEST_F(MusicXml_JiMS_Tests, authoritativeJimsIdentityNormalizesContradictoryStandardPitchOnImport)
{
    MasterScore* score = readJims("jims-v3-m5-key-down.musicxml");
    ASSERT_TRUE(score);
    int disagreements = 0;
    for (const Note* note : notesInOrder(score)) {
        ASSERT_TRUE(note->hasJimsPitch());
        const StaffType* state = note->staff()->staffTypeForElement(note);
        ASSERT_TRUE(state && state->isJiMS());
        jims::SoundingPitch projected;
        String error;
        ASSERT_TRUE(jims::noteSoundingPitch(state->jimsStateJson(), note->jimsNPer(), note->jimsNGen(), projected, &error))
            << error.toStdString();
        disagreements += note->pitch() != projected.midiKey;
    }
    EXPECT_EQ(disagreements, 0) << "jims:pitch is authoritative; adjacent standard pitch must be normalized";
    delete score;
}

// ---------------------------------------------------------------------------
// Interchange hardening — native JiMS MusicXML EXPORT (converged FINAL 96%,
// 2026-08-17). The Kernel writes every jims:staff-state / jims:change element
// in full (bridge ops from jims PR 214); the fork places them verbatim, adds
// jims:pitch from each JiMS note's two stored integers, declares the V3
// V4 namespace when JiMS content is present, and fails closed.
// ---------------------------------------------------------------------------
namespace {
struct JimsSnapshot {
    std::vector<String> baseStates;                             // Kernel-canonical XML per staff
    std::vector<std::pair<int, String> > carriers;              // (tick, Kernel-canonical XML) per staff, in order
    std::vector<std::pair<int, int> > identities;               // JiMS notes in document order (all tracks)
};

/// The Kernel's own canonical serialization of a state — semantic equality
/// through the Kernel, never a byte comparison of two JSON spellings.
String canonicalState(const String& stateJson)
{
    String xml;
    String err;
    EXPECT_TRUE(jims::musicxmlStaffStateV3Xml(stateJson, 0, xml, &err)) << err.toStdString();
    return xml;
}

JimsSnapshot snapshotOf(Score* score)
{
    JimsSnapshot snap;
    for (staff_idx_t s = 0; s < score->nstaves(); ++s) {
        const Staff* staff = score->staff(s);
        const StaffType* base = staff->staffType(Fraction(0, 1));
        snap.baseStates.push_back(base && base->isJiMS() ? canonicalState(base->jimsStateJson()) : String());
        for (const Measure* m = score->firstMeasure(); m; m = m->nextMeasure()) {
            const StaffTypeChange* c = jims::changeCarrier(m, s);
            if (c && c->staffType() && c->staffType()->isJiMS()) {
                snap.carriers.emplace_back(m->tick().ticks(), canonicalState(staff->staffType(m->tick())->jimsStateJson()));
            }
        }
    }
    for (const Segment* seg = score->firstSegment(SegmentType::ChordRest); seg; seg = seg->next1(SegmentType::ChordRest)) {
        for (track_idx_t t = 0; t < score->ntracks(); ++t) {
            const EngravingItem* el = seg->element(t);
            if (el && el->isChord()) {
                for (const Chord* g : toChord(el)->graceNotes()) {
                    for (const Note* n : g->notes()) {
                        if (n->hasJimsPitch()) {
                            snap.identities.emplace_back(n->jimsNPer(), n->jimsNGen());
                        }
                    }
                }
                for (const Note* n : toChord(el)->notes()) {
                    if (n->hasJimsPitch()) {
                        snap.identities.emplace_back(n->jimsNPer(), n->jimsNGen());
                    }
                }
            }
        }
    }
    return snap;
}

String exportToScratch(MasterScore* score, const char* name)
{
    // The stock export tests do the same before saving (musicxml_tests.cpp).
    score->connectTies();
    score->masterScore()->rebuildMidiMapping();
    score->doLayout();
    const String dir(u"jims-export-scratch");
    muse::io::Dir::mkpath(dir);
    const String path = dir + u"/" + String::fromUtf8(name);
    muse::io::File::remove(path);
    EXPECT_TRUE(saveXml(score, path)) << name;
    return path;
}

String readAll(const String& path)
{
    muse::io::File f(path);
    EXPECT_TRUE(f.open(muse::io::IODevice::ReadOnly)) << path.toStdString();
    return String::fromUtf8(f.readAll());
}
}

TEST_F(MusicXml_JiMS_Tests, exportWritesV4AndRoundTripsThroughTheNativeImporter)
{
    const char* corpus[] = {
        "jims-v3-m5-mode.musicxml", "jims-v3-m5-key-up.musicxml", "jims-v3-m5-key-down.musicxml",
        "jims-v3-m5-scale.musicxml", "jims-v3-m5-key-mode.musicxml", "jims-v3-m5-syshead.musicxml",
        "jims-reference-none.musicxml", "jims-reference-none-explicit.musicxml", "jims-reference-pitch.musicxml",
        "jims-reference-pitch-class.musicxml", "jims-reference-hertz.musicxml", "jims-mid-score-state-change.musicxml",
        "jims-multi-staff.musicxml", "jims-12tet-diatonic.musicxml", "jims-v2-mode-change.musicxml",
    };
    for (const char* file : corpus) {
        MasterScore* original = readJims(file);
        ASSERT_TRUE(original) << file;
        original->doLayout();
        const JimsSnapshot before = snapshotOf(original);
        ASSERT_FALSE(before.identities.empty()) << file;
        const String out = exportToScratch(original, (String(u"export-") + String::fromUtf8(file)).toStdString().c_str());
        const String xml = readAll(out);
        EXPECT_TRUE(xml.contains(u"xmlns:jims=\"urn:jims:musicxml:4\"")) << file;
        EXPECT_TRUE(xml.contains(u"<jims:staff-state")) << file;
        EXPECT_TRUE(xml.contains(u"<jims:pitch ")) << file;
        // Round trip through the accepted native importer.
        auto importXml = [](MasterScore* score, const muse::io::path_t& path) -> engraving::Err {
            return importMusicXml(score, path.toQString(), false);
        };
        MasterScore* again = ScoreRW::readScore(out, true, importXml);
        ASSERT_TRUE(again) << file;
        again->doLayout();
        const JimsSnapshot after = snapshotOf(again);
        EXPECT_EQ(after.baseStates, before.baseStates) << file;
        EXPECT_EQ(after.carriers, before.carriers) << file;
        EXPECT_EQ(after.identities, before.identities) << file;
        // Re-export is again valid-looking and semantically identical.
        const String out2 = exportToScratch(again, (String(u"reexport-") + String::fromUtf8(file)).toStdString().c_str());
        MasterScore* third = ScoreRW::readScore(out2, true, importXml);
        ASSERT_TRUE(third) << file;
        third->doLayout();
        const JimsSnapshot after2 = snapshotOf(third);
        EXPECT_EQ(after2.baseStates, before.baseStates) << file;
        EXPECT_EQ(after2.carriers, before.carriers) << file;
        EXPECT_EQ(after2.identities, before.identities) << file;
        delete original;
        delete again;
        delete third;
    }
}

TEST_F(MusicXml_JiMS_Tests, exportOfANativeJiMSScoreCarriesStatesChangesAndIdentities)
{
    // The M6/M7 gate file: base reference 62, bar-2 carrier (reference 53,
    // mode La), bar 3 pasted identities.
    MasterScore* score = ScoreRW::readScore(JIMS_DATA_DIR + u"m7-gate.mscz");
    ASSERT_TRUE(score);
    score->doLayout();
    const JimsSnapshot before = snapshotOf(score);
    ASSERT_EQ(before.identities.size(), 12u);
    ASSERT_EQ(before.carriers.size(), 1u);
    const String out = exportToScratch(score, "export-m7-gate.musicxml");
    const String xml = readAll(out);
    EXPECT_TRUE(xml.contains(u"xmlns:jims=\"urn:jims:musicxml:4\""));
    // Two states (base + bar 2) and one Kernel change (key, mode) right after the later state.
    EXPECT_EQ(int(xml.count(u"<jims:staff-state>")), 2);
    EXPECT_EQ(int(xml.count(u"<jims:change>")), 1);
    const size_t later = xml.indexOf(u"<jims:staff-state>", xml.indexOf(u"<jims:staff-state>") + 1);
    const size_t change = xml.indexOf(u"<jims:change>");
    EXPECT_LT(later, change);
    EXPECT_TRUE(xml.contains(u"<jims:kind>key</jims:kind>"));
    EXPECT_TRUE(xml.contains(u"<jims:kind>mode</jims:kind>"));
    EXPECT_EQ(int(xml.count(u"<jims:pitch ")), 12);
    // No jims:staff-state shares an <attributes> block with staff-lines (Schematron rule).
    size_t pos = 0;
    while ((pos = xml.indexOf(u"<attributes>", pos)) != muse::nidx) {
        const size_t end = xml.indexOf(u"</attributes>", pos);
        const String block = xml.mid(pos, end - pos);
        EXPECT_FALSE(block.contains(u"<jims:staff-state") && block.contains(u"<staff-lines>")) << block.toStdString();
        pos = end;
    }
    auto importXml = [](MasterScore* s, const muse::io::path_t& path) -> engraving::Err {
        return importMusicXml(s, path.toQString(), false);
    };
    MasterScore* again = ScoreRW::readScore(out, true, importXml);
    ASSERT_TRUE(again);
    again->doLayout();
    const JimsSnapshot after = snapshotOf(again);
    EXPECT_EQ(after.baseStates, before.baseStates);
    EXPECT_EQ(after.carriers, before.carriers);
    EXPECT_EQ(after.identities, before.identities);
    delete score;
    delete again;
}

TEST_F(MusicXml_JiMS_Tests, multiStaffExportNumbersStatesThroughTheKernel)
{
    MasterScore* score = readJims("jims-multi-staff.musicxml");
    ASSERT_TRUE(score);
    score->doLayout();
    const String out = exportToScratch(score, "export-multi-staff.musicxml");
    const String xml = readAll(out);
    EXPECT_TRUE(xml.contains(u"<jims:staff-state number=\"1\">")) << xml.toStdString().substr(0, 2000);
    EXPECT_TRUE(xml.contains(u"<jims:staff-state number=\"2\">"));
    delete score;
}

TEST_F(MusicXml_JiMS_Tests, stockScoreExportDeclaresNoJimsNamespace)
{
    auto importXml = [](MasterScore* s, const muse::io::path_t& path) -> engraving::Err {
        return importMusicXml(s, path.toQString(), false);
    };
    MasterScore* score = ScoreRW::readScore(u"data/testHello.xml", false, importXml);
    ASSERT_TRUE(score);
    score->doLayout();
    const String out = exportToScratch(score, "export-stock-hello.musicxml");
    const String xml = readAll(out);
    EXPECT_FALSE(xml.contains(u"jims"));
    EXPECT_TRUE(xml.contains(u"<score-partwise version=\"4.0\">"));
    delete score;
}

TEST_F(MusicXml_JiMS_Tests, exportFailsClosedWhenAJimsNoteLacksItsIdentity)
{
    MasterScore* score = ScoreRW::readScore(JIMS_DATA_DIR + u"m7-gate.mscz");
    ASSERT_TRUE(score);
    score->doLayout();
    // Strip one identity (a defective document): export must refuse and
    // write nothing.
    for (Segment* seg = score->firstSegment(SegmentType::ChordRest); seg; seg = seg->next1(SegmentType::ChordRest)) {
        EngravingItem* el = seg->element(0);
        if (el && el->isChord()) {
            toChord(el)->notes().front()->setJimsPitch(INT_MIN, INT_MIN);   // Note::JIMS_UNSET (private)
            break;
        }
    }
    muse::io::Buffer buf;
    buf.open(muse::io::IODevice::WriteOnly);
    String error;
    EXPECT_FALSE(saveXml(score, &buf, &error));
    EXPECT_TRUE(error.contains(u"no lattice identity"));
    EXPECT_TRUE(buf.data().empty());
    muse::io::Buffer mxl;
    mxl.open(muse::io::IODevice::ReadWrite);
    String mxlError;
    EXPECT_FALSE(saveMxl(score, &mxl, &mxlError));
    EXPECT_EQ(mxlError, error);
    EXPECT_TRUE(mxl.data().empty());
    delete score;
}

TEST_F(MusicXml_JiMS_Tests, trustedRawFragmentWriterInsertsVerbatimAndKeepsBalance)
{
    muse::io::Buffer buf;
    buf.open(muse::io::IODevice::WriteOnly);
    XmlWriter xml(&buf);
    xml.startDocument();
    xml.startElement("attributes");
    xml.tag("divisions", 1);
    xml.writeTrustedRawFragment(u"<jims:staff-state number=\"2\"><jims:x a=\"&amp;\"/></jims:staff-state>");
    xml.tag("after", 2);
    xml.endElement();
    xml.flush();
    const String out = String::fromUtf8(buf.data());
    EXPECT_TRUE(out.contains(u"<jims:staff-state number=\"2\"><jims:x a=\"&amp;\"/></jims:staff-state>\n")) << out.toStdString();
    EXPECT_LT(out.indexOf(u"<divisions>"), out.indexOf(u"<jims:staff-state"));
    EXPECT_LT(out.indexOf(u"</jims:staff-state>"), out.indexOf(u"<after>"));
    EXPECT_TRUE(out.contains(u"</attributes>"));
}

// JiMStaff Milestone 8 (octave-band elision): the three presentation switches
// (score style jimsElideEmptyOctaves / jimsShowAllOctavesInFirstSystem, staff
// type Auto/On/Off) never reach MusicXML — export is byte-identical with
// elision off and on, and no jims:staff-state carries them.
TEST_F(MusicXml_JiMS_Tests, m8ElisionSwitchesNeverChangeMusicXmlExport)
{
    MasterScore* score = ScoreRW::readScore(JIMS_DATA_DIR + u"m8-two-hand.mscx");
    ASSERT_TRUE(score);
    score->doLayout();
    const String off = readAll(exportToScratch(score, "export-m8-two-hand-off.musicxml"));
    ASSERT_TRUE(off.contains(u"<jims:staff-state>"));
    EXPECT_FALSE(off.contains(u"elide"));
    EXPECT_FALSE(off.contains(u"Elide"));

    score->style().set(Sid::jimsElideEmptyOctaves, true);
    score->style().set(Sid::jimsShowAllOctavesInFirstSystem, false);
    score->staff(0)->staffType(Fraction(0, 1))->setJimsElideOctaves(JimsElideOctaves::On);
    score->setLayoutAll();
    score->doLayout();
    // The banded layout is in effect (system 2 has two bands) ...
    System* system2 = nullptr;
    int measureSystems = 0;
    for (System* s : score->systems()) {
        if (s->firstMeasure() && ++measureSystems == 2) {
            system2 = s;
        }
    }
    ASSERT_TRUE(system2);
    EXPECT_EQ(staffTypeAtStart(score)->jimsFrameView(score, 0, system2).bands.size(), 2u);
    // ... and the export is byte for byte the same.
    const String on = readAll(exportToScratch(score, "export-m8-two-hand-on.musicxml"));
    EXPECT_EQ(on, off);
    EXPECT_FALSE(on.contains(u"elide"));
    delete score;
}

// ---------------------------------------------------------------------------
// Interchange hardening 2 (owner decisions 2026-08-19): jims:provenance and
// jims:tuning-trajectory are transported — imported, saved in the score file,
// exported back exactly as carried — and multi-part documents follow the
// owner's rule: several JiMS parts allowed, mixed JiMS + stock parts allowed,
// every JiMS part shares one state timeline.
// ---------------------------------------------------------------------------

TEST_F(MusicXml_JiMS_Tests, provenanceIsImportedSavedAndExportedVerbatim)
{
    MasterScore* score = readJims("jims-provenance.musicxml");
    ASSERT_TRUE(score);
    score->doLayout();
    const jims::Provenance prov = score->jimsProvenance();   // by value: the score is deleted before the reload check
    ASSERT_EQ(prov.resources.size(), 3u);
    EXPECT_TRUE(prov.strictFallback);
    EXPECT_EQ(prov.resources[0].role, u"source");
    EXPECT_EQ(prov.resources[0].uri, u"https://example.org/scores/original.pdf");
    EXPECT_EQ(prov.resources[0].mediaType, u"application/pdf");
    EXPECT_EQ(prov.resources[0].sha256, u"e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
    EXPECT_EQ(prov.resources[0].text, u"Original engraving");
    EXPECT_EQ(prov.resources[1].role, u"master");
    EXPECT_TRUE(prov.resources[1].sha256.isEmpty());
    EXPECT_TRUE(prov.resources[1].text.isEmpty());
    EXPECT_EQ(prov.resources[2].role, u"arrangement");
    // Export writes it back inside identification, before miscellaneous.
    const String out = exportToScratch(score, "export-provenance.musicxml");
    const String xml = readAll(out);
    EXPECT_TRUE(xml.contains(u"<jims:provenance fallback-profile=\"strict\">"));
    EXPECT_EQ(int(xml.count(u"<jims:resource ")), 3);
    EXPECT_TRUE(xml.contains(
                    u"sha-256=\"e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855\">Original engraving</jims:resource>"));
    const size_t provPos = xml.indexOf(u"<jims:provenance");
    const size_t identEnd = xml.indexOf(u"</identification>");
    const size_t misc = xml.indexOf(u"<miscellaneous>");
    ASSERT_NE(provPos, muse::nidx);
    EXPECT_LT(provPos, identEnd);
    if (misc != muse::nidx) {
        EXPECT_LT(provPos, misc);
    }
    // Round trip: the reimported carrier is equal.
    auto importXml = [](MasterScore* s, const muse::io::path_t& path) -> engraving::Err {
        return importMusicXml(s, path.toQString(), false);
    };
    MasterScore* again = ScoreRW::readScore(out, true, importXml);
    ASSERT_TRUE(again);
    EXPECT_TRUE(again->jimsProvenance() == prov);
    delete again;
    // Score-file persistence (.mscz): survives save + reload, then exports the same.
    const String dir(u"jims-export-scratch");
    muse::io::Dir::mkpath(dir);
    const String mscz = dir + u"/provenance-roundtrip.mscz";
    muse::io::File::remove(mscz);
    {
        muse::io::File file(mscz);
        ASSERT_TRUE(file.open(muse::io::IODevice::WriteOnly));
        MscWriter::Params params;
        params.device = &file;
        params.filePath = mscz;
        params.mode = MscIoMode::Zip;
        MscWriter writer(params);
        ASSERT_TRUE(writer.open());
        MscSaver saver(score->iocContext());
        ASSERT_TRUE(saver.writeMscz(score, writer, false));
        writer.close();
        file.close();
    }
    delete score;
    MasterScore* reloaded = ScoreRW::readScore(mscz, true);
    ASSERT_TRUE(reloaded);
    reloaded->doLayout();
    EXPECT_TRUE(reloaded->jimsProvenance() == prov);
    const String xml2 = readAll(exportToScratch(reloaded, "export-provenance-after-mscz.musicxml"));
    EXPECT_EQ(int(xml2.count(u"<jims:resource ")), 3);
    EXPECT_TRUE(xml2.contains(u"<jims:provenance fallback-profile=\"strict\">"));
    delete reloaded;
}

TEST_F(MusicXml_JiMS_Tests, tuningTrajectoriesAreImportedSavedAndExportedVerbatim)
{
    struct Case {
        const char* file;
        const char* interpolation;
        size_t controls;
    };
    const Case cases[] = { { "jims-trajectory-linear.musicxml", "linear", 0 }, { "jims-trajectory-cubic.musicxml", "cubic-bezier", 2 } };
    for (const Case& c : cases) {
        MasterScore* score = readJims(c.file);
        ASSERT_TRUE(score) << c.file;
        score->doLayout();
        const std::vector<jims::TuningTrajectory>& ts = score->staff(0)->jimsTuningTrajectories();
        ASSERT_EQ(ts.size(), 1u) << c.file;
        const jims::TuningTrajectory t = ts[0];   // by value: the score is deleted before the reload check
        EXPECT_EQ(t.tick, Fraction(0, 1)) << c.file;
        EXPECT_EQ(t.placement, u"above") << c.file;
        ASSERT_EQ(t.segments.size(), 1u) << c.file;
        const jims::TrajectorySegment& seg = t.segments[0];
        EXPECT_EQ(seg.duration, Fraction(4, 4)) << c.file;   // 16 divisions at divisions=4: one whole note
        EXPECT_EQ(seg.startCents, u"700") << c.file;
        EXPECT_EQ(seg.endCents, u"696") << c.file;
        EXPECT_EQ(seg.interpolation, String::fromAscii(c.interpolation)) << c.file;
        ASSERT_EQ(seg.controls.size(), c.controls) << c.file;
        if (c.controls == 2) {
            EXPECT_EQ(seg.controls[0].time, u"0.25");
            EXPECT_EQ(seg.controls[0].valueCents, u"699");
            EXPECT_EQ(seg.controls[1].time, u"0.75");
            EXPECT_EQ(seg.controls[1].valueCents, u"697");
        }
        // Export: a direction at the trajectory's measure with the carrier and
        // the duration re-expressed in the export's own divisions.
        const String out = exportToScratch(score, (String(u"export-") + String::fromUtf8(c.file)).toStdString().c_str());
        const String xml = readAll(out);
        EXPECT_TRUE(xml.contains(u"<direction placement=\"above\">")) << c.file;
        EXPECT_TRUE(xml.contains(u"<jims:tuning-trajectory>")) << c.file;
        EXPECT_TRUE(xml.contains(u"start-cents=\"700\" end-cents=\"696\" interpolation=\"" + String::fromAscii(c.interpolation) + u"\""))
            << c.file << "\n" << xml.toStdString().substr(0, 3000);
        if (c.controls == 2) {
            EXPECT_TRUE(xml.contains(u"<jims:control time=\"0.25\" value-cents=\"699\"/>")) << c.file;
            EXPECT_TRUE(xml.contains(u"<jims:control time=\"0.75\" value-cents=\"697\"/>")) << c.file;
        }
        // The exported duration-divisions is one whole note in the export's divisions.
        const size_t divPos = xml.indexOf(u"<divisions>");
        ASSERT_NE(divPos, muse::nidx) << c.file;
        const size_t divEnd = xml.indexOf(u"</divisions>", divPos);
        const int divisions = xml.mid(divPos + 11, divEnd - divPos - 11).toInt();
        EXPECT_TRUE(xml.contains(String(u"duration-divisions=\"%1\"").arg(4 * divisions))) << c.file << " divisions=" << divisions;
        // Round trip through the importer: equal carrier.
        auto importXml = [](MasterScore* s, const muse::io::path_t& path) -> engraving::Err {
            return importMusicXml(s, path.toQString(), false);
        };
        MasterScore* again = ScoreRW::readScore(out, true, importXml);
        ASSERT_TRUE(again) << c.file;
        again->doLayout();
        ASSERT_EQ(again->staff(0)->jimsTuningTrajectories().size(), 1u) << c.file;
        EXPECT_TRUE(again->staff(0)->jimsTuningTrajectories()[0] == t) << c.file;
        delete again;
        // Score-file persistence (.mscz).
        const String dir(u"jims-export-scratch");
        muse::io::Dir::mkpath(dir);
        const String mscz = dir + u"/" + String::fromUtf8(c.file) + u".mscz";
        muse::io::File::remove(mscz);
        {
            muse::io::File file(mscz);
            ASSERT_TRUE(file.open(muse::io::IODevice::WriteOnly));
            MscWriter::Params params;
            params.device = &file;
            params.filePath = mscz;
            params.mode = MscIoMode::Zip;
            MscWriter writer(params);
            ASSERT_TRUE(writer.open());
            MscSaver saver(score->iocContext());
            ASSERT_TRUE(saver.writeMscz(score, writer, false));
            writer.close();
            file.close();
        }
        delete score;
        MasterScore* reloaded = ScoreRW::readScore(mscz, true);
        ASSERT_TRUE(reloaded) << c.file;
        reloaded->doLayout();
        ASSERT_EQ(reloaded->staff(0)->jimsTuningTrajectories().size(), 1u) << c.file;
        EXPECT_TRUE(reloaded->staff(0)->jimsTuningTrajectories()[0] == t) << c.file;
        delete reloaded;
    }
}

TEST_F(MusicXml_JiMS_Tests, malformedCarriersAreFatalImportErrors)
{
    // A trajectory segment without interpolation, and a provenance resource
    // without a role: a JiMS document never imports with part of its JiMS
    // content silently dropped.
    const String dir(u"jims-export-scratch");
    muse::io::Dir::mkpath(dir);
    struct Bad {
        const char* base;
        const char* find;
        const char* replace;
        const char* name;
    };
    const Bad bads[] = {
        { "jims-trajectory-linear.musicxml", " interpolation=\"linear\"", "", "bad-trajectory.musicxml" },
        { "jims-provenance.musicxml", "role=\"master\" ", "", "bad-provenance.musicxml" },
    };
    for (const Bad& b : bads) {
        String text = readAll(ScoreRW::rootPath() + u"/" + JIMS_DATA_DIR + String::fromUtf8(b.base));
        ASSERT_TRUE(text.contains(String::fromUtf8(b.find))) << b.name;
        text.replace(String::fromUtf8(b.find), String::fromUtf8(b.replace));
        const String path = dir + u"/" + String::fromUtf8(b.name);
        muse::io::File f(path);
        ASSERT_TRUE(f.open(muse::io::IODevice::WriteOnly));
        f.write(text.toUtf8());
        f.close();
        auto importXml = [](MasterScore* s, const muse::io::path_t& p) -> engraving::Err {
            return importMusicXml(s, p.toQString(), false);
        };
        MasterScore* score = ScoreRW::readScore(path, true, importXml);
        EXPECT_FALSE(score) << b.name;
        delete score;
    }
}

TEST_F(MusicXml_JiMS_Tests, severalJimsPartsSharingOneTimelineImportAndRoundTrip)
{
    MasterScore* score = readJims("jims-multi-part-shared.musicxml");
    ASSERT_TRUE(score);
    score->doLayout();
    ASSERT_EQ(score->nstaves(), 2u);
    for (staff_idx_t s = 0; s < 2; ++s) {
        EXPECT_TRUE(staffTypeAtStart(score, s)->isJiMS()) << s;
        Measure* m2 = measureNo(score, 2);
        ASSERT_TRUE(m2);
        EXPECT_TRUE(jims::changeCarrier(m2, s) != nullptr) << s;   // the La-mode section on both parts
    }
    const JimsSnapshot before = snapshotOf(score);
    EXPECT_EQ(before.identities.size(), 4u);
    EXPECT_EQ(before.carriers.size(), 2u);
    EXPECT_EQ(sharedState(before.baseStates[0]), sharedState(before.baseStates[1]));
    EXPECT_EQ(sharedState(before.carriers[0].second), sharedState(before.carriers[1].second));
    const String out = exportToScratch(score, "export-multi-part-shared.musicxml");
    const String xml = readAll(out);
    EXPECT_EQ(int(xml.count(u"<jims:staff-state>")), 4);   // two parts x (base + bar 2)
    auto importXml = [](MasterScore* s, const muse::io::path_t& path) -> engraving::Err {
        return importMusicXml(s, path.toQString(), false);
    };
    MasterScore* again = ScoreRW::readScore(out, true, importXml);
    ASSERT_TRUE(again);
    again->doLayout();
    const JimsSnapshot after = snapshotOf(again);
    EXPECT_EQ(after.baseStates, before.baseStates);
    EXPECT_EQ(after.carriers, before.carriers);
    EXPECT_EQ(after.identities, before.identities);
    delete score;
    delete again;
}

// Owner ruling 2026-08-22 (M8.9): parts of one document are compared on the
// Kernel's shared projection, which omits the per-staff extent. Four SATB
// voices legitimately differ in frame extent, while tonic-ambit is one
// song-wide value that every transport carrier must share. Before this
// change the whole element was compared and such a document was refused.
TEST_F(MusicXml_JiMS_Tests, partsDifferingOnlyInPerStaffFieldsImportAndRoundTrip)
{
    MasterScore* score = readJims("jims-multi-part-perstaff-differs.musicxml");
    ASSERT_TRUE(score);
    score->doLayout();
    ASSERT_EQ(score->nstaves(), 2u);
    const JimsSnapshot before = snapshotOf(score);
    ASSERT_EQ(before.baseStates.size(), 2u);
    // The two parts really do differ — this is the condition that used to be
    // refused outright, so importing at all is the behaviour under test.
    EXPECT_NE(before.baseStates[0], before.baseStates[1]);

    // The Kernel's shared projection is blind only to extent.
    const String centered
        =
            uR"({"scale":["M2","m2","M2","M2","M2","m2","M2"],"collection_rotation":0,"mode_rotation":0,"generator_cents":700.0,"period_cents":1200.0,"embedding":{"large_steps":5,"small_steps":2},"extent":{"lower":{"nPer":1,"nGen":-2},"upper":{"nPer":2,"nGen":-2}},"tonic_ambit":"tonic-centered","reference":"none"})";
    const String otherExtent
        =
            uR"({"scale":["M2","m2","M2","M2","M2","m2","M2"],"collection_rotation":0,"mode_rotation":0,"generator_cents":700.0,"period_cents":1200.0,"embedding":{"large_steps":5,"small_steps":2},"extent":{"lower":{"nPer":0,"nGen":-2},"upper":{"nPer":1,"nGen":-2}},"tonic_ambit":"tonic-centered","reference":"none"})";
    String sharedCentered, sharedOtherExtent, err;
    ASSERT_TRUE(jims::musicxmlSharedStateV3Xml(centered, sharedCentered, &err)) << err.toStdString();
    ASSERT_TRUE(jims::musicxmlSharedStateV3Xml(otherExtent, sharedOtherExtent, &err)) << err.toStdString();
    EXPECT_EQ(sharedCentered, sharedOtherExtent) << "extent must not make parts disagree";
    EXPECT_FALSE(sharedCentered.contains(u"jims:extent"));
    EXPECT_TRUE(sharedCentered.contains(u"jims:tonic-ambit"));
    const String otherAmbit = String(centered).replace(u"tonic-centered", u"tonic-bounded");
    String sharedOtherAmbit;
    ASSERT_TRUE(jims::musicxmlSharedStateV3Xml(otherAmbit, sharedOtherAmbit, &err)) << err.toStdString();
    EXPECT_NE(sharedOtherAmbit, sharedCentered) << "tonic-ambit is song-wide and must be compared";
    // ...while a real musical difference still shows up as one.
    String sharedOtherMode;
    const String otherMode = String(centered).replace(u"\"mode_rotation\":0", u"\"mode_rotation\":5");
    ASSERT_TRUE(jims::musicxmlSharedStateV3Xml(otherMode, sharedOtherMode, &err)) << err.toStdString();
    EXPECT_NE(sharedOtherMode, sharedCentered);

    const String out = exportToScratch(score, "export-multi-part-perstaff-differs.musicxml");
    const String xml = readAll(out);
    // Both per-staff values survive export verbatim: the data stays, only the
    // comparison narrowed.
    EXPECT_TRUE(xml.contains(u"lower-n-per=\"0\" lower-n-gen=\"0\" upper-n-per=\"0\" upper-n-gen=\"0\""));
    EXPECT_TRUE(xml.contains(u"lower-n-per=\"-1\" lower-n-gen=\"-1\" upper-n-per=\"-1\" upper-n-gen=\"-1\""));
    EXPECT_TRUE(xml.contains(u"<jims:tonic-ambit>"));   // the field is still written per staff

    auto importXml = [](MasterScore* s, const muse::io::path_t& path) -> engraving::Err {
        return importMusicXml(s, path.toQString(), false);
    };
    MasterScore* again = ScoreRW::readScore(out, true, importXml);
    ASSERT_TRUE(again);
    again->doLayout();
    const JimsSnapshot after = snapshotOf(again);
    EXPECT_EQ(after.baseStates, before.baseStates);
    EXPECT_EQ(after.identities, before.identities);
    delete score;
    delete again;
}

TEST_F(MusicXml_JiMS_Tests, aJimsPartBesideAStockPartImportsAndRoundTrips)
{
    MasterScore* score = readJims("jims-multi-part-mixed.musicxml");
    ASSERT_TRUE(score);
    score->doLayout();
    ASSERT_EQ(score->nstaves(), 2u);
    EXPECT_TRUE(staffTypeAtStart(score, 0)->isJiMS());
    EXPECT_FALSE(staffTypeAtStart(score, 1)->isJiMS());
    const JimsSnapshot before = snapshotOf(score);
    EXPECT_EQ(before.identities.size(), 2u);   // only the JiMS part carries identities
    const String out = exportToScratch(score, "export-multi-part-mixed.musicxml");
    const String xml = readAll(out);
    EXPECT_EQ(int(xml.count(u"<jims:staff-state>")), 2);
    // The stock part is exported as stock: its notes carry no jims:pitch.
    const size_t p2 = xml.indexOf(u"<part id=\"P2\">");
    ASSERT_NE(p2, muse::nidx);
    EXPECT_EQ(xml.mid(p2).indexOf(u"<jims:pitch"), muse::nidx);
    auto importXml = [](MasterScore* s, const muse::io::path_t& path) -> engraving::Err {
        return importMusicXml(s, path.toQString(), false);
    };
    MasterScore* again = ScoreRW::readScore(out, true, importXml);
    ASSERT_TRUE(again);
    again->doLayout();
    EXPECT_TRUE(staffTypeAtStart(again, 0)->isJiMS());
    EXPECT_FALSE(staffTypeAtStart(again, 1)->isJiMS());
    const JimsSnapshot after = snapshotOf(again);
    EXPECT_EQ(after.baseStates, before.baseStates);
    EXPECT_EQ(after.identities, before.identities);
    delete score;
    delete again;
}

TEST_F(MusicXml_JiMS_Tests, jimsPartsWithDifferentTimelinesAreRefusedOnImportAndOnExport)
{
    // Import: the divergent fixture is refused outright.
    EXPECT_FALSE(readJims("jims-multi-part-divergent-invalid.musicxml"));
    // Export: a document whose JiMS parts have drifted apart in the editor
    // is refused, and nothing is written.
    MasterScore* score = readJims("jims-multi-part-shared.musicxml");
    ASSERT_TRUE(score);
    score->doLayout();
    StaffType* st = score->staff(1)->staffType(Fraction(0, 1));
    String json = st->jimsStateJson();
    ASSERT_TRUE(json.contains(u"\"generator_cents\":700.0"));
    json.replace(u"\"generator_cents\":700.0", u"\"generator_cents\":696.578");
    st->setJimsStateJson(json);
    score->setLayoutAll();
    score->doLayout();
    muse::io::Buffer buf;
    buf.open(muse::io::IODevice::WriteOnly);
    EXPECT_FALSE(saveXml(score, &buf));
    EXPECT_TRUE(buf.data().empty());
    delete score;
}

// ---------------------------------------------------------------------------
// Milestone 9 — the SATB (JiMStaff) template through the current JiMS namespace
// ---------------------------------------------------------------------------

namespace {
String satbTemplatePath()
{
    // This module's data root is src/importexport/musicxml/tests, so the fork
    // root is four levels up (the engraving suite is three).
    return ScoreRW::rootPath() + u"/../../../../share/templates/02-Choral/12-SATB_(JiMStaff)/12-SATB_(JiMStaff).mscx";
}
}

// The four voices carry one shared musical timeline and four DIFFERENT frame
// extents. Under the narrowed comparison that document is
// accepted, and every voice's own frame height survives the round trip.
TEST_F(MusicXml_JiMS_Tests, m9SATBTemplateRoundTripsPreservingEachVoicesOwnExtent)
{
    MasterScore* score = ScoreRW::readScore(satbTemplatePath(), true);
    ASSERT_TRUE(score) << "the SATB (JiMStaff) template is not shipped";
    score->doLayout();
    ASSERT_EQ(score->nstaves(), 4u);

    const JimsSnapshot before = snapshotOf(score);
    ASSERT_EQ(before.baseStates.size(), 4u);
    for (const String& s : before.baseStates) {
        EXPECT_FALSE(s.empty()) << "every SATB staff must be a JiMStaff";
    }

    const String out = exportToScratch(score, "export-m9-satb-template.musicxml");
    const String xml = readAll(out);
    EXPECT_EQ(xml.count(u"lower-n-per=\"3\" lower-n-gen=\"-5\" upper-n-per=\"4\" upper-n-gen=\"-5\""), 1);
    EXPECT_EQ(xml.count(u"lower-n-per=\"2\" lower-n-gen=\"-4\" upper-n-per=\"3\" upper-n-gen=\"-4\""), 1);
    EXPECT_EQ(xml.count(u"lower-n-per=\"1\" lower-n-gen=\"-3\" upper-n-per=\"2\" upper-n-gen=\"-3\""), 1);
    EXPECT_EQ(xml.count(u"lower-n-per=\"2\" lower-n-gen=\"-6\" upper-n-per=\"3\" upper-n-gen=\"-6\""), 1)
        << "each SATB voice must export its own Kernel-derived empty-staff extent";

    auto importXml = [](MasterScore* s, const muse::io::path_t& path) -> engraving::Err {
        return importMusicXml(s, path.toQString(), false);
    };
    MasterScore* again = ScoreRW::readScore(out, true, importXml);
    ASSERT_TRUE(again) << "a four-part document differing only in jims:extent must import";
    again->doLayout();
    ASSERT_EQ(again->nstaves(), 4u);
    const JimsSnapshot after = snapshotOf(again);
    EXPECT_EQ(after.baseStates, before.baseStates);
    EXPECT_EQ(after.identities, before.identities);

    // Re-export once more: no drift in anything JiMS owns. The one byte that
    // does move is stock MuseScore's part-group round trip — a re-imported
    // score re-exports <group-barline>yes even though the source omitted it —
    // and the stock 02-Choral/01-SATB template drifts identically, so it is
    // not a JiMS behaviour and not this milestone's to change. Recorded as an
    // observed follow-up in the M9 final report.
    const String out2 = exportToScratch(again, "export-m9-satb-template-2.musicxml");
    const String xml2 = readAll(out2);
    auto jimsLinesOf = [](const String& doc) {
        StringList out;
        for (const String& line : doc.split(u'\n')) {
            if (line.contains(u"jims:")) {
                out.push_back(line.trimmed());
            }
        }
        return out;
    };
    EXPECT_EQ(jimsLinesOf(xml2), jimsLinesOf(xml)) << "the JiMS content must not drift across a second round trip";
    EXPECT_EQ(xml2.count(u"lower-n-per=\"3\" lower-n-gen=\"-5\" upper-n-per=\"4\" upper-n-gen=\"-5\""), 1);
    EXPECT_EQ(xml2.count(u"lower-n-per=\"2\" lower-n-gen=\"-4\" upper-n-per=\"3\" upper-n-gen=\"-4\""), 1);
    EXPECT_EQ(xml2.count(u"lower-n-per=\"1\" lower-n-gen=\"-3\" upper-n-per=\"2\" upper-n-gen=\"-3\""), 1);
    EXPECT_EQ(xml2.count(u"lower-n-per=\"2\" lower-n-gen=\"-6\" upper-n-per=\"3\" upper-n-gen=\"-6\""), 1);

    delete again;
    delete score;
}

// A state change applied through the decision-2a path leaves every JiMS part
// on the same musical chronology, which is exactly what the interchange rule
// requires — so the changed score still exports.
TEST_F(MusicXml_JiMS_Tests, m9SATBScoreWideChangeKeepsOneSharedTimelineOnExport)
{
    MasterScore* score = ScoreRW::readScore(satbTemplatePath(), true);
    ASSERT_TRUE(score) << "the SATB (JiMStaff) template is not shipped";
    score->doLayout();
    ASSERT_EQ(score->nstaves(), 4u);

    Measure* m2 = measureNo(score, 2);
    ASSERT_TRUE(m2);
    String error;
    ASSERT_TRUE(jims::applyChangeToAllJimsParts(score, m2, { u"mode:1" }, error)) << error.toStdString();
    score->doLayout();

    const String out = exportToScratch(score, "export-m9-satb-mode-change.musicxml");
    const String xml = readAll(out);
    EXPECT_FALSE(xml.empty()) << "a score-wide change must leave the document exportable";
    EXPECT_EQ(xml.count(u"<jims:mode-rotation>5</jims:mode-rotation>"), 4)
        << "every one of the four parts must carry the change";
    // Every empty vocal staff retains its independent singer-range centre.
    EXPECT_EQ(xml.count(u"lower-n-per=\"3\" lower-n-gen=\"-5\" upper-n-per=\"4\" upper-n-gen=\"-5\""), 2);
    EXPECT_EQ(xml.count(u"lower-n-per=\"2\" lower-n-gen=\"-4\" upper-n-per=\"3\" upper-n-gen=\"-4\""), 2);
    EXPECT_EQ(xml.count(u"lower-n-per=\"1\" lower-n-gen=\"-3\" upper-n-per=\"2\" upper-n-gen=\"-3\""), 2);
    EXPECT_EQ(xml.count(u"lower-n-per=\"2\" lower-n-gen=\"-6\" upper-n-per=\"3\" upper-n-gen=\"-6\""), 2);

    delete score;
}

// The per-staff exclusion is exactly jims:extent; every song-wide divergence,
// including tonic-ambit, is still refused in both
// directions.
TEST_F(MusicXml_JiMS_Tests, m9SATBExtentOnlyDivergenceIsAcceptedAndMusicalDivergenceIsStillRefused)
{
    MasterScore* accepted = readJims("jims-multi-part-perstaff-differs.musicxml");
    ASSERT_TRUE(accepted) << "parts differing only in per-staff fields must import";
    delete accepted;

    EXPECT_FALSE(readJims("jims-multi-part-divergent-invalid.musicxml"))
        << "a musical-field divergence must still be refused on import";

    MasterScore* score = ScoreRW::readScore(satbTemplatePath(), true);
    ASSERT_TRUE(score) << "the SATB (JiMStaff) template is not shipped";
    score->doLayout();
    ASSERT_EQ(score->nstaves(), 4u);
    // Diverge one voice in a MUSICAL field: export must fail closed.
    StaffType* tenor = score->staff(2)->staffType(Fraction(0, 1));
    String json = tenor->jimsStateJson();
    ASSERT_TRUE(json.contains(u"\"generator_cents\":700.0"));
    json.replace(u"\"generator_cents\":700.0", u"\"generator_cents\":696.578");
    tenor->setJimsStateJson(json);
    score->setLayoutAll();
    score->doLayout();
    muse::io::Buffer buf;
    buf.open(muse::io::IODevice::WriteOnly);
    EXPECT_FALSE(saveXml(score, &buf)) << "a tuning divergence across parts must be refused on export";
    EXPECT_TRUE(buf.data().empty());

    delete score;
}

TEST_F(MusicXml_JiMS_Tests, MelodyPartDefaultsToSopranoAndDefaultIsOmittedOnExport)
{
    MasterScore* score = ScoreRW::readScore(satbTemplatePath(), true);
    ASSERT_TRUE(score);
    EXPECT_EQ(score->jimsMelodyPart(), jims::MelodyPart::Soprano);
    const String out = exportToScratch(score, "export-m10-melody-default.musicxml");
    const String xml = readAll(out);
    EXPECT_FALSE(xml.contains(u"<jims:melody-part>"));
    delete score;

    auto importXml = [](MasterScore* s, const muse::io::path_t& path) -> engraving::Err {
        return importMusicXml(s, path.toQString(), false);
    };
    MasterScore* reloaded = ScoreRW::readScore(out, true, importXml);
    ASSERT_TRUE(reloaded);
    EXPECT_EQ(reloaded->jimsMelodyPart(), jims::MelodyPart::Soprano);
    delete reloaded;
}

TEST_F(MusicXml_JiMS_Tests, MelodyPartTenorOverrideRoundTripsAndInvalidValueIsRefused)
{
    MasterScore* score = ScoreRW::readScore(satbTemplatePath(), true);
    ASSERT_TRUE(score);
    score->setJimsMelodyPart(jims::MelodyPart::Tenor);
    const String out = exportToScratch(score, "export-m10-melody-tenor.musicxml");
    String xml = readAll(out);
    EXPECT_TRUE(xml.contains(u"<jims:melody-part>tenor</jims:melody-part>"));
    delete score;

    auto importXml = [](MasterScore* s, const muse::io::path_t& path) -> engraving::Err {
        return importMusicXml(s, path.toQString(), false);
    };
    MasterScore* reloaded = ScoreRW::readScore(out, true, importXml);
    ASSERT_TRUE(reloaded);
    EXPECT_EQ(reloaded->jimsMelodyPart(), jims::MelodyPart::Tenor);
    delete reloaded;

    xml.replace(u"<jims:melody-part>tenor</jims:melody-part>",
                u"<jims:melody-part>descant</jims:melody-part>");
    const String invalid(u"jims-export-scratch/m10-melody-invalid.musicxml");
    muse::io::File file(invalid);
    ASSERT_TRUE(file.open(muse::io::IODevice::WriteOnly));
    file.write(xml.toUtf8());
    file.close();
    MasterScore* refused = ScoreRW::readScore(invalid, true, importXml);
    EXPECT_FALSE(refused);
    delete refused;
}
