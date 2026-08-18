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

#include <climits>

#include "engraving/dom/masterscore.h"
#include "engraving/dom/measure.h"
#include "engraving/dom/note.h"
#include "engraving/dom/part.h"
#include "engraving/dom/segment.h"
#include "engraving/dom/staff.h"
#include "engraving/dom/stafftype.h"
#include "engraving/dom/system.h"
#include "engraving/dom/stafftypechange.h"
#include "engraving/jims/jimschange.h"
#include "engraving/style/style.h"

#include "importexport/musicxml/internal/import/importmusicxml.h"
#include "importexport/musicxml/internal/export/exportmusicxml.h"
#include "engraving/jims/jimsbridge.h"
#include "engraving/rw/xmlwriter.h"
#include "io/buffer.h"
#include "io/file.h"
#include "io/fileinfo.h"
#include "io/dir.h"
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

// ---------------------------------------------------------------------------
// Interchange hardening — native JiMS MusicXML EXPORT (converged FINAL 96%,
// 2026-08-17). The Kernel writes every jims:staff-state / jims:change element
// in full (bridge ops from jims PR 214); the fork places them verbatim, adds
// jims:pitch from each JiMS note's two stored integers, declares the V3
// namespace only when a JiMStaff is present, and fails closed.
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
    const String dir = ScoreRW::rootPath() + u"/../../../../build.release/jims-export-scratch";
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

TEST_F(MusicXml_JiMS_Tests, exportWritesV3AndRoundTripsThroughTheNativeImporter)
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
        EXPECT_TRUE(xml.contains(u"xmlns:jims=\"urn:jims:musicxml:3\"")) << file;
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
    EXPECT_TRUE(xml.contains(u"xmlns:jims=\"urn:jims:musicxml:3\""));
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
    EXPECT_FALSE(saveXml(score, &buf));
    EXPECT_TRUE(buf.data().empty());
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
