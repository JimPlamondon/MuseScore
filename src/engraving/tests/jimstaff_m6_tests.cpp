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
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <tuple>

#include "engraving/dom/accidental.h"
#include "engraving/dom/chord.h"
#include "engraving/dom/factory.h"
#include "engraving/dom/input.h"
#include "engraving/dom/utils.h"
#include "engraving/dom/masterscore.h"
#include "engraving/dom/measure.h"
#include "engraving/dom/note.h"
#include "engraving/dom/partialtie.h"
#include "engraving/dom/segment.h"
#include "engraving/dom/staff.h"
#include "engraving/dom/text.h"
#include "engraving/dom/box.h"
#include "engraving/dom/stafftype.h"
#include "engraving/dom/stafftypechange.h"
#include "engraving/dom/tie.h"
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

std::vector<Note*> notesInMeasure(Measure* measure, staff_idx_t staffIdx = 0)
{
    std::vector<Note*> out;
    for (Segment* segment = measure->first(SegmentType::ChordRest); segment;
         segment = segment->next(SegmentType::ChordRest)) {
        for (voice_idx_t voice = 0; voice < VOICES; ++voice) {
            EngravingItem* item = segment->element(staffIdx * VOICES + voice);
            if (item && item->isChord()) {
                out.insert(out.end(), toChord(item)->notes().begin(), toChord(item)->notes().end());
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

Score* syntheticCommonToneScore()
{
    Score* score = ScoreRW::readScore(u"jimstaff_data/jims-template.mscz");
    if (!score) {
        return nullptr;
    }
    Measure* third = measureNo(score, 3);
    if (third) {
        score->deleteMeasures(third, third);
    }
    String error;
    if (!jims::applyChange(score, 0, measureNo(score, 1), u"bind:reference-pitch:62", error)) {
        delete score;
        return nullptr;
    }
    InputState& input = score->inputState();
    input.setTrack(0);
    input.setSegment(score->tick2segment(Fraction(0, 1), false, SegmentType::ChordRest));
    input.setDuration(DurationType::V_WHOLE);
    input.setAccidentalType(AccidentalType::NONE);
    input.setNoteEntryMode(true);
    for (int i = 0; i < 2; ++i) {
        score->startCmd(TranslatableString::untranslatable("common-tone note"));
        score->cmdAddPitch(5 * 7 + 1, false, false); // D4, the bound Re0.
        score->endCmd();
    }
    input.setNoteEntryMode(false);
    std::vector<Note*> notes = jimsNotes(score);
    if (notes.size() != 2) {
        delete score;
        return nullptr;
    }
    Tie* tie = Factory::createTie(score->dummy());
    tie->setStartNote(notes[0]);
    tie->setEndNote(notes[1]);
    tie->setTrack(notes[0]->track());
    tie->setTick(notes[0]->tick());
    tie->setTick2(notes[1]->tick());
    score->startCmd(TranslatableString::untranslatable("common-tone tie"));
    score->undoAddElement(tie);
    score->endCmd();
    if (!jims::applyChange(score, 0, measureNo(score, 2), u"key:-1:3", error)) {
        delete score;
        return nullptr;
    }
    score->setMetaTag(u"workTitle", u"JiMS Common-Tone Projection Acceptance");
    score->doLayout();
    return score;
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

    struct Case {
        UpDownMode mode;
        bool up;
        const char* domain;
    };
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
    // Byte for byte the Kernel's answer — except the tonic ambit, which layout
    // derives from the new section's melody and saves (owner Q4 rider,
    // automatic since 2026-08-19).
    auto withoutAmbit = [](const muse::String& json) {
        muse::String out = json;
        for (const char16_t* tok : { u",\"tonic_ambit\":\"tonic-bounded\"", u",\"tonic_ambit\":\"tonic-centered\"" }) {
            out.replace(muse::String(tok), muse::String());
        }
        return out;
    };
    EXPECT_EQ(withoutAmbit(stc->staffType()->jimsStateJson()), withoutAmbit(expected));
    EXPECT_TRUE(stc->staffType()->jimsStateJson().contains(u"\"tonic_ambit\":\"tonic-"));
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
    // The base staff type keeps its mode: the mode/key change is carried at
    // the measure only — but the BIND is staff-wide (a reference names what
    // the staff's Re0 is), so the base is now bound to 62 as well.
    EXPECT_TRUE(jimsStaffType(score)->jimsStateJson().contains(u"\"mode_rotation\":0"));
    EXPECT_TRUE(jimsStaffType(score)->jimsStateJson().contains(u"\"key_number\":62"));
    // Compounded from the carrier: options now report La as current.
    ASSERT_TRUE(jims::changeOptions(score, 0, m2, options));
    EXPECT_TRUE(options.tonics[5].current);
    EXPECT_TRUE(options.referenceBound);
    // With the base bound, the indicator at m2 is the owner's worked
    // example (kinds key, mode) — binding at the change bar no longer
    // leaves it silently undrawable.
    {
        jims::ChangeIndicator model;
        ASSERT_TRUE(jims::midSystemChangeIndicator(m2, 0, model));
        ASSERT_EQ(model.kinds.size(), 2u);
        EXPECT_EQ(model.kinds[0], muse::String(u"key"));
        EXPECT_EQ(model.kinds[1], muse::String(u"mode"));
    }
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

TEST(JiMStaffTests, stateChangeAtomicallyReinterpretsAFullTieAtExactFrequency)
{
    Score* score = ScoreRW::readScore(u"jimstaff_data/m5-key-up.mscx");
    ASSERT_TRUE(score);
    auto notes = jimsNotes(score);
    ASSERT_GE(notes.size(), 5u);
    Note* start = notes[3];
    Note* continuation = notes[4];
    start->setJimsPitch(0, 0);
    start->setPitch(62, 16, 16);
    continuation->setJimsPitch(0, 0);
    continuation->setPitch(62, 16, 16);
    Tie* tie = Factory::createTie(score->dummy());
    tie->setStartNote(start);
    tie->setEndNote(continuation);
    tie->setTrack(start->track());
    tie->setTick(start->tick());
    tie->setTick2(continuation->tick());
    score->startCmd(TranslatableString::untranslatable("projection tie fixture"));
    score->undoAddElement(tie);
    score->endCmd();

    jims::SoundingPitch established;
    const StaffType* oldState = start->staff()->staffTypeForElement(start);
    ASSERT_TRUE(jims::noteSoundingPitch(oldState->jimsStateJson(), start->jimsNPer(), start->jimsNGen(), established));
    const int oldNPer = continuation->jimsNPer();
    const int oldNGen = continuation->jimsNGen();
    const int oldPitch = continuation->pitch();
    const int oldTpc1 = continuation->tpc1();
    const int oldTpc2 = continuation->tpc2();

    String error;
    ASSERT_TRUE(jims::applyChange(score, 0, measureNo(score, 2), u"mode:1", error)) << error.toStdString();
    const StaffType* newState = continuation->staff()->staffTypeForElement(continuation);
    jims::SoundingPitch projected;
    ASSERT_TRUE(jims::noteSoundingPitch(newState->jimsStateJson(), continuation->jimsNPer(), continuation->jimsNGen(), projected));
    EXPECT_NEAR(projected.frequencyHz, established.frequencyHz, 1e-9);
    EXPECT_EQ(continuation->pitch(), projected.midiKey);
    EXPECT_NEAR(continuation->tuning(), projected.centsOffset, 1e-9);
    EXPECT_NE(continuation->jimsNGen(), oldNGen) << "the continuation takes its new-state teaching identity";

    score->undoRedo(true, nullptr);
    EXPECT_EQ(continuation->jimsNPer(), oldNPer);
    EXPECT_EQ(continuation->jimsNGen(), oldNGen);
    EXPECT_EQ(continuation->pitch(), oldPitch);
    EXPECT_EQ(continuation->tpc1(), oldTpc1);
    EXPECT_EQ(continuation->tpc2(), oldTpc2);
    score->undoRedo(false, nullptr);
    EXPECT_EQ(continuation->pitch(), projected.midiKey);
    EXPECT_EQ(continuation->jimsNPer(), projected.nPer);
    EXPECT_EQ(continuation->jimsNGen(), projected.nGen);
    delete score;
}

TEST(JiMStaffTests, syntheticTwoMeasureCommonTonePersistsItsExactContinuation)
{
    Score* score = syntheticCommonToneScore();
    ASSERT_TRUE(score);
    ASSERT_EQ(score->firstMeasure()->nextMeasure(), score->lastMeasure());
    std::vector<Note*> notes = jimsNotes(score);
    ASSERT_EQ(notes.size(), 2u);
    ASSERT_TRUE(notes[0]->tieForNonPartial());
    ASSERT_EQ(notes[0]->tieForNonPartial()->endNote(), notes[1]);
    jims::SoundingPitch first;
    jims::SoundingPitch continuation;
    ASSERT_TRUE(jims::noteSoundingPitch(notes[0]->staff()->staffTypeForElement(notes[0])->jimsStateJson(),
                                        notes[0]->jimsNPer(), notes[0]->jimsNGen(), first));
    ASSERT_TRUE(jims::noteSoundingPitch(notes[1]->staff()->staffTypeForElement(notes[1])->jimsStateJson(),
                                        notes[1]->jimsNPer(), notes[1]->jimsNGen(), continuation));
    EXPECT_NEAR(first.frequencyHz, continuation.frequencyHz, 1e-9);
    EXPECT_NE(notes[0]->jimsNGen(), notes[1]->jimsNGen());

    const String path(u"synthetic-common-tone-roundtrip.mscx");
    ASSERT_TRUE(ScoreRW::saveScore(score, path));
    Score* reopened = ScoreRW::readScore(path, true);
    ASSERT_TRUE(reopened);
    std::vector<Note*> reopenedNotes = jimsNotes(reopened);
    ASSERT_EQ(reopenedNotes.size(), 2u);
    jims::SoundingPitch reopenedContinuation;
    ASSERT_TRUE(jims::noteSoundingPitch(reopenedNotes[1]->staff()->staffTypeForElement(reopenedNotes[1])->jimsStateJson(),
                                        reopenedNotes[1]->jimsNPer(), reopenedNotes[1]->jimsNGen(), reopenedContinuation));
    EXPECT_NEAR(first.frequencyHz, reopenedContinuation.frequencyHz, 1e-9);
    delete reopened;
    delete score;
}

TEST(JiMStaffTests, writeSyntheticCommonToneAcceptanceScore)
{
    const char* outDir = std::getenv("JIMS_NOTE_CONFORMANCE_OUT");
    if (!outDir) {
        GTEST_SKIP() << "set JIMS_NOTE_CONFORMANCE_OUT to write the common-tone acceptance score";
    }
    Score* score = syntheticCommonToneScore();
    ASSERT_TRUE(score);
    const String path = String::fromUtf8(outDir) + u"/common-tone-projection.mscx";
    ASSERT_TRUE(ScoreRW::saveScore(score, path));
    delete score;
}

TEST(JiMStaffTests, consecutiveStateChangesKeepAMultiSegmentTieExact)
{
    Score* score = ScoreRW::readScore(u"jimstaff_data/m5-key-up.mscx");
    ASSERT_TRUE(score);
    std::vector<Note*> notes = jimsNotes(score);
    ASSERT_GE(notes.size(), 9u);
    Note* chain[] = { notes[3], notes[4], notes[8] };
    for (Note* note : chain) {
        note->setJimsPitch(0, 0);
        note->setPitch(62, 16, 16);
    }
    score->startCmd(TranslatableString::untranslatable("multi-segment tie fixture"));
    for (size_t i = 0; i < 2; ++i) {
        Tie* tie = Factory::createTie(score->dummy());
        tie->setStartNote(chain[i]);
        tie->setEndNote(chain[i + 1]);
        tie->setTrack(chain[i]->track());
        tie->setTick(chain[i]->tick());
        tie->setTick2(chain[i + 1]->tick());
        score->undoAddElement(tie);
    }
    score->endCmd();
    jims::SoundingPitch established;
    ASSERT_TRUE(jims::noteSoundingPitch(chain[0]->staff()->staffTypeForElement(chain[0])->jimsStateJson(), 0, 0, established));
    String error;
    ASSERT_TRUE(jims::applyChange(score, 0, measureNo(score, 2), u"mode:1", error)) << error.toStdString();
    jims::StateChangeOptions options;
    ASSERT_TRUE(jims::changeOptions(score, 0, measureNo(score, 3), options));
    bool changedAgain = false;
    for (const jims::StateChangeOption& option : options.keyTargets) {
        if (option.current) {
            continue;
        }
        const String current = score->staff(0)->staffType(measureNo(score, 3)->tick())->jimsStateJson();
        String candidate;
        jims::SoundingPitch candidateProjection;
        if (jims::applyStateChange(current, option.id, candidate, error)
            && jims::noteContinuation(candidate, established.frequencyHz, candidateProjection, &error)
            && (candidateProjection.nPer != chain[1]->jimsNPer() || candidateProjection.nGen != chain[1]->jimsNGen())
            && jims::applyChange(score, 0, measureNo(score, 3), option.id, error)) {
            changedAgain = true;
            break;
        }
    }
    ASSERT_TRUE(changedAgain) << error.toStdString();
    for (Note* note : chain) {
        jims::SoundingPitch projection;
        ASSERT_TRUE(jims::noteSoundingPitch(note->staff()->staffTypeForElement(note)->jimsStateJson(),
                                            note->jimsNPer(), note->jimsNGen(), projection));
        EXPECT_NEAR(projection.frequencyHz, established.frequencyHz, 1e-9);
        EXPECT_EQ(note->pitch(), projection.midiKey);
        EXPECT_NEAR(note->tuning(), projection.centsOffset, 1e-9);
    }
    EXPECT_NE(chain[0]->jimsNGen(), chain[1]->jimsNGen());
    delete score;
}

TEST(JiMStaffTests, projectionFailureRollsBackStateAndEveryStoredField)
{
    Score* score = ScoreRW::readScore(u"jimstaff_data/m5-key-up.mscx");
    ASSERT_TRUE(score);
    Measure* changed = measureNo(score, 2);
    std::vector<Note*> notes = notesInMeasure(changed);
    ASSERT_FALSE(notes.empty());
    notes.front()->setJimsPitch(INT_MAX, INT_MAX);
    const String stateBefore = score->staff(0)->staffType(changed->tick())->jimsStateJson();
    const auto noteBefore = std::make_tuple(notes.front()->jimsNPer(), notes.front()->jimsNGen(), notes.front()->pitch(),
                                            notes.front()->tpc1(), notes.front()->tpc2(), notes.front()->tuning());
    String error;
    EXPECT_FALSE(jims::applyChange(score, 0, changed, u"mode:1", error));
    EXPECT_FALSE(error.isEmpty());
    EXPECT_EQ(score->staff(0)->staffType(changed->tick())->jimsStateJson(), stateBefore);
    EXPECT_EQ(std::make_tuple(notes.front()->jimsNPer(), notes.front()->jimsNGen(), notes.front()->pitch(),
                              notes.front()->tpc1(), notes.front()->tpc2(), notes.front()->tuning()), noteBefore);
    delete score;
}

TEST(JiMStaffTests, linkedNotesReceiveOneCoherentProjection)
{
    Score* score = ScoreRW::readScore(u"jimstaff_data/m5-key-up.mscx");
    ASSERT_TRUE(score);
    std::vector<Note*> notes = notesInMeasure(measureNo(score, 2));
    ASSERT_GE(notes.size(), 2u);
    notes[0]->setJimsPitch(0, 0);
    notes[1]->setJimsPitch(0, 0);
    notes[1]->setPitch(notes[0]->pitch(), notes[0]->tpc1(), notes[0]->tpc2());
    notes[1]->setTuning(notes[0]->tuning());
    notes[1]->linkTo(notes[0]);
    String error;
    ASSERT_TRUE(jims::applyChange(score, 0, measureNo(score, 2), u"mode:1", error)) << error.toStdString();
    EXPECT_EQ(std::make_tuple(notes[0]->jimsNPer(), notes[0]->jimsNGen(), notes[0]->pitch(), notes[0]->tpc1(),
                              notes[0]->tpc2(), notes[0]->tuning()),
              std::make_tuple(notes[1]->jimsNPer(), notes[1]->jimsNGen(), notes[1]->pitch(), notes[1]->tpc1(),
                              notes[1]->tpc2(), notes[1]->tuning()));
    delete score;
}

TEST(JiMStaffTests, stateProjectionSpanStopsAtTheNextIndependentCarrier)
{
    Score* score = ScoreRW::readScore(u"jimstaff_data/m5-key-up.mscx");
    ASSERT_TRUE(score);
    Measure* m2 = measureNo(score, 2);
    Measure* m3 = measureNo(score, 3);
    String error;
    ASSERT_TRUE(jims::applyChange(score, 0, m3, u"mode:1", error)) << error.toStdString();
    std::vector<Note*> m2Notes = notesInMeasure(m2);
    std::vector<Note*> m3Notes = notesInMeasure(m3);
    ASSERT_FALSE(m2Notes.empty());
    ASSERT_FALSE(m3Notes.empty());
    std::vector<std::tuple<int, int, int, int, int, double> > laterBefore;
    for (Note* note : m3Notes) {
        laterBefore.emplace_back(note->jimsNPer(), note->jimsNGen(), note->pitch(), note->tpc1(), note->tpc2(), note->tuning());
    }
    const int affectedPitchBefore = m2Notes.front()->pitch();
    jims::StateChangeOptions options;
    ASSERT_TRUE(jims::changeOptions(score, 0, m2, options));
    auto current = std::find_if(options.keyTargets.begin(), options.keyTargets.end(),
                                [](const jims::StateChangeOption& option) { return option.current; });
    ASSERT_NE(current, options.keyTargets.end());
    auto target = std::min_element(options.keyTargets.begin(), options.keyTargets.end(),
                                   [&](const jims::StateChangeOption& a, const jims::StateChangeOption& b) {
        const int da = a.current ? INT_MAX : std::abs(a.nPer - current->nPer) + std::abs(a.nGen - current->nGen);
        const int db = b.current ? INT_MAX : std::abs(b.nPer - current->nPer) + std::abs(b.nGen - current->nGen);
        return da < db;
    });
    ASSERT_NE(target, options.keyTargets.end());
    ASSERT_TRUE(jims::applyChange(score, 0, m2, target->id, error)) << error.toStdString();
    EXPECT_NE(m2Notes.front()->pitch(), affectedPitchBefore);
    for (size_t i = 0; i < m3Notes.size(); ++i) {
        EXPECT_EQ(std::make_tuple(m3Notes[i]->jimsNPer(), m3Notes[i]->jimsNGen(), m3Notes[i]->pitch(),
                                  m3Notes[i]->tpc1(), m3Notes[i]->tpc2(), m3Notes[i]->tuning()), laterBefore[i]);
    }
    delete score;
}

TEST(JiMStaffTests, ambiguousPartialTieAcrossStateBoundaryIsRefusedWithoutMutation)
{
    Score* score = ScoreRW::readScore(u"jimstaff_data/m5-key-up.mscx");
    ASSERT_TRUE(score);
    std::vector<Note*> notes = jimsNotes(score);
    ASSERT_GE(notes.size(), 5u);
    Note* start = notes[3];
    Note* continuation = notes[4];
    PartialTie* tie = Factory::createPartialTie(start);
    tie->setStartNote(start);
    tie->setEndNote(continuation);
    tie->setTrack(start->track());
    tie->setTick(start->tick());
    tie->setTick2(continuation->tick());
    score->startCmd(TranslatableString::untranslatable("partial tie fixture"));
    score->undoAddElement(tie);
    score->endCmd();
    Measure* m2 = measureNo(score, 2);
    const String stateBefore = score->staff(0)->staffType(m2->tick())->jimsStateJson();
    const auto noteBefore = std::make_tuple(continuation->jimsNPer(), continuation->jimsNGen(), continuation->pitch(),
                                            continuation->tpc1(), continuation->tpc2(), continuation->tuning());
    String error;
    EXPECT_FALSE(jims::applyChange(score, 0, m2, u"mode:1", error));
    EXPECT_TRUE(error.contains(u"path-dependent partial tie")) << error.toStdString();
    EXPECT_EQ(score->staff(0)->staffType(m2->tick())->jimsStateJson(), stateBefore);
    EXPECT_EQ(std::make_tuple(continuation->jimsNPer(), continuation->jimsNGen(), continuation->pitch(),
                              continuation->tpc1(), continuation->tpc2(), continuation->tuning()), noteBefore);
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

    struct Entry {
        int letter;
        AccidentalType acc;
        int pitch;
        int nPer;
        int nGen;
    };
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

TEST(JiMStaffTests, conventionalEntryUsesTheEffectivePostChangeState)
{
    Score* score = ScoreRW::readScore(u"jimstaff_data/jims-template.mscx");
    ASSERT_TRUE(score);
    String error;
    ASSERT_TRUE(jims::applyChange(score, 0, measureNo(score, 1), u"bind:reference-pitch:62", error)) << error.toStdString();
    ASSERT_TRUE(jims::applyChange(score, 0, measureNo(score, 2), u"key:-1:3", error)) << error.toStdString();
    const StaffType* state = score->staff(0)->staffType(measureNo(score, 2)->tick());
    ASSERT_TRUE(state && state->isJiMS());
    jims::SoundingPitch expected;
    ASSERT_TRUE(jims::entryFromStandardPitch(state->jimsStateJson(), 'D', 0, 4, expected, &error)) << error.toStdString();

    InputState& input = score->inputState();
    input.setTrack(0);
    input.setSegment(score->tick2segment(measureNo(score, 2)->tick(), false, SegmentType::ChordRest));
    input.setDuration(DurationType::V_HALF);
    input.setAccidentalType(AccidentalType::NONE);
    input.setNoteEntryMode(true);
    score->startCmd(TranslatableString::untranslatable("post-change conventional entry"));
    score->cmdAddPitch(5 * 7 + 1, false, false);
    score->endCmd();
    input.setNoteEntryMode(false);
    std::vector<Note*> notes = notesInMeasure(measureNo(score, 2));
    ASSERT_EQ(notes.size(), 1u);
    EXPECT_EQ(notes[0]->jimsNPer(), expected.nPer);
    EXPECT_EQ(notes[0]->jimsNGen(), expected.nGen);
    EXPECT_EQ(notes[0]->pitch(), expected.midiKey);
    EXPECT_NEAR(notes[0]->tuning(), expected.centsOffset, 1e-9);
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
