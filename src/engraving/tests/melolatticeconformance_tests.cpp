// SPDX-License-Identifier: GPL-3.0-only
// MuseScore-Studio-CLA-applies
// Copyright (C) 2026 Jim Plamondon
// Coordinate authority and failure-atomicity regressions.
#include <gtest/gtest.h>
#include <QJsonDocument>
#include <QJsonObject>
#include "engraving/dom/chord.h"
#include "engraving/dom/factory.h"
#include "engraving/dom/input.h"
#include "engraving/dom/masterscore.h"
#include "engraving/dom/measure.h"
#include "engraving/dom/note.h"
#include "engraving/dom/segment.h"
#include "engraving/dom/staff.h"
#include "engraving/dom/stafftype.h"
#include "engraving/dom/tie.h"
#include "engraving/dom/utils.h"
#include "engraving/editing/undo.h"
#include "engraving/melo/melobridge.h"
#include "engraving/melo/melochangecontroller.h"
#include "engraving/melo/melotuningcontroller.h"
#include "utils/scorerw.h"
using namespace mu::engraving;
using muse::String;

static std::vector<Note*> notes(Score* s)
{
    std::vector<Note*> out;
    for (Segment* seg = s->firstSegment(SegmentType::ChordRest); seg; seg = seg->next1(SegmentType::ChordRest)) {
        auto* e = seg->element(0);
        if (e && e->isChord()) {
            for (auto* n : toChord(e)->notes()) {
                out.push_back(n);
            }
        }
    }
    return out;
}

static bool place(Note* n, int p, int g)
{
    melo::SoundingPitch v;
    if (!melo::noteSoundingPitch(n->staff()->staffTypeForElement(n)->meloStateJson(), p, g, v)) {
        return false;
    }
    n->setMeloPitch(p, g);
    int tpc = step2tpc(int(String(u"CDEFGAB").indexOf(muse::Char(v.step))), AccidentalVal(v.alter));
    n->setPitch(v.midiKey, tpc, tpc);
    n->setTuning(v.centsOffset);
    return true;
}

TEST(MeloLatticeConformanceTests, NoteValueTransferKeepsCoordinatesAcrossTunings) {
    auto* s=ScoreRW::readScore(u"jimstaff_data/m9-dense-voices.mscx");
    ASSERT_TRUE(s);
    Chord* c=notes(s)[0]->chord();
    int mismatches=0, checked=0;
    for (double gen : { 686.0, 700.0, 720.0 }) {
        melo::TuningController tune(s, 0);
        ASSERT_TRUE(tune.beginPreview());
        ASSERT_TRUE(tune.commit(gen));
        int genChecked=0, genMissing=0, genChanged=0, commonMismatch=0;
        for (int g=-17; g <= 17; ++g) {
            for (int p=-10; p <= 10; ++p) {
                auto* a=Factory::createNote(c);
                a->setParent(c);
                a->setTrack(0);
                if (!place(a, p, g) || a->pitch() < 48 || a->pitch() > 84) {
                    delete a;
                    continue;
                }
                auto* b=Factory::createNote(c);
                b->setParent(c);
                b->setTrack(0);
                ASSERT_TRUE(b->setNval(a->noteVal()));
                ++checked;
                ++genChecked;
                if (!b->hasMeloPitch() || b->meloNPer() != p || b->meloNGen() != g) {
                    if (mismatches < 8) {
                        std::cout << "TRANSFER_MISMATCH gen=" << gen << " source=(" << p << "," << g << ") target=(" << b->meloNPer() <<
                            "," << b->meloNGen() << ")\n";
                    }
                    ++mismatches;
                    if (!b->hasMeloPitch()) {
                        ++genMissing;
                    } else {
                        ++genChanged;
                    }
                    if (g >= -3 && g <= 3) {
                        ++commonMismatch;
                    }
                }
                delete b;
                delete a;
            }
        }
        std::cout << "TRANSFER_SUMMARY gen=" << gen << " checked=" << genChecked << " missing=" << genMissing << " changed=" <<
            genChanged << " common_mismatches=" << commonMismatch << "\n";
        ::testing::Test::RecordProperty("transfer_" + std::to_string(int(gen)),
                                        std::to_string(genChecked) + " checked; " + std::to_string(genMissing) + " missing; "
                                        + std::to_string(genChanged) + " changed; " + std::to_string(commonMismatch)
                                        + " common mismatches");
    }
    EXPECT_EQ(mismatches, 0) << "checked " << checked << " source-coordinate transfers";
    delete s;
}

static void addKnownRe0(double generator)
{
    auto* s=ScoreRW::readScore(u"jimstaff_data/m9-dense-voices.mscx");
    ASSERT_TRUE(s);
    melo::TuningController tune(s, 0);
    ASSERT_TRUE(tune.beginPreview());
    ASSERT_TRUE(tune.commit(generator));
    Note* source=notes(s)[0];
    ASSERT_TRUE(place(source, 0, 0));
    auto nv=source->noteVal();
    melo::SoundingPitch inverse;
    String error;
    bool recovered=melo::entryFromStandardPitch(source->staff()->staffTypeForElement(source)->meloStateJson(), 'D', 0, 4, inverse, &error);
    std::cout << "RE0_REVERSE generator=" << generator << " success=" << recovered << " error=" << error.toStdString() << "\n";
    ::testing::Test::RecordProperty("reverse_error", error.toStdString());
    s->startCmd(muse::TranslatableString::untranslatable("audit public add note"));
    Note* inserted=s->addNote(source->chord(), nv);
    s->endCmd();
    ASSERT_TRUE(inserted);
    EXPECT_TRUE(inserted->hasMeloPitch()) << "Public insertion accepted a note with no lattice coordinates";
    EXPECT_EQ(inserted->meloNPer(), 0);
    EXPECT_EQ(inserted->meloNGen(), 0);
    delete s;
}

TEST(MeloLatticeConformanceTests, PublicInsertionPreservesKnownRe0At720) {
    addKnownRe0(720);
}
TEST(MeloLatticeConformanceTests, PublicInsertionAt700Control) {
    addKnownRe0(700);
}

TEST(MeloLatticeConformanceTests, AmbiguousCoordinateLessInsertionIsNonMutating)
{
    auto* s = ScoreRW::readScore(u"jimstaff_data/m9-dense-voices.mscx");
    ASSERT_TRUE(s);
    melo::TuningController tune(s, 0);
    ASSERT_TRUE(tune.beginPreview());
    ASSERT_TRUE(tune.commit(720));
    Note* source = notes(s).front();
    const String state = source->staff()->staffTypeForElement(source)->meloStateJson();
    const size_t count = source->chord()->notes().size();
    const size_t undo = s->undoStack()->currentIndex();
    s->select(source, SelectType::SINGLE, 0);
    const auto segment = s->inputState().segment();
    const auto selected = s->selection().element();
    s->setPlayNote(false);
    s->setPlayChord(false);
    NoteVal value(62);
    value.tpc1 = value.tpc2 = step2tpc(1, AccidentalVal::NATURAL);
    s->startCmd(muse::TranslatableString::untranslatable("ambiguous input"));
    Note* inserted = s->addNote(source->chord(), value);
    EXPECT_EQ(inserted, nullptr);
    EXPECT_FALSE(s->playNote());
    EXPECT_FALSE(s->playChord());
    s->endCmd();
    EXPECT_EQ(source->chord()->notes().size(), count);
    EXPECT_EQ(source->staff()->staffTypeForElement(source)->meloStateJson(), state);
    EXPECT_EQ(s->undoStack()->currentIndex(), undo);
    EXPECT_EQ(s->inputState().segment(), segment);
    EXPECT_EQ(s->selection().element(), selected);
    delete s;
}

TEST(MeloLatticeConformanceTests, TransferEqualityDistinguishesCoincidentCoordinates)
{
    NoteVal first(62);
    NoteVal second = first;
    EXPECT_TRUE(first == second);
    first.hasMeloPitch = true;
    EXPECT_FALSE(first == second);
    second.hasMeloPitch = true;
    EXPECT_TRUE(first == second);
    second.meloNPer = -7;
    second.meloNGen = 12;
    EXPECT_FALSE(first == second);
}

TEST(MeloLatticeConformanceTests, AmbiguousPublicEntryPreservesInputAndScore)
{
    for (bool addToChord : { false, true }) {
        auto* s = ScoreRW::readScore(u"jimstaff_data/m9-dense-voices.mscx");
        ASSERT_TRUE(s);
        melo::TuningController tune(s, 0);
        ASSERT_TRUE(tune.beginPreview());
        ASSERT_TRUE(tune.commit(720));
        Note* source = notes(s).front();
        InputState input;
        input.setTrack(0);
        input.setSegment(source->chord()->segment());
        input.setLastSegment(source->chord()->segment());
        input.setDuration(TDuration(DurationType::V_QUARTER));
        const auto position = input.segment();
        const auto count = notes(s).size();
        const auto undo = s->undoStack()->currentIndex();
        NoteVal value(62);
        value.tpc1 = value.tpc2 = step2tpc(1, AccidentalVal::NATURAL);
        s->startCmd(muse::TranslatableString::untranslatable("ambiguous public entry"));
        EXPECT_EQ(s->addPitch(value, addToChord, &input), nullptr);
        EXPECT_EQ(input.segment(), position);
        EXPECT_EQ(input.lastSegment(), position);
        s->endCmd();
        EXPECT_EQ(notes(s).size(), count);
        EXPECT_EQ(s->undoStack()->currentIndex(), undo);
        delete s;
    }
}
