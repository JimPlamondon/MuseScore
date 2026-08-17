/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 *
 * JiMStaff Milestone 3 — shared continuous-tuning controller.
 * See jimstuningcontroller.h.
 */
#include "jimstuningcontroller.h"

#include <chrono>

#include "serialization/json.h"

#include "jimsbridge.h"

#include "../dom/chord.h"
#include "../dom/masterscore.h"
#include "../dom/measure.h"
#include "../dom/note.h"
#include "../dom/score.h"
#include "../dom/segment.h"
#include "../dom/staff.h"
#include "../dom/stafftype.h"
#include "../dom/stafftypechange.h"
#include "../editing/undo.h"

using namespace muse;

namespace mu::engraving::jims {
namespace {
// Replace ONLY generator_cents in a state JSON, preserving every other
// field byte-for-byte where possible. String-level surgery keeps the
// Kernel-authored field order and content intact — this is transport,
// not a musical-fact computation.
String withGeneratorCents(const String& stateJson, double generatorCents)
{
    static const String marker = u"\"generator_cents\":";
    size_t at = stateJson.indexOf(marker);
    if (at == muse::nidx) {
        return stateJson;
    }
    size_t from = at + marker.size();
    size_t end = from;
    while (end < stateJson.size()) {
        Char c = stateJson.at(end);
        if (c == u',' || c == u'}') {
            break;
        }
        ++end;
    }
    return stateJson.left(from) + String::number(generatorCents, 12) + stateJson.mid(end);
}

// One undoable edit covering every span of the staff: flip() swaps all
// captured state JSONs at once, so undo/redo is a single step.
class JimsChangeStaffStates : public UndoCommand
{
    OBJECT_ALLOCATOR(engraving, JimsChangeStaffStates)

    Staff* m_staff = nullptr;
    std::vector<Fraction> m_ticks;
    std::vector<String> m_states;

    void flip(EditData*) override
    {
        std::vector<String> previous;
        previous.reserve(m_ticks.size());
        for (size_t i = 0; i < m_ticks.size(); ++i) {
            StaffType* st = m_staff->staffType(m_ticks[i]);
            previous.push_back(st ? st->jimsStateJson() : String());
            if (st) {
                st->setJimsStateJson(m_states[i]);
            }
        }
        m_states = previous;
        m_staff->score()->setLayoutAll();
    }

public:
    JimsChangeStaffStates(Staff* staff, std::vector<Fraction> ticks, std::vector<String> states)
        : m_staff(staff), m_ticks(std::move(ticks)), m_states(std::move(states)) {}

    UNDO_NAME("JimsChangeStaffStates")
    UNDO_CHANGED_OBJECTS({ m_staff })
};
}

TuningController::TuningController(Score* score, staff_idx_t staffIdx)
    : m_score(score), m_staffIdx(staffIdx)
{
}

bool TuningController::collectSpans(std::vector<Span>& spans) const
{
    Staff* staff = m_score ? m_score->staff(m_staffIdx) : nullptr;
    if (!staff) {
        return false;
    }
    StaffType* base = staff->staffType(Fraction(0, 1));
    if (!base || !base->isJiMS()) {
        return false;
    }
    spans.push_back({ nullptr, base->jimsStateJson() });
    for (MeasureBase* mb = m_score->first(); mb; mb = mb->next()) {
        if (!mb->isMeasure()) {
            continue;
        }
        for (EngravingItem* el : mb->el()) {
            if (el && el->isStaffTypeChange() && el->staffIdx() == m_staffIdx) {
                StaffTypeChange* change = toStaffTypeChange(el);
                if (change->staffType() && change->staffType()->isJiMS()) {
                    spans.push_back({ change, change->staffType()->jimsStateJson() });
                }
            }
        }
    }
    return true;
}

double TuningController::currentGeneratorCents() const
{
    std::vector<Span> spans;
    if (!collectSpans(spans) || spans.empty()) {
        return 0.0;
    }
    double generatorCents = 0.0, periodCents = 0.0;
    return staffMetrics(spans.front().stateJson, generatorCents, periodCents) ? generatorCents : 0.0;
}

bool TuningController::beginPreview()
{
    m_prePreview.clear();
    if (!collectSpans(m_prePreview)) {
        return false;
    }
    m_previewing = true;
    return true;
}

bool TuningController::applyToSpans(double generatorCents)
{
    Staff* staff = m_score->staff(m_staffIdx);
    if (!staff) {
        return false;
    }
    StaffType* base = staff->staffType(Fraction(0, 1));
    if (!base || !base->isJiMS()) {
        return false;
    }
    base->setJimsStateJson(withGeneratorCents(base->jimsStateJson(), generatorCents));
    for (MeasureBase* mb = m_score->first(); mb; mb = mb->next()) {
        if (!mb->isMeasure()) {
            continue;
        }
        for (EngravingItem* el : mb->el()) {
            if (el && el->isStaffTypeChange() && el->staffIdx() == m_staffIdx) {
                StaffTypeChange* change = toStaffTypeChange(el);
                StaffType* st = staff->staffType(mb->tick());
                if (st && st->isJiMS() && change->staffType() && change->staffType()->isJiMS()) {
                    st->setJimsStateJson(withGeneratorCents(st->jimsStateJson(), generatorCents));
                }
            }
        }
    }
    invalidateAndLayout();
    return true;
}

void TuningController::invalidateAndLayout()
{
    // Every cached musical fact re-derives from the Kernel: note cents
    // caches reset (the lattice identities are the durable facts), and
    // the frame cache re-keys by construction because the state string
    // changed. Layout then reprojects through the single seam.
    for (Segment* seg = m_score->firstSegment(SegmentType::ChordRest); seg;
         seg = seg->next1(SegmentType::ChordRest)) {
        for (track_idx_t track = m_staffIdx * VOICES; track < (m_staffIdx + 1) * VOICES; ++track) {
            EngravingItem* el = seg->element(track);
            if (el && el->isChord()) {
                for (Note* note : toChord(el)->notes()) {
                    if (note->hasJimsPitch()) {
                        note->setJimsPitch(note->jimsNPer(), note->jimsNGen());
                    }
                }
            }
        }
    }
    m_score->setLayoutAll();
    m_score->doLayout();
    // Milestone 7 (playback): a live preview (and its cancel) edits the
    // staff states outside an undoable command, so nothing tells the
    // playback model that this staff's notes now sound differently. Send
    // the score's EXISTING change signal for this staff's whole tick range
    // — the same channel endCmd uses — so the next rebuild re-derives every
    // JiMS note's sounding pitch from the current state. Commit already
    // announces itself through endCmd.
    if (m_previewing) {
        ScoreChanges changes;
        changes.tickFrom = 0;
        const Measure* last = m_score->lastMeasure();
        changes.tickTo = last ? last->endTick().ticks() : 0;
        changes.staffIdxFrom = m_staffIdx;
        changes.staffIdxTo = m_staffIdx;
        changes.changedTypes.insert(ElementType::STAFFTYPE_CHANGE);
        m_score->changesChannel().send(changes);
    }
}

bool TuningController::preview(double generatorCents)
{
    if (!m_previewing) {
        return false;
    }
    const auto t0 = std::chrono::steady_clock::now();
    const bool ok = applyToSpans(generatorCents);
    m_lastApplyMs = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t0).count();
    return ok;
}

void TuningController::restoreSpans(const std::vector<Span>& spans)
{
    Staff* staff = m_score->staff(m_staffIdx);
    if (!staff || spans.empty()) {
        return;
    }
    StaffType* base = staff->staffType(Fraction(0, 1));
    if (base) {
        base->setJimsStateJson(spans.front().stateJson);
    }
    for (size_t i = 1; i < spans.size(); ++i) {
        if (spans[i].change && spans[i].change->parent()) {
            StaffType* st = staff->staffType(toMeasure(spans[i].change->parent())->tick());
            if (st) {
                st->setJimsStateJson(spans[i].stateJson);
            }
        }
    }
    invalidateAndLayout();
}

void TuningController::cancel()
{
    if (!m_previewing) {
        return;
    }
    restoreSpans(m_prePreview);
    m_previewing = false;
    m_prePreview.clear();
}

bool TuningController::commit(double generatorCents)
{
    if (!m_previewing) {
        return false;
    }
    // Restore the pre-preview state silently, then land the final value
    // as exactly one undoable edit from the ORIGINAL state.
    std::vector<Span> original = m_prePreview;
    restoreSpans(original);
    m_previewing = false;
    m_prePreview.clear();

    Staff* staff = m_score->staff(m_staffIdx);
    if (!staff) {
        return false;
    }
    std::vector<Fraction> ticks;
    std::vector<String> states;
    ticks.push_back(Fraction(0, 1));
    states.push_back(withGeneratorCents(original.front().stateJson, generatorCents));
    for (size_t i = 1; i < original.size(); ++i) {
        if (original[i].change && original[i].change->parent()) {
            ticks.push_back(toMeasure(original[i].change->parent())->tick());
            states.push_back(withGeneratorCents(original[i].stateJson, generatorCents));
        }
    }
    const auto t0 = std::chrono::steady_clock::now();
    m_score->startCmd(TranslatableString("undoableAction", "Change JiMS tuning"));
    m_score->undo(new JimsChangeStaffStates(staff, std::move(ticks), std::move(states)));
    m_score->endCmd();
    invalidateAndLayout();
    m_lastApplyMs = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t0).count();
    return true;
}
}
