/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 *
 * MuseScore Studio
 * Music Composition & Notation
 *
 * Copyright (C) 2021 MuseScore Limited and others
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

#include <gtest/gtest.h>
#include <QFile>
#include <QTemporaryDir>

#include "io/file.h"

#include "engraving/tests/utils/scorerw.h"
#include "engraving/tests/utils/scorecomp.h"

#include "engraving/dom/masterscore.h"
#include "engraving/dom/excerpt.h"
#include "engraving/dom/note.h"
#include "engraving/dom/segment.h"
#include "engraving/dom/staff.h"
#include "engraving/melo/melochangecontroller.h"
#include "engraving/dom/part.h"
#include "engraving/dom/instrtemplate.h"
#include "engraving/dom/stafftype.h"

#include "modularity/ioc.h"
#include "importexport/mei/imeiconfiguration.h"
#include "importexport/mei/internal/meireader.h"
#include "importexport/mei/internal/meiwriter.h"

using namespace mu::engraving;

static const String MEI_DIR(u"data/");

////////////////////////////////////////////////////////////////
// Set to true to re-generate the MuseScore reference test files
#define BUILD_MSCORE_REF_FILE false
////////////////////////////////////////////////////////////////

namespace mu::iex::mei {
class Mei_Tests : public ::testing::Test
{
public:
    void meiReadTest(const char* file);

    inline static bool s_generateReferenceFile = BUILD_MSCORE_REF_FILE;
};

void Mei_Tests::meiReadTest(const char* file)
{
    String fileName = String::fromUtf8(file);

    auto importFunc = [](MasterScore* score, const muse::io::path_t& path) -> Err {
        MeiReader meiReader(nullptr);
        return meiReader.import(score, path);
    };

    auto exportFunc = [](Score* score, const muse::io::path_t& path) -> Err {
        MeiWriter meiWriter;
        return meiWriter.writeScore(score, path);
    };

    // Load the .mei file
    MasterScore* score = ScoreRW::readScore(MEI_DIR + fileName + u".mei", false, importFunc);
    EXPECT_TRUE(score);

    // Flag to be turned on to generate the test reference .mscx files from the .mei
    if (s_generateReferenceFile) {
        bool res = ScoreRW::saveScore(score, ScoreRW::rootPath() + u"/" + MEI_DIR + fileName + u".mscx");
        EXPECT_TRUE(res);
        return;
    }

    // Compare with the reference MuseScore file
    EXPECT_TRUE(ScoreComp::saveCompareScore(score, fileName + u".mscx", MEI_DIR + fileName + u".mscx"));

    // Save the .mei file for round trip testing
    bool output = ScoreRW::saveScore(score,  fileName + u".test.mei", exportFunc);
    EXPECT_TRUE(output);
    delete score;

    // Compare the mei files
    EXPECT_TRUE(ScoreComp::compareFiles(fileName + u".test.mei", ScoreRW::rootPath() + u"/" + MEI_DIR + fileName + u".mei"));
}

// MeloPresto MEI (mei-jims profile) focused round trip: typed state import,
// native carriers, and extMeta regeneration on export.
TEST_F(Mei_Tests, mei_melo_concert_do_roundtrip) {
    auto importFunc = [](MasterScore* score, const muse::io::path_t& path) -> Err {
        MeiReader reader(nullptr);
        return reader.import(score, path);
    };
    auto exportFunc = [](Score* score, const muse::io::path_t& path) -> Err {
        MeiWriter writer;
        return writer.writeScore(score, path);
    };
    // Reuse the exact default-reference native passage. The synthetic MEI
    // fixture intentionally has a rounded 261.63 Hz reference, which cannot
    // be converted losslessly to concert C and must remain a refusal case.
    std::unique_ptr<MasterScore> score(ScoreRW::readScore(u"../../../engraving/tests/jimstaff_data/collision.mscx"));
    ASSERT_TRUE(score);
    score->parts().front()->setInstrument(Instrument::fromTemplate(searchTemplate(u"bb-clarinet")));
    String error;
    ASSERT_TRUE(melo::applyChange(score.get(), 0, score->firstMeasure(), u"notation:concert", error)) << error.toStdString();
    score->rebuildMidiMapping();
    ASSERT_TRUE(ScoreRW::saveScore(score.get(), u"concert-do.test.mei", exportFunc));
    std::unique_ptr<MasterScore> reopened(ScoreRW::readScore(u"concert-do.test.mei", true, importFunc));
    ASSERT_TRUE(reopened);
    melo::StateChangeOptions options;
    ASSERT_TRUE(melo::stateChangeOptions(reopened->staff(0)->staffType(Fraction(0, 1))->meloStateJson(), options));
    EXPECT_TRUE(options.concertC);
    auto notes = [](Score* s) {
        std::vector<std::tuple<int, int, int, int> > result;
        for (const Segment* seg = s->firstSegment(SegmentType::ChordRest); seg; seg = seg->next1(SegmentType::ChordRest)) {
            const EngravingItem* item = seg->element(0);
            if (item && item->isChord()) {
                for (const Note* note : toChord(item)->notes()) {
                    result.emplace_back(note->pitch(), note->tpc2(), note->meloNPer(), note->meloNGen());
                }
            }
        }
        return result;
    };
    EXPECT_EQ(notes(reopened.get()), notes(score.get()));
}

TEST_F(Mei_Tests, mei_melo_roundtrip_01) {
    auto importFunc = [](MasterScore* score, const muse::io::path_t& path) -> Err {
        MeiReader meiReader(nullptr);
        return meiReader.import(score, path);
    };
    auto exportFunc = [](Score* score, const muse::io::path_t& path) -> Err {
        MeiWriter meiWriter;
        return meiWriter.writeScore(score, path);
    };

    MasterScore* score = ScoreRW::readScore(MEI_DIR + u"jims/jims-synthetic.mei", false, importFunc);
    ASSERT_TRUE(score);

    // Typed staff state: MeloPresto staff type at tick 0 plus two later states.
    const Staff* staff = score->staff(0);
    ASSERT_TRUE(staff);
    const StaffType* base = staff->staffType(Fraction(0, 1));
    ASSERT_TRUE(base && base->isMelo());
    EXPECT_FALSE(base->meloStateJson().isEmpty());
    EXPECT_EQ(base->meloTonicAmbit(), String(u"tonic-bounded"));

    // Note identities and melody part.
    int pitched = 0;
    int identified = 0;
    for (const Segment* seg = score->firstSegment(SegmentType::ChordRest); seg;
         seg = seg->next1(SegmentType::ChordRest)) {
        for (const EngravingItem* item : seg->elist()) {
            if (!item || !item->isChord()) {
                continue;
            }
            for (const Note* note : toChord(item)->notes()) {
                ++pitched;
                if (note->hasMeloPitch()) {
                    ++identified;
                }
            }
        }
    }
    EXPECT_GT(pitched, 0);
    EXPECT_EQ(pitched, identified);
    EXPECT_EQ(score->meloMelodyPart(), engraving::melo::MelodyPart::Soprano);
    ASSERT_EQ(score->meloProvenance().resources.size(), size_t(1));
    EXPECT_TRUE(score->meloProvenance().strictFallback);
    EXPECT_EQ(staff->meloTuningTrajectories().size(), size_t(1));

    // Export regenerates the typed carriers. The harness never rebuilds the
    // MIDI mapping the way the application does on load; writeInstrDef needs
    // playback channels to exist.
    score->masterScore()->rebuildMidiMapping();
    bool output = ScoreRW::saveScore(score, u"jims-synthetic.test.mei", exportFunc);
    EXPECT_TRUE(output);
    muse::io::File out(muse::io::path_t(u"jims-synthetic.test.mei"));
    ASSERT_TRUE(out.open(muse::io::IODevice::ReadOnly));
    const muse::ByteArray meiBytes = out.readAll();
    const String mei = String::fromUtf8(meiBytes.constChar());
    out.close();
    EXPECT_TRUE(mei.contains(u"jm:record"));
    EXPECT_TRUE(mei.contains(u"jims-tonal-state"));
    EXPECT_TRUE(mei.contains(u"jims-chord-name"));
    EXPECT_TRUE(mei.contains(u"jims-tonic-ambit"));
    EXPECT_TRUE(mei.contains(u"jims-melody-part"));
    EXPECT_TRUE(mei.contains(u"<ambitus>"));
    EXPECT_TRUE(mei.contains(u"jims:tuning-trajectory"));
    delete score;
}

TEST_F(Mei_Tests, mei_accid_01) {
    meiReadTest("accid-01");
}

TEST_F(Mei_Tests, mei_accid_02) {
    meiReadTest("accid-02");
}

TEST_F(Mei_Tests, mei_arpeg_01) {
    meiReadTest("arpeg-01");
}

TEST_F(Mei_Tests, mei_artic_01) {
    meiReadTest("artic-01");
}

TEST_F(Mei_Tests, mei_artic_02) {
    meiReadTest("artic-02");
}

TEST_F(Mei_Tests, mei_beam_01) {
    meiReadTest("beam-01");
}

TEST_F(Mei_Tests, mei_beam_02) {
    meiReadTest("beam-02");
}

TEST_F(Mei_Tests, mei_beam_03) {
    meiReadTest("beam-03");
}

TEST_F(Mei_Tests, mei_breaks_01) {
    meiReadTest("breaks-01");
}

TEST_F(Mei_Tests, mei_breath_01) {
    meiReadTest("breath-01");
}

TEST_F(Mei_Tests, mei_btrem_01) {
    meiReadTest("btrem-01");
}

TEST_F(Mei_Tests, mei_chord_label_01) {
    meiReadTest("chord-label-01");
}

TEST_F(Mei_Tests, mei_clef_01) {
    meiReadTest("clef-01");
}

TEST_F(Mei_Tests, mei_color_01) {
    meiReadTest("color-01");
}

TEST_F(Mei_Tests, mei_cross_staff_01) {
    meiReadTest("cross-staff-01");
}

TEST_F(Mei_Tests, mei_dir_01) {
    meiReadTest("dir-01");
}

TEST_F(Mei_Tests, mei_dynamic_01) {
    meiReadTest("dynamic-01");
}

TEST_F(Mei_Tests, mei_ending_01) {
    meiReadTest("ending-01");
}

TEST_F(Mei_Tests, mei_fermata_01) {
    meiReadTest("fermata-01");
}

TEST_F(Mei_Tests, mei_fig_bass_01) {
    meiReadTest("fig-bass-01");
}

TEST_F(Mei_Tests, mei_fingering_01) {
    meiReadTest("fingering-01");
}

TEST_F(Mei_Tests, mei_ftrem_01) {
    meiReadTest("ftrem-01");
}

TEST_F(Mei_Tests, mei_glisss_01) {
    meiReadTest("gliss-01");
}

TEST_F(Mei_Tests, mei_gracenote_01) {
    meiReadTest("gracenote-01");
}

TEST_F(Mei_Tests, mei_gracenote_02) {
    meiReadTest("gracenote-02");
}

TEST_F(Mei_Tests, mei_hairpin_01) {
    meiReadTest("hairpin-01");
}

TEST_F(Mei_Tests, mei_harp_01) {
    meiReadTest("harp-01");
}

TEST_F(Mei_Tests, mei_harm_tstamp_01) {
    meiReadTest("harm-tstamp-01");
}

TEST_F(Mei_Tests, mei_jump_01) {
    meiReadTest("jump-01");
}

TEST_F(Mei_Tests, mei_jump_02) {
    meiReadTest("jump-02");
}

TEST_F(Mei_Tests, mei_key_signature_01) {
    meiReadTest("key-signature-01");
}

TEST_F(Mei_Tests, mei_midi_01) {
    meiReadTest("midi-01");
}

TEST_F(Mei_Tests, mei_label_01) {
    meiReadTest("label-01");
}

TEST_F(Mei_Tests, laissez_vibrer_01) {
    meiReadTest("laissez-vibrer-01");
}

TEST_F(Mei_Tests, mei_lyric_01) {
    meiReadTest("lyric-01");
}

TEST_F(Mei_Tests, mei_lyric_02) {
    meiReadTest("lyric-02");
}

TEST_F(Mei_Tests, mei_lyric_03) {
    meiReadTest("lyric-03");
}

TEST_F(Mei_Tests, mei_lyric_04) {
    meiReadTest("lyric-04");
}

TEST_F(Mei_Tests, mei_measure_01) {
    meiReadTest("measure-01");
}

TEST_F(Mei_Tests, mei_measure_02) {
    meiReadTest("measure-02");
}

TEST_F(Mei_Tests, mei_mrpt_01) {
    meiReadTest("measure-repeat-01");
}

TEST_F(Mei_Tests, mei_metadata_01) {
    meiReadTest("metadata-01");
}

TEST_F(Mei_Tests, mei_mordent_01) {
    meiReadTest("mordent-01");
}

TEST_F(Mei_Tests, mei_octave_01) {
    meiReadTest("octave-01");
}

TEST_F(Mei_Tests, mei_ornam_01) {
    meiReadTest("ornam-01");
}

TEST_F(Mei_Tests, mei_page_head_01) {
    meiReadTest("page-head-01");
}

TEST_F(Mei_Tests, mei_page_head_02) {
    meiReadTest("page-head-02");
}

TEST_F(Mei_Tests, mei_pedal_01) {
    meiReadTest("pedal-01");
}

TEST_F(Mei_Tests, mei_reh_01) {
    meiReadTest("reh-01");
}

TEST_F(Mei_Tests, mei_roman_numeral_01) {
    meiReadTest("roman-numeral-01");
}

TEST_F(Mei_Tests, mei_score_01) {
    meiReadTest("score-01");
}

TEST_F(Mei_Tests, mei_score_02) {
    meiReadTest("score-02");
}

TEST_F(Mei_Tests, mei_score_03) {
    meiReadTest("score-03");
}

TEST_F(Mei_Tests, mei_slur_01) {
    meiReadTest("slur-01");
}

TEST_F(Mei_Tests, mei_slur_02) {
    meiReadTest("slur-02");
}

TEST_F(Mei_Tests, mei_stem_01) {
    meiReadTest("stem-01");
}

TEST_F(Mei_Tests, mei_tempo_01) {
    meiReadTest("tempo-01");
}

TEST_F(Mei_Tests, mei_tie_01) {
    meiReadTest("tie-01");
}

TEST_F(Mei_Tests, mei_time_signature_01) {
    meiReadTest("time-signature-01");
}

TEST_F(Mei_Tests, mei_time_signature_02) {
    meiReadTest("time-signature-02");
}

TEST_F(Mei_Tests, mei_transpose_01) {
    meiReadTest("transpose-01");
}

TEST_F(Mei_Tests, mei_trill_01) {
    meiReadTest("trill-01");
}

TEST_F(Mei_Tests, mei_tuplet_01) {
    meiReadTest("tuplet-01");
}

TEST_F(Mei_Tests, mei_tuplet_02) {
    meiReadTest("tuplet-02");
}

TEST_F(Mei_Tests, mei_tuplet_03) {
    meiReadTest("tuplet-03");
}
TEST_F(Mei_Tests, missingStateIsExplainedAndDoesNotLeakIntoTheNextFileError)
{
    QFile source(QString::fromUtf8(iex_mei_tests_DATA_ROOT) + "/data/jims/jims-synthetic.mei");
    ASSERT_TRUE(source.open(QIODevice::ReadOnly));
    QByteArray xml = source.readAll();
    int start = xml.indexOf("<extMeta>");
    int end = xml.indexOf("</extMeta>", start);
    ASSERT_GE(start, 0);
    ASSERT_GT(end, start);
    xml.remove(start, end + int(sizeof("</extMeta>") - 1) - start);
    QTemporaryDir directory;
    ASSERT_TRUE(directory.isValid());
    QString path = directory.filePath("missing-state.mei");
    QFile stripped(path);
    ASSERT_TRUE(stripped.open(QIODevice::WriteOnly));
    stripped.write(xml);
    stripped.close();
    muse::Ret received;
    auto import = [&received](MasterScore* score, const muse::io::path_t& input) {
        MeiReader reader(nullptr);
        received = reader.read(score, input);
        return received ? Err::NoError : Err::FileCriticallyCorrupted;
    };
    std::unique_ptr<MasterScore> refused(ScoreRW::readScore(String::fromQString(path), true, import));
    EXPECT_FALSE(refused);
    EXPECT_NE(received.text().find("required state record is missing"), std::string::npos);
    MeiReader reader(nullptr);
    muse::Ret missing = reader.read(nullptr, directory.filePath("does-not-exist.mei"));
    EXPECT_FALSE(missing);
    EXPECT_EQ(missing.text().find("required state record is missing"), std::string::npos);
}

TEST_F(Mei_Tests, unrelatedTypeTokenDoesNotClaimTheFileUsesTheProfile)
{
    QFile source(QString::fromUtf8(iex_mei_tests_DATA_ROOT) + "/data/label-01.mei");
    ASSERT_TRUE(source.open(QIODevice::ReadOnly));
    QByteArray xml = source.readAll();
    xml.replace("<score>", "<score type=\"unrelated-jims-tag\">");
    QTemporaryDir directory;
    QFile output(directory.filePath("unrelated.mei"));
    ASSERT_TRUE(output.open(QIODevice::WriteOnly));
    output.write(xml);
    output.close();
    auto import = [](MasterScore* score, const muse::io::path_t& input) {
        MeiReader reader(nullptr);
        return reader.import(score, input);
    };
    std::unique_ptr<MasterScore> score(ScoreRW::readScore(String::fromQString(output.fileName()), true, import));
    EXPECT_TRUE(score);
}

TEST_F(Mei_Tests, mei_shared_jammer_change_preserves_acoustic_mapping)
{
    std::unique_ptr<MasterScore> score(ScoreRW::readScore(u"../../../engraving/tests/jimstaff_data/m9-satb-hymn.mscx"));
    ASSERT_TRUE(score);
    score->parts().front()->instrument()->setId(u"melo-jammer");
    String error;
    for (staff_idx_t i = 1; i < score->nstaves(); ++i) {
        ASSERT_TRUE(melo::applyChange(score.get(), i, score->firstMeasure(), u"notation:concert", error)) << error.toStdString();
    }
    Measure* change = score->firstMeasure()->nextMeasure();
    ASSERT_TRUE(change);
    const Fraction tick = change->tick();
    ASSERT_TRUE(melo::applyChangeToAllMeloParts(score.get(), change, { u"key:-1:3" }, error)) << error.toStdString();
    score->rebuildMidiMapping();
    auto exportFunc = [](Score* source, const muse::io::path_t& path) -> Err {
        MeiWriter writer;
        return writer.writeScore(source, path);
    };
    ASSERT_TRUE(ScoreRW::saveScore(score.get(), u"shared-jammer-acoustic.test.mei", exportFunc));
    auto importFunc = [](MasterScore* target, const muse::io::path_t& path) -> Err {
        MeiReader reader(nullptr);
        return reader.import(target, path);
    };
    std::unique_ptr<MasterScore> again(ScoreRW::readScore(u"shared-jammer-acoustic.test.mei", true, importFunc));
    ASSERT_TRUE(again);
    ASSERT_EQ(again->nstaves(), score->nstaves());
    for (staff_idx_t i = 0; i < score->nstaves(); ++i) {
        for (const Fraction at : { Fraction(0, 1), tick }) {
            EXPECT_EQ(again->staff(i)->staffType(at)->meloStateJson(), score->staff(i)->staffType(at)->meloStateJson());
        }
    }
}
}
