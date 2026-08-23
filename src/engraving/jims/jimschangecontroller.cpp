/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 *
 * JiMStaff Milestone 6 — change-insertion controller (owner decision 1a,
 * 2026-08-16). Transports Kernel-returned states into the StaffTypeChange
 * carrier; computes no musical fact.
 */
#include "jimschangecontroller.h"

#include "../dom/factory.h"
#include "../dom/chord.h"
#include "../dom/measure.h"
#include "../dom/note.h"
#include "../dom/score.h"
#include "../dom/segment.h"
#include "../dom/staff.h"
#include "../dom/stafftype.h"
#include "../dom/stafftypechange.h"
#include "../dom/tie.h"
#include "../dom/utils.h"
#include "../editing/undo.h"
#include "jimschange.h"
#include "jimsbridge.h"

#include "translation.h"

#include <cmath>
#include <set>

using namespace muse;

namespace mu::engraving::jims {
namespace {
struct StateEdit {
    Staff* staff = nullptr;
    staff_idx_t staffIdx = 0;
    Fraction tick;
    const Measure* stop = nullptr;
    String state;
};

struct NoteEdit {
    Note* note = nullptr;
    SoundingPitch projection;
    int tpc = Tpc::TPC_INVALID;
};

const Measure* nextCarrierMeasure(const Score* score, staff_idx_t staffIdx, const Fraction& tick)
{
    Measure* start = score->tick2measure(tick);
    for (Measure* measure = start ? start->nextMeasure() : nullptr; measure; measure = measure->nextMeasure()) {
        if (changeCarrier(measure, staffIdx)) {
            return measure;
        }
    }
    return nullptr;
}

const StateEdit* stateEditFor(const std::vector<StateEdit>& edits, const Note* note)
{
    for (const StateEdit& edit : edits) {
        if (note->staffIdx() != edit.staffIdx || note->tick() < edit.tick) {
            continue;
        }
        if (!edit.stop || note->tick() < edit.stop->tick()) {
            return &edit;
        }
    }
    return nullptr;
}

bool projectionFor(const std::vector<StateEdit>& edits, Note* note, SoundingPitch& projection, String& error)
{
    const StateEdit* edit = stateEditFor(edits, note);
    const StaffType* current = note->staff() ? note->staff()->staffTypeForElement(note) : nullptr;
    const String state = edit ? edit->state : (current ? current->jimsStateJson() : String());
    if (state.isEmpty()) {
        error = u"a linked JiMS note has no effective JiMS state";
        return false;
    }

    Note* first = note;
    while (first->tieBackNonPartial() && first->tieBackNonPartial()->startNote()) {
        first = first->tieBackNonPartial()->startNote();
    }
    if (edit && first->tick() < edit->tick) {
        SoundingPitch established;
        const StateEdit* firstEdit = stateEditFor(edits, first);
        const StaffType* firstCurrent = first->staff() ? first->staff()->staffTypeForElement(first) : nullptr;
        const String firstState = firstEdit ? firstEdit->state : (firstCurrent ? firstCurrent->jimsStateJson() : String());
        if (firstState.isEmpty()
            || !noteSoundingPitch(firstState, first->jimsNPer(), first->jimsNGen(), established, &error)) {
            return false;
        }
        return noteContinuation(state, established.frequencyHz, projection, &error);
    }
    return noteSoundingPitch(state, note->jimsNPer(), note->jimsNGen(), projection, &error);
}

bool sameProjection(const SoundingPitch& a, const SoundingPitch& b)
{
    return a.nPer == b.nPer && a.nGen == b.nGen && a.midiKey == b.midiKey
           && a.step == b.step && a.alter == b.alter && a.octave == b.octave
           && std::abs(a.centsOffset - b.centsOffset) < 1e-9
           && std::abs(a.frequencyHz - b.frequencyHz) < 1e-9;
}

bool prepareNoteEdits(Score* score, const std::vector<StateEdit>& stateEdits,
                      std::vector<NoteEdit>& noteEdits, String& error)
{
    std::set<Note*> seen;
    for (const StateEdit& edit : stateEdits) {
        Measure* start = score->tick2measure(edit.tick);
        for (Measure* measure = start; measure && measure != edit.stop; measure = measure->nextMeasure()) {
            for (Segment* segment = measure->first(SegmentType::ChordRest); segment;
                 segment = segment->next(SegmentType::ChordRest)) {
                for (voice_idx_t voice = 0; voice < VOICES; ++voice) {
                    EngravingItem* item = segment->element(edit.staffIdx * VOICES + voice);
                    if (!item || !item->isChord()) {
                        continue;
                    }
                    for (Note* note : toChord(item)->notes()) {
                        if (!note->hasJimsPitch() || seen.count(note)) {
                            continue;
                        }
                        if (note->incomingPartialTie() || note->outgoingPartialTie()) {
                            error = u"a path-dependent partial tie crosses the JiMS state span; the edit was not applied";
                            return false;
                        }
                        SoundingPitch projection;
                        if (!projectionFor(stateEdits, note, projection, error)) {
                            return false;
                        }
                        for (EngravingObject* linkedObject : note->linkList()) {
                            Note* linked = toNote(linkedObject);
                            if (!linked->hasJimsPitch()) {
                                error = u"a linked note disagrees about JiMS identity; the edit was not applied";
                                return false;
                            }
                            SoundingPitch linkedProjection;
                            if (!projectionFor(stateEdits, linked, linkedProjection, error)) {
                                return false;
                            }
                            if (!sameProjection(projection, linkedProjection)) {
                                error = u"linked notes require conflicting JiMS projections; the edit was not applied";
                                return false;
                            }
                            seen.insert(linked);
                        }
                        const int step = int(String(u"CDEFGAB").indexOf(Char(projection.step)));
                        noteEdits.push_back({ note, projection, step2tpc(step, AccidentalVal(projection.alter)) });
                    }
                }
            }
        }
    }
    return true;
}

void commitNoteEdits(Score* score, const std::vector<NoteEdit>& edits)
{
    for (const NoteEdit& edit : edits) {
        edit.note->undoChangeProperty(Pid::JIMS_NPER, edit.projection.nPer);
        edit.note->undoChangeProperty(Pid::JIMS_NGEN, edit.projection.nGen);
        score->undoChangePitch(edit.note, edit.projection.midiKey, edit.tpc, edit.tpc);
        edit.note->undoChangeProperty(Pid::TUNING, edit.projection.centsOffset);
    }
}

/// Replace the JiMS state of the staff type in force at `tick` on `staff`
/// (the base type or a carrier's copy in the staff's list) — one undoable
/// flip, layout invalidated (the same shape the tuning controller uses).
class JimsChangeStateAt : public UndoCommand
{
    OBJECT_ALLOCATOR(engraving, JimsChangeStateAt)

    Staff* m_staff = nullptr;
    Fraction m_tick;
    String m_state;

    void flip(EditData*) override
    {
        StaffType* st = m_staff->staffType(m_tick);
        if (!st) {
            return;
        }
        String previous = st->jimsStateJson();
        st->setJimsStateJson(m_state);
        m_state = previous;
        m_staff->staffTypeListChanged(m_tick);
        m_staff->score()->setLayoutAll();
    }

public:
    JimsChangeStateAt(Staff* staff, const Fraction& tick, String state)
        : m_staff(staff), m_tick(tick), m_state(std::move(state)) {}
    UNDO_NAME("JimsChangeStateAt")
    UNDO_CHANGED_OBJECTS({ m_staff })
};

const StaffTypeChange* anyCarrier(const Measure* measure, staff_idx_t staffIdx)
{
    for (const EngravingItem* el : measure->el()) {
        if (el && el->isStaffTypeChange() && el->staffIdx() == staffIdx) {
            return toStaffTypeChange(el);
        }
    }
    return nullptr;
}
}

bool effectiveState(const Score* score, staff_idx_t staffIdx, const Measure* measure,
                    String& stateJson, const StaffType** effective)
{
    if (!score || !measure || staffIdx >= score->nstaves()) {
        return false;
    }
    // Staff::staffType(tick) already answers "the type in force at this
    // tick" — a carrier at the measure installs its copy at measure->tick().
    const StaffType* st = score->staff(staffIdx)->staffType(measure->tick());
    if (!st || !st->isJiMS()) {
        return false;
    }
    stateJson = st->jimsStateJson();
    if (effective) {
        *effective = st;
    }
    return true;
}

bool changeOptions(const Score* score, staff_idx_t staffIdx, const Measure* measure,
                   StateChangeOptions& options)
{
    String state;
    if (!effectiveState(score, staffIdx, measure, state)) {
        return false;
    }
    return stateChangeOptions(state, options);
}

bool canInsertChange(const Score* score, staff_idx_t staffIdx, const Measure* measure, String& reason)
{
    String state;
    if (!effectiveState(score, staffIdx, measure, state)) {
        reason = u"not a JiMStaff";
        return false;
    }
    if (measure->tick().isZero()) {
        return true;        // the origin measure edits the base staff type
    }
    if (changeCarrier(measure, staffIdx)) {
        return true;        // the JiMS carrier is updated in place
    }
    if (anyCarrier(measure, staffIdx)) {
        reason = u"this measure already carries a non-JiMS staff type change on this staff";
        return false;
    }
    if (!measure->canAddStaffTypeChange(staffIdx)) {
        reason = u"MuseScore refuses a staff type change at this measure";
        return false;
    }
    return true;
}

bool applyChange(Score* score, staff_idx_t staffIdx, Measure* measure, const String& choiceId, String& error)
{
    String reason;
    if (!canInsertChange(score, staffIdx, measure, reason)) {
        error = reason;
        return false;
    }
    String current;
    const StaffType* effective = nullptr;
    effectiveState(score, staffIdx, measure, current, &effective);
    String next;
    if (!applyStateChange(current, choiceId, next, error)) {
        return false;
    }
    if (next == current) {
        return true;        // no-op choice: nothing to edit
    }
    Staff* staff = score->staff(staffIdx);
    const bool origin = measure->tick().isZero();
    const bool hasCarrier = changeCarrier(measure, staffIdx) != nullptr;
    if (choiceId.startsWith(u"bind:")) {
        // Binding Re0 names what the staff's reference IS — a staff-wide
        // fact, not a per-bar change (M6 gate finding, 2026-08-17: binding
        // at a change bar left the base unbound, so later key changes had
        // no anchor and drew no indicator). Apply the same Kernel choice to
        // the base staff type and to every carrier on this staff whose
        // state the Kernel reports as unbound; bound carriers (key changes)
        // keep their own reference. One undo step; no carrier is created.
        std::vector<std::pair<Fraction, String> > edits;
        auto consider = [&](const StaffType* st, const Fraction& tick) {
            if (!st || !st->isJiMS()) {
                return true;
            }
            StateChangeOptions opts;
            if (!stateChangeOptions(st->jimsStateJson(), opts)) {
                return true;
            }
            if (opts.referenceBound) {
                return true;
            }
            String bound;
            String err;
            if (!applyStateChange(st->jimsStateJson(), choiceId, bound, err)) {
                error = err;
                return false;
            }
            if (bound != st->jimsStateJson()) {
                edits.emplace_back(tick, bound);
            }
            return true;
        };
        if (!consider(staff->staffType(Fraction(0, 1)), Fraction(0, 1))) {
            return false;
        }
        for (const Measure* m = score->firstMeasure(); m; m = m->nextMeasure()) {
            if (m->tick().isZero()) {
                continue;
            }
            if (const StaffTypeChange* c = changeCarrier(m, staffIdx)) {
                if (!consider(c->staffType(), m->tick())) {
                    return false;
                }
            }
        }
        if (edits.empty()) {
            return true;
        }
        std::vector<StateEdit> stateEdits;
        for (const auto& e : edits) {
            stateEdits.push_back({ staff, staffIdx, e.first, nextCarrierMeasure(score, staffIdx, e.first), e.second });
        }
        std::vector<NoteEdit> noteEdits;
        if (!prepareNoteEdits(score, stateEdits, noteEdits, error)) {
            return false;
        }
        score->startCmd(TranslatableString("undoableAction", "Bind JiMS reference"));
        for (const auto& e : edits) {
            score->undo(new JimsChangeStateAt(staff, e.first, e.second));
        }
        commitNoteEdits(score, noteEdits);
        score->endCmd();
        return true;
    }
    const std::vector<StateEdit> stateEdits {
        { staff, staffIdx, measure->tick(), nextCarrierMeasure(score, staffIdx, measure->tick()), next }
    };
    std::vector<NoteEdit> noteEdits;
    if (!prepareNoteEdits(score, stateEdits, noteEdits, error)) {
        return false;
    }
    score->startCmd(TranslatableString("undoableAction", "Insert JiMS change"));
    if (origin || hasCarrier) {
        // The base type (origin) or the carrier's copy in the staff list is
        // the type in force at this tick: replace its state in place.
        score->undo(new JimsChangeStateAt(staff, measure->tick(), next));
    } else {
        // New carrier: a copy of the effective staff type carrying the new
        // state (file-read style; Measure::add installs the staff's copy).
        StaffTypeChange* stc = Factory::createStaffTypeChange(measure);
        stc->setParent(measure);
        stc->setTrack(staffIdx * VOICES);
        StaffType* st = new StaffType(*effective);
        st->setJimsStateJson(next);
        stc->setStaffType(st, true);
        score->undoAddElement(stc);
    }
    commitNoteEdits(score, noteEdits);
    score->endCmd();
    return true;
}

bool applyChangeToAllJimsParts(Score* score, Measure* measure, const std::vector<String>& choiceIds, String& error)
{
    // Owner decision 2a (2026-08-22). Same shape as the `bind:` branch above —
    // prepare every edit first, then commit them all in ONE transaction — but
    // widened from staff-wide to score-wide, and taking a LIST of choice ids
    // so one user gesture that is several Kernel choices is still one step.
    if (!score || !measure) {
        error = u"no score or measure";
        return false;
    }
    if (choiceIds.empty()) {
        return true;
    }
    for (const String& choiceId : choiceIds) {
        // A reference names what ONE staff's Re0 is, so it stays staff-wide
        // (owner decision 9). Routing it here would widen it by inference.
        if (choiceId.startsWith(u"bind:")) {
            error = u"a reference binding is staff-wide; apply it to one staff";
            return false;
        }
    }

    struct Prepared {
        Staff* staff = nullptr;
        staff_idx_t staffIdx = 0;
        const StaffType* effective = nullptr;
        String next;
        bool editInPlace = false;         // origin measure, or an existing JiMS carrier
    };
    std::vector<Prepared> prepared;

    // PREPARE. Every JiMS part is a target, whether or not the change turns
    // out to be a no-op for it: a part that cannot accept the change must
    // refuse the whole operation rather than be skipped, because skipping it
    // is precisely how the parts' timelines would drift apart.
    for (staff_idx_t staffIdx = 0; staffIdx < score->nstaves(); ++staffIdx) {
        Staff* staff = score->staff(staffIdx);
        const StaffType* base = staff ? staff->staffType(Fraction(0, 1)) : nullptr;
        if (!base || !base->isJiMS()) {
            continue;                       // not a JiMS part: untouched
        }
        String reason;
        if (!canInsertChange(score, staffIdx, measure, reason)) {
            const StaffType* here = staff->staffType(measure->tick());
            if (!here || !here->isJiMS()) {
                reason = u"this measure already carries a non-JiMS staff type change on this staff";
            }
            error = String(u"staff %1: %2").arg(int(staffIdx) + 1).arg(reason);
            return false;
        }
        String current;
        const StaffType* effective = nullptr;
        if (!effectiveState(score, staffIdx, measure, current, &effective)) {
            error = String(u"staff %1: no JiMS state in force at this measure").arg(int(staffIdx) + 1);
            return false;
        }
        // The Kernel applies the issued ids, in order, to THIS target's own
        // effective state. The complete state it returns is what gets stored,
        // so every field the change does not concern — this voice's frame
        // extent above all — survives, and no part's state is copied onto
        // another. The fork compares nothing and derives nothing.
        String next = current;
        for (const String& choiceId : choiceIds) {
            String out;
            String err;
            if (!applyStateChange(next, choiceId, out, err)) {
                error = String(u"staff %1: %2").arg(int(staffIdx) + 1).arg(err);
                return false;
            }
            next = out;
        }
        if (next == current) {
            continue;                       // no-op for this target: nothing to edit
        }
        prepared.push_back({ staff, staffIdx, effective, next,
                             measure->tick().isZero() || changeCarrier(measure, staffIdx) != nullptr });
    }

    if (prepared.empty()) {
        return true;                        // nothing changed anywhere
    }

    std::vector<StateEdit> stateEdits;
    for (const Prepared& p : prepared) {
        stateEdits.push_back({ p.staff, p.staffIdx, measure->tick(),
                               nextCarrierMeasure(score, p.staffIdx, measure->tick()), p.next });
    }
    std::vector<NoteEdit> noteEdits;
    if (!prepareNoteEdits(score, stateEdits, noteEdits, error)) {
        return false;
    }

    // COMMIT. One startCmd/endCmd pair for every target and every choice id,
    // so the whole gesture is one undo step and one redo step.
    score->startCmd(TranslatableString("undoableAction", "Insert JiMS change"));
    for (const Prepared& p : prepared) {
        if (p.editInPlace) {
            score->undo(new JimsChangeStateAt(p.staff, measure->tick(), p.next));
        } else {
            StaffTypeChange* stc = Factory::createStaffTypeChange(measure);
            stc->setParent(measure);
            stc->setTrack(p.staffIdx * VOICES);
            StaffType* st = new StaffType(*p.effective);
            st->setJimsStateJson(p.next);
            stc->setStaffType(st, true);
            score->undoAddElement(stc);
        }
    }
    commitNoteEdits(score, noteEdits);
    score->endCmd();
    return true;
}

bool removeChange(Score* score, staff_idx_t staffIdx, Measure* measure, String& error)
{
    const StaffTypeChange* stc = changeCarrier(measure, staffIdx);
    if (!stc) {
        error = u"no JiMS change at this measure";
        return false;
    }
    Measure* previous = measure->prevMeasure();
    Staff* staff = score->staff(staffIdx);
    const StaffType* previousType = staff ? staff->staffType(previous ? previous->tick() : Fraction(0, 1)) : nullptr;
    if (!previousType || !previousType->isJiMS()) {
        error = u"no preceding JiMS state can replace this change";
        return false;
    }
    const std::vector<StateEdit> stateEdits {
        { staff, staffIdx, measure->tick(), nextCarrierMeasure(score, staffIdx, measure->tick()), previousType->jimsStateJson() }
    };
    std::vector<NoteEdit> noteEdits;
    if (!prepareNoteEdits(score, stateEdits, noteEdits, error)) {
        return false;
    }
    score->startCmd(TranslatableString("undoableAction", "Remove JiMS change"));
    score->undoRemoveElement(const_cast<StaffTypeChange*>(stc));
    commitNoteEdits(score, noteEdits);
    score->endCmd();
    return true;
}

bool normalizeStoredPitchesAfterLoad(Score* score, size_t& repairs, String& error, bool undoable, bool commandOpen)
{
    repairs = 0;
    if (!score) {
        error = u"no score to normalize";
        return false;
    }
    std::vector<StateEdit> stateEdits;
    for (staff_idx_t staffIdx = 0; staffIdx < score->nstaves(); ++staffIdx) {
        Staff* staff = score->staff(staffIdx);
        const StaffType* base = staff ? staff->staffType(Fraction(0, 1)) : nullptr;
        if (!base || !base->isJiMS()) {
            continue;
        }
        stateEdits.push_back({ staff, staffIdx, Fraction(0, 1),
                               nextCarrierMeasure(score, staffIdx, Fraction(0, 1)), base->jimsStateJson() });
        for (Measure* measure = score->firstMeasure(); measure; measure = measure->nextMeasure()) {
            const StaffTypeChange* carrier = changeCarrier(measure, staffIdx);
            if (!carrier || !carrier->staffType() || !carrier->staffType()->isJiMS()) {
                continue;
            }
            stateEdits.push_back({ staff, staffIdx, measure->tick(),
                                   nextCarrierMeasure(score, staffIdx, measure->tick()),
                                   carrier->staffType()->jimsStateJson() });
        }
    }
    std::vector<NoteEdit> projected;
    if (!prepareNoteEdits(score, stateEdits, projected, error)) {
        return false;
    }
    std::vector<NoteEdit> repairsNeeded;
    for (const NoteEdit& edit : projected) {
        if (edit.note->jimsNPer() != edit.projection.nPer
            || edit.note->jimsNGen() != edit.projection.nGen
            || edit.note->pitch() != edit.projection.midiKey
            || edit.note->tpc1() != edit.tpc || edit.note->tpc2() != edit.tpc
            || std::abs(edit.note->tuning() - edit.projection.centsOffset) >= 1e-9) {
            repairsNeeded.push_back(edit);
        }
    }
    repairs = repairsNeeded.size();
    if (repairsNeeded.empty()) {
        return true;
    }
    if (undoable) {
        if (!commandOpen) {
            score->startCmd(TranslatableString("undoableAction", "Normalize JiMS stored pitches"));
        }
        commitNoteEdits(score, repairsNeeded);
        if (!commandOpen) {
            score->endCmd();
        }
    } else {
        for (const NoteEdit& edit : repairsNeeded) {
            for (EngravingObject* linkedObject : edit.note->linkList()) {
                Note* linked = toNote(linkedObject);
                linked->setJimsPitch(edit.projection.nPer, edit.projection.nGen);
                linked->setPitch(edit.projection.midiKey, edit.tpc, edit.tpc);
                linked->setTuning(edit.projection.centsOffset);
            }
        }
    }
    score->setLayoutAll();
    return true;
}
}
