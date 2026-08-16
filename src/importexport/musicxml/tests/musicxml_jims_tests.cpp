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
// importer reads jims:staff-state (urn:jims:musicxml:1/2/3) and jims:pitch
// and builds the JiMStaff score directly — the same DOM the fixture converter
// tools/jims/enriched_to_jims_mscx.py produces. jims:change is never read;
// the Kernel validate op gates every state; an unrecognised JiMS namespace
// version is a fatal import error.

#include <gtest/gtest.h>

#include "engraving/dom/masterscore.h"
#include "engraving/dom/measure.h"
#include "engraving/dom/note.h"
#include "engraving/dom/part.h"
#include "engraving/dom/segment.h"
#include "engraving/dom/staff.h"
#include "engraving/dom/stafftype.h"
#include "engraving/dom/stafftypechange.h"
#include "engraving/jims/jimschange.h"
#include "engraving/style/style.h"

#include "importexport/musicxml/internal/import/importmusicxml.h"
#include "engraving/tests/utils/scorerw.h"

using namespace mu;
using namespace mu::engraving;
using namespace mu::iex::musicxml;

static const String JIMS_DATA_DIR(u"data/jims/");

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
// key order, no spaces, tonic_extent last.
static const char* KEY_MODE_STATE_1
    = "{\"scale\":[\"M2\",\"m2\",\"M2\",\"M2\",\"M2\",\"m2\",\"M2\"],\"collection_rotation\":0,\"mode_rotation\":0,"
      "\"generator_cents\":700.0,\"period_cents\":1200.0,\"embedding\":{\"large_steps\":5,\"small_steps\":2},"
      "\"extent\":{\"lower_do_register\":4,\"period_count\":1},\"reference\":{\"reference-pitch\":{\"key_number\":62}},"
      "\"tonic_extent\":\"tonic-bounded\"}";
static const char* KEY_MODE_STATE_2
    = "{\"scale\":[\"M2\",\"m2\",\"M2\",\"M2\",\"M2\",\"m2\",\"M2\"],\"collection_rotation\":0,\"mode_rotation\":5,"
      "\"generator_cents\":700.0,\"period_cents\":1200.0,\"embedding\":{\"large_steps\":5,\"small_steps\":2},"
      "\"extent\":{\"lower_do_register\":4,\"period_count\":1},\"reference\":{\"reference-pitch\":{\"key_number\":53}},"
      "\"tonic_extent\":\"tonic-bounded\"}";

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
    EXPECT_EQ(st->jimsStateJson(), String::fromUtf8(KEY_MODE_STATE_1));
    EXPECT_TRUE(st->jimsJiLines());
    EXPECT_EQ(st->jimsTonicExtent(), String(u"tonic-bounded"));
    // The change measure carries the complete second state (never derived from jims:change).
    Measure* m2 = measureNo(score, 2);
    ASSERT_TRUE(m2);
    const StaffTypeChange* stc = jims::changeCarrier(m2, 0);
    ASSERT_TRUE(stc);
    ASSERT_TRUE(stc->staffType());
    EXPECT_EQ(stc->staffType()->jimsStateJson(), String::fromUtf8(KEY_MODE_STATE_2));
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

TEST_F(MusicXml_JiMS_Tests, everyReferenceFormTranscribesVerbatimAndOlderProfilesReadAsNone)
{
    struct Case { const char* file; const char* reference; };
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
        EXPECT_FALSE(st->jimsStateJson().contains(u"\"tonic_extent\":\"\"")) << c.file;
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
    // V1 (no tonic-extent in the profile): the field is simply absent.
    MasterScore* v1 = readJims("jims-v1-collision.musicxml");
    ASSERT_TRUE(v1);
    EXPECT_FALSE(staffTypeAtStart(v1)->jimsStateJson().contains(u"tonic_extent"));
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
    struct Case { const char* file; int changeMeasure; const char* marker; };
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
