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

// JiMStaff Milestone 6 — editing workflow (owner decision 1a, 2026-08-16):
// keyboard pitch editing moves on the lattice through the Kernel; every
// copy/paste/clone keeps the lattice identity; key/mode/scale changes are
// authored through a controller that transports Kernel states into the
// StaffTypeChange carrier. The fork never computes a step, a tonic, a
// reference shift, or a rotation itself.

#include <gtest/gtest.h>

#include <QMimeData>

#include "engraving/internal/qmimedataadapter.h"
#include <cstdlib>
#include <fstream>

#include "engraving/dom/accidental.h"
#include "engraving/dom/chord.h"
#include "engraving/dom/factory.h"
#include "engraving/dom/input.h"
#include "engraving/dom/utils.h"
#include "engraving/dom/masterscore.h"
#include "engraving/dom/measure.h"
#include "engraving/dom/note.h"
#include "engraving/dom/segment.h"
#include "engraving/dom/staff.h"
#include "engraving/dom/text.h"
#include "engraving/dom/box.h"
#include "engraving/dom/stafftype.h"
#include "engraving/dom/stafftypechange.h"
#include "engraving/jims/jimsbridge.h"
#include "engraving/jims/jimschange.h"
#include "engraving/jims/jimschangecontroller.h"

#include "utils/scorerw.h"

using namespace mu::engraving;

namespace {
Measure* measureNo(Score* score, int n)
{
    Measure* m = score->firstMeasure();
    for (int i = 1; m && i < n; ++i) {
        m = m->nextMeasure();
    }
    return m;
}

std::vector<Note*> jimsNotes(Score* score, staff_idx_t staffIdx = 0)
{
    std::vector<Note*> out;
    for (Measure* m = score->firstMeasure(); m; m = m->nextMeasure()) {
        for (Segment* s = m->first(SegmentType::ChordRest); s; s = s->next(SegmentType::ChordRest)) {
            for (voice_idx_t v = 0; v < VOICES; ++v) {
                EngravingItem* e = s->element(staffIdx * VOICES + v);
                if (e && e->isChord()) {
                    for (Note* n : toChord(e)->notes()) {
                        out.push_back(n);
                    }
                }
            }
        }
    }
    return out;
}

const StaffType* jimsStaffType(Score* score)
{
    return score->staff(0)->staffType(Fraction(0, 1));
}

int compatPitch(const jims::PitchHit& hit)
{
    static const muse::String letters(u"CDEFGAB");
    const int step = int(letters.indexOf(muse::Char(hit.step)));
    static const int stepPitches[7] = { 0, 2, 4, 5, 7, 9, 11 };
    return (hit.octave + 1) * 12 + stepPitches[step] + hit.alter;
}
}

// Binding Requirement 2: plain Up/Down = nearest realizable lattice pitch
// (CHROMATIC), Alt+Shift = collection member (DIATONIC), Ctrl = one period
// (OCTAVE) — every answer from the Kernel step_pitch op, identity and
// compatibility pitch changed together, undoable.
TEST(JiMStaffTests, m6KeyboardStepsMoveOnTheLatticeThroughTheKernel)
{
    Score* score = ScoreRW::readScore(u"jimstaff_data/collision.mscx");
    ASSERT_TRUE(score);
    score->doLayout();
    const StaffType* st = jimsStaffType(score);
    ASSERT_TRUE(st && st->isJiMS());
    auto notes = jimsNotes(score);
    ASSERT_FALSE(notes.empty());
    Note* n = notes.front();
    ASSERT_TRUE(n->hasJimsPitch());

    struct Case { UpDownMode mode; bool up; const char* domain; };
    const Case cases[] = {
        { UpDownMode::CHROMATIC, true, "lattice" },
        { UpDownMode::CHROMATIC, false, "lattice" },
        { UpDownMode::DIATONIC, true, "collection" },
        { UpDownMode::DIATONIC, false, "collection" },
        { UpDownMode::OCTAVE, true, "period" },
        { UpDownMode::OCTAVE, false, "period" },
    };
    for (const Case& c : cases) {
        const int nPer0 = n->jimsNPer(), nGen0 = n->jimsNGen(), pitch0 = n->pitch();
        jims::PitchHit hit;
        ASSERT_TRUE(jims::stepPitch(st->jimsStateJson(), nPer0, nGen0, c.up, c.domain, hit)) << c.domain;
        score->select(n);
        score->startCmd(TranslatableString::untranslatable("M6 test step"));
        score->upDown(c.up, c.mode);
        score->endCmd();
        score->doLayout();
        EXPECT_EQ(n->jimsNPer(), hit.nPer) << c.domain << (c.up ? " up" : " down");
        EXPECT_EQ(n->jimsNGen(), hit.nGen) << c.domain << (c.up ? " up" : " down");
        EXPECT_EQ(n->pitch(), compatPitch(hit)) << "compatibility pitch follows the Kernel spelling";
        EXPECT_NE(n->pitch(), pitch0) << "a step must move";
        score->undoRedo(true, nullptr);
        score->doLayout();
        EXPECT_EQ(n->jimsNPer(), nPer0) << "undo restores the identity";
        EXPECT_EQ(n->jimsNGen(), nGen0);
        EXPECT_EQ(n->pitch(), pitch0);
        score->undoRedo(false, nullptr);
        score->doLayout();
        EXPECT_EQ(n->jimsNPer(), hit.nPer) << "redo re-applies";
        score->undoRedo(true, nullptr);
        score->doLayout();
    }
    // Period steps preserve the class; collection steps land on members.
    {
        jims::PitchHit hit;
        ASSERT_TRUE(jims::stepPitch(st->jimsStateJson(), n->jimsNPer(), n->jimsNGen(), true, "period", hit));
        EXPECT_EQ(hit.nGen, n->jimsNGen());
        EXPECT_EQ(hit.nPer, n->jimsNPer() + 1);
    }
    delete score;
}

// Binding Requirement 3 (Decision 1a): a copied/cloned note keeps its
// lattice identity and forgets its cached cents; a range copy/paste keeps
// every identity in order.
TEST(JiMStaffTests, m6CopiedAndPastedNotesKeepTheirLatticeIdentity)
{
    Score* score = ScoreRW::readScore(u"jimstaff_data/collision.mscx");
    ASSERT_TRUE(score);
    score->doLayout();
    auto notes = jimsNotes(score);
    ASSERT_FALSE(notes.empty());
    Note* src = notes.front();
    ASSERT_TRUE(src->hasJimsPitch());
    // Direct clone through the copy constructor (Factory::copyNote).
    Note* copy = Factory::copyNote(*src, false);
    ASSERT_TRUE(copy);
    EXPECT_TRUE(copy->hasJimsPitch()) << "the copy constructor must carry the identity";
    EXPECT_EQ(copy->jimsNPer(), src->jimsNPer());
    EXPECT_EQ(copy->jimsNGen(), src->jimsNGen());
    EXPECT_FALSE(copy->jimsCentsValid()) << "derived cents are re-derived in the destination, never carried";
    delete copy;

    // Range copy of measure 1, paste at measure 3.
    Measure* m1 = measureNo(score, 1);
    Measure* m3 = measureNo(score, 3);
    ASSERT_TRUE(m1 && m3);
    std::vector<std::pair<int, int> > sourceIds;
    for (Segment* s = m1->first(SegmentType::ChordRest); s; s = s->next(SegmentType::ChordRest)) {
        if (EngravingItem* e = s->element(0); e && e->isChord()) {
            for (Note* nn : toChord(e)->notes()) {
                sourceIds.push_back({ nn->jimsNPer(), nn->jimsNGen() });
            }
        }
    }
    ASSERT_FALSE(sourceIds.empty());
    score->select(m1);
    ASSERT_TRUE(score->selection().canCopy());
    QMimeData* mimeData = new QMimeData;
    mimeData->setData(score->selection().mimeType(), score->selection().mimeData().toQByteArray());
    ASSERT_TRUE(m3->first(SegmentType::ChordRest)->element(0));
    score->select(m3->first(SegmentType::ChordRest)->element(0));
    score->startCmd(TranslatableString::untranslatable("M6 test paste"));
    QMimeDataAdapter ma(mimeData);
    score->cmdPaste(&ma, 0);
    score->endCmd();
    score->doLayout();
    std::vector<std::pair<int, int> > pastedIds;
    for (Segment* s = m3->first(SegmentType::ChordRest); s; s = s->next(SegmentType::ChordRest)) {
        if (EngravingItem* e = s->element(0); e && e->isChord()) {
            for (Note* nn : toChord(e)->notes()) {
                pastedIds.push_back({ nn->jimsNPer(), nn->jimsNGen() });
            }
        }
    }
    ASSERT_GE(pastedIds.size(), sourceIds.size());
    for (size_t i = 0; i < sourceIds.size(); ++i) {
        EXPECT_EQ(pastedIds[i], sourceIds[i]) << "pasted note " << i << " lost its identity";
    }
    // Undo the paste: measure 3 returns to its prior content, identities intact.
    score->undoRedo(true, nullptr);
    score->doLayout();
    for (Note* nn : jimsNotes(score)) {
        EXPECT_TRUE(nn->hasJimsPitch());
    }
    delete mimeData;
    delete score;
}

// Binding Requirement 4: the change controller inserts, compounds, and
// removes the measure's carrier from Kernel-returned states only.
TEST(JiMStaffTests, m6ChangeControllerAuthorsCarriersFromKernelStates)
{
    Score* score = ScoreRW::readScore(u"jimstaff_data/collision.mscx");
    ASSERT_TRUE(score);
    score->doLayout();
    Measure* m2 = measureNo(score, 2);
    ASSERT_TRUE(m2);
    ASSERT_FALSE(jims::changeCarrier(m2, 0));

    muse::String base;
    ASSERT_TRUE(jims::effectiveState(score, 0, m2, base));
    jims::StateChangeOptions options;
    ASSERT_TRUE(jims::changeOptions(score, 0, m2, options));
    ASSERT_EQ(options.tonics.size(), 7u);
    EXPECT_EQ(options.tonics[0].label, muse::String(u"Do"));
    EXPECT_TRUE(options.tonics[0].current);
    EXPECT_EQ(options.tonics[5].label, muse::String(u"La"));
    EXPECT_EQ(options.tonics[5].id, muse::String(u"mode:1"));
    EXPECT_FALSE(options.referenceBound);
    muse::String why;
    EXPECT_TRUE(jims::canInsertChange(score, 0, m2, why)) << why.toStdString();

    // Mode change Do -> La: the carrier's state is exactly the Kernel's answer.
    muse::String expected, error;
    ASSERT_TRUE(jims::applyStateChange(base, u"mode:1", expected, error)) << error.toStdString();
    ASSERT_TRUE(jims::applyChange(score, 0, m2, u"mode:1", error)) << error.toStdString();
    score->doLayout();
    const StaffTypeChange* stc = jims::changeCarrier(m2, 0);
    ASSERT_TRUE(stc);
    EXPECT_EQ(stc->staffType()->jimsStateJson(), expected);
    EXPECT_TRUE(stc->staffType()->jimsStateJson().contains(u"\"mode_rotation\":5"));
    jims::ChangeIndicator model;
    ASSERT_TRUE(jims::midSystemChangeIndicator(m2, 0, model));
    ASSERT_EQ(model.kinds.size(), 1u);
    EXPECT_EQ(model.kinds[0], muse::String(u"mode"));

    // One undo step removes the carrier; redo restores it.
    score->undoRedo(true, nullptr);
    score->doLayout();
    EXPECT_FALSE(jims::changeCarrier(m2, 0));
    score->undoRedo(false, nullptr);
    score->doLayout();
    ASSERT_TRUE(jims::changeCarrier(m2, 0));

    // A key change needs a bound reference: refused, then bind-first, then key.
    EXPECT_FALSE(jims::applyChange(score, 0, m2, u"key:-1:3", error));
    ASSERT_TRUE(jims::applyChange(score, 0, m2, u"bind:reference-pitch:62", error)) << error.toStdString();
    ASSERT_TRUE(jims::applyChange(score, 0, m2, u"key:-1:3", error)) << error.toStdString();
    score->doLayout();
    stc = jims::changeCarrier(m2, 0);
    ASSERT_TRUE(stc);
    EXPECT_TRUE(stc->staffType()->jimsStateJson().contains(u"\"key_number\":53"));
    EXPECT_TRUE(stc->staffType()->jimsStateJson().contains(u"\"mode_rotation\":5"));
    // The base staff type is untouched: the change is carried at the measure only.
    EXPECT_TRUE(jimsStaffType(score)->jimsStateJson().contains(u"\"mode_rotation\":0"));
    // Compounded from the carrier: options now report La as current.
    ASSERT_TRUE(jims::changeOptions(score, 0, m2, options));
    EXPECT_TRUE(options.tonics[5].current);
    EXPECT_TRUE(options.referenceBound);
    // The indicator is the owner's worked example only if the base state
    // also binds 62 — here the base is reference-none, so the Kernel reports
    // the reference forms as unrecoverable; the controller must still have
    // produced exactly the states it was given (no fork-side arithmetic).
    // Remove: carrier gone; undo brings it back with the last state.
    ASSERT_TRUE(jims::removeChange(score, 0, m2, error)) << error.toStdString();
    score->doLayout();
    EXPECT_FALSE(jims::changeCarrier(m2, 0));
    score->undoRedo(true, nullptr);
    score->doLayout();
    ASSERT_TRUE(jims::changeCarrier(m2, 0));
    EXPECT_TRUE(jims::changeCarrier(m2, 0)->staffType()->jimsStateJson().contains(u"\"key_number\":53"));
    // Foreign choice ids are refused without touching the score.
    EXPECT_FALSE(jims::applyChange(score, 0, m2, u"tuning:700", error));
    delete score;
}

// The owner's worked example authored entirely through the controller: base
// bound to 62 (bind at measure 1 = the base staff type), then at measure 2
// mode Do->La and key Do0->La0 — the accepted m5-key-mode semantics
// (kinds key, mode; one arrow up; La wrapped) fall out unchanged.
TEST(JiMStaffTests, m6WorkedExampleAuthoredThroughTheControllerMatchesM5)
{
    Score* score = ScoreRW::readScore(u"jimstaff_data/collision.mscx");
    ASSERT_TRUE(score);
    score->doLayout();
    Measure* m1 = measureNo(score, 1);
    Measure* m2 = measureNo(score, 2);
    muse::String error;
    // Binding at measure 1 edits the base staff type (no carrier at the origin).
    ASSERT_TRUE(jims::applyChange(score, 0, m1, u"bind:reference-pitch:62", error)) << error.toStdString();
    score->doLayout();
    EXPECT_FALSE(jims::changeCarrier(m1, 0)) << "the origin measure has no carrier; the base state is edited";
    EXPECT_TRUE(jimsStaffType(score)->jimsStateJson().contains(u"\"key_number\":62"));
    ASSERT_TRUE(jims::applyChange(score, 0, m2, u"mode:1", error)) << error.toStdString();
    ASSERT_TRUE(jims::applyChange(score, 0, m2, u"key:-1:3", error)) << error.toStdString();
    score->doLayout();
    jims::ChangeIndicator model;
    ASSERT_TRUE(jims::midSystemChangeIndicator(m2, 0, model));
    ASSERT_EQ(model.kinds.size(), 2u);
    EXPECT_EQ(model.kinds[0], muse::String(u"key"));
    EXPECT_EQ(model.kinds[1], muse::String(u"mode"));
    ASSERT_EQ(model.arrows.size(), 1u);
    EXPECT_TRUE(model.arrows[0].up);
    EXPECT_EQ(model.arrows[0].to.label, muse::String(u"La"));
    delete score;
}

// Binding Requirement 2 (letter entry): typing a letter (with the input
// state's accidental) on a JiMStaff enters THAT NOTE — its identity is
// established through the Kernel entry seam (Note::setNval), not read off
// a stock-clef line position (a JiMStaff has no clef; its lines are 50 cents
// apart). D-sharp typed = D#4 = identity (-4, 7); E-flat = (3, -5); C = (1, -2).
TEST(JiMStaffTests, m6LetterEntryEstablishesTheKernelIdentityOfTheNamedNote)
{
    Score* score = ScoreRW::readScore(u"jimstaff_data/jims-template.mscx");
    ASSERT_TRUE(score);
    score->doLayout();
    ASSERT_TRUE(jimsStaffType(score)->isJiMS());
    InputState& is = score->inputState();
    is.setTrack(0);
    is.setSegment(score->tick2segment(Fraction(0, 1), false, SegmentType::ChordRest));
    is.setDuration(DurationType::V_QUARTER);
    is.setNoteEntryMode(true);

    struct Entry { int letter; AccidentalType acc; int pitch; int nPer; int nGen; };
    // step = octave*7 + letter, as Score::resolveNoteInputParams produces
    // (MuseScore's octave index is pitch/12, so C4 = 60 is octave 5: step 35).
    const Entry entries[] = {
        { 1, AccidentalType::SHARP, 63, -4, 7 },     // D#4
        { 2, AccidentalType::FLAT, 63, 3, -5 },      // Eb4
        { 0, AccidentalType::NONE, 60, 1, -2 },      // C4
        { 5, AccidentalType::NONE, 69, 0, 1 },       // A4
    };
    for (const Entry& e : entries) {
        score->startCmd(TranslatableString::untranslatable("M6 test entry"));
        is.setAccidentalType(e.acc);
        score->cmdAddPitch(5 * 7 + e.letter, false, false);
        score->endCmd();
    }
    score->doLayout();
    auto notes = jimsNotes(score);
    ASSERT_EQ(notes.size(), 4u);
    for (size_t i = 0; i < 4; ++i) {
        EXPECT_EQ(notes[i]->pitch(), entries[i].pitch) << "note " << i;
        EXPECT_TRUE(notes[i]->hasJimsPitch()) << "note " << i;
        EXPECT_EQ(notes[i]->jimsNPer(), entries[i].nPer) << "note " << i;
        EXPECT_EQ(notes[i]->jimsNGen(), entries[i].nGen) << "note " << i;
    }
    delete score;
}

// Phase 7 — the editing-only scenario: starting from the JiMStaff template
// (no XML edit, no converter, no caller-authored state JSON), reproduce the
// accepted m5-key-mode piece through the new commands alone — bind the
// reference at bar 1, type the notes (letters + accidentals, chords by
// Shift+letter), insert mode Do->La and key Do0->La0 at bar 2 through the
// change controller — then save; the harness renders and hashes it against
// jims-evidence/m5-acceptance/m5-key-mode/initial-1.png.
TEST(JiMStaffTests, m6WriteEditingScenario)
{
    const char* gate = std::getenv("JIMS_M6_SCENARIO");
    if (!gate || std::string(gate) != "1") {
        GTEST_SKIP() << "set JIMS_M6_SCENARIO=1 and JIMS_SWEEP_OUT to write the M6 editing scenario";
    }
    const char* outDir = std::getenv("JIMS_SWEEP_OUT");
    ASSERT_TRUE(outDir);
    const muse::String out = muse::String::fromUtf8(outDir);

    // The template as the wizard installs it (.mscz: score + its style).
    Score* score = ScoreRW::readScore(u"jimstaff_data/jims-template.mscz");
    ASSERT_TRUE(score);
    score->doLayout();
    ASSERT_TRUE(jimsStaffType(score)->isJiMS());

    // Title (the fixture's) — ordinary metadata/text editing.
    score->startCmd(TranslatableString::untranslatable("M6 scenario title"));
    score->setMetaTag(u"workTitle", u"Sharp-Flat Collision and Second Cluster");
    for (MeasureBase* mb = score->first(); mb; mb = mb->next()) {
        if (mb->isVBox()) {
            for (EngravingItem* e : mb->el()) {
                if (e->isText() && toText(e)->textStyleType() == TextStyleType::TITLE) {
                    toText(e)->undoChangeProperty(Pid::TEXT, muse::String(u"Sharp-Flat Collision and Second Cluster"));
                }
            }
        }
    }
    score->endCmd();

    // Bar 1: bind Re0 to key number 62 (the base state).
    muse::String error;
    ASSERT_TRUE(jims::applyChange(score, 0, measureNo(score, 1), u"bind:reference-pitch:62", error)) << error.toStdString();

    // Notes, typed. Letters as Score::resolveNoteInputParams' step (octave index = pitch/12).
    InputState& is = score->inputState();
    is.setTrack(0);
    is.setSegment(score->tick2segment(Fraction(0, 1), false, SegmentType::ChordRest));
    is.setNoteEntryMode(true);
    auto type = [&](DurationType d, int letter, int octaveIndex, AccidentalType acc, bool addToChord) {
        score->startCmd(TranslatableString::untranslatable("M6 scenario note"));
        is.setDuration(d);
        is.setAccidentalType(acc);
        score->cmdAddPitch(octaveIndex * 7 + letter, addToChord, false);
        score->endCmd();
    };
    // Bar 1: D#4 Eb4 D#4 Eb4 (quarters).
    type(DurationType::V_QUARTER, 1, 5, AccidentalType::SHARP, false);
    type(DurationType::V_QUARTER, 2, 5, AccidentalType::FLAT, false);
    type(DurationType::V_QUARTER, 1, 5, AccidentalType::SHARP, false);
    type(DurationType::V_QUARTER, 2, 5, AccidentalType::FLAT, false);
    // Bar 2: [D#4 + Eb4] half (the collision), then the second cluster
    // [C4 D4 E4 F4] half — chord members by Shift+letter.
    type(DurationType::V_HALF, 1, 5, AccidentalType::SHARP, false);
    type(DurationType::V_HALF, 2, 5, AccidentalType::FLAT, true);
    type(DurationType::V_HALF, 0, 5, AccidentalType::NONE, false);
    type(DurationType::V_HALF, 1, 5, AccidentalType::NONE, true);
    type(DurationType::V_HALF, 2, 5, AccidentalType::NONE, true);
    type(DurationType::V_HALF, 3, 5, AccidentalType::NONE, true);
    // Bar 3: C4 half, C5 half.
    type(DurationType::V_HALF, 0, 5, AccidentalType::NONE, false);
    type(DurationType::V_HALF, 0, 6, AccidentalType::NONE, false);
    is.setNoteEntryMode(false);
    score->doLayout();
    auto notes = jimsNotes(score);
    ASSERT_EQ(notes.size(), 12u);

    // Bar 2: mode Do -> La, key Do0 -> La0 (the owner's worked example).
    Measure* m2 = measureNo(score, 2);
    ASSERT_TRUE(jims::applyChange(score, 0, m2, u"mode:1", error)) << error.toStdString();
    ASSERT_TRUE(jims::applyChange(score, 0, m2, u"key:-1:3", error)) << error.toStdString();
    score->doLayout();
    jims::ChangeIndicator model;
    ASSERT_TRUE(jims::midSystemChangeIndicator(m2, 0, model));
    ASSERT_EQ(model.kinds.size(), 2u);

    ASSERT_TRUE(ScoreRW::saveScore(score, out + u"/m6-key-mode.mscx"));
    // Semantics + identities record.
    std::ofstream rec(std::string(outDir) + "/m6-key-mode-semantics.json");
    rec << "{\"base\":" << jimsStaffType(score)->jimsStateJson().toStdString()
        << ",\"change\":" << jims::changeCarrier(m2, 0)->staffType()->jimsStateJson().toStdString()
        << ",\"kinds\":[";
    for (size_t i = 0; i < model.kinds.size(); ++i) {
        rec << (i ? "," : "") << "\"" << model.kinds[i].toStdString() << "\"";
    }
    rec << "],\"arrows\":" << model.arrows.size() << ",\"identities\":[";
    for (size_t i = 0; i < notes.size(); ++i) {
        rec << (i ? "," : "") << "[" << notes[i]->jimsNPer() << "," << notes[i]->jimsNGen() << "," << notes[i]->pitch() << "]";
    }
    rec << "]}\n";
    delete score;
}
