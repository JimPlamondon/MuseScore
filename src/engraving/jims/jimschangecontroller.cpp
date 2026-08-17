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
#include "../dom/measure.h"
#include "../dom/score.h"
#include "../dom/staff.h"
#include "../dom/stafftype.h"
#include "../dom/stafftypechange.h"
#include "../editing/undo.h"
#include "jimschange.h"

#include "translation.h"

using namespace muse;

namespace mu::engraving::jims {
namespace {
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
        score->startCmd(TranslatableString("undoableAction", "Bind JiMS reference"));
        for (const auto& e : edits) {
            score->undo(new JimsChangeStateAt(staff, e.first, e.second));
        }
        score->endCmd();
        return true;
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
    score->startCmd(TranslatableString("undoableAction", "Remove JiMS change"));
    score->undoRemoveElement(const_cast<StaffTypeChange*>(stc));
    score->endCmd();
    return true;
}
}
