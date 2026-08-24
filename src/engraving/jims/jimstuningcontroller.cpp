/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 *
 * JiMStaff Milestone 3 — shared continuous-tuning controller.
 * See jimstuningcontroller.h.
 */
#include "jimstuningcontroller.h"

#include <algorithm>
#include <chrono>

#include "serialization/json.h"

#include "jimsbridge.h"
#include "jimschangecontroller.h"

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

// One undoable edit covering every JiMS span in the score: flip() swaps all
// captured state JSONs at once, so undo/redo is a single step.
class JimsChangeStaffStates : public UndoCommand
{
    OBJECT_ALLOCATOR(engraving, JimsChangeStaffStates)

    std::vector<Staff*> m_staves;
    std::vector<Fraction> m_ticks;
    std::vector<String> m_states;

    void flip(EditData*) override
    {
        std::vector<String> previous;
        previous.reserve(m_ticks.size());
        for (size_t i = 0; i < m_ticks.size(); ++i) {
            StaffType* st = m_staves[i]->staffType(m_ticks[i]);
            previous.push_back(st ? st->jimsStateJson() : String());
            if (st) {
                st->setJimsStateJson(m_states[i]);
            }
        }
        m_states = previous;
        if (!m_staves.empty()) {
            m_staves.front()->score()->setLayoutAll();
        }
    }

public:
    JimsChangeStaffStates(std::vector<Staff*> staves, std::vector<Fraction> ticks, std::vector<String> states)
        : m_staves(std::move(staves)), m_ticks(std::move(ticks)), m_states(std::move(states)) {}

    UNDO_NAME("JimsChangeStaffStates")
    std::vector<EngravingObject*> objectItems() const override
    {
        std::vector<EngravingObject*> objects;
        for (Staff* staff : m_staves) {
            if (std::find(objects.begin(), objects.end(), staff) == objects.end()) {
                objects.push_back(staff);
            }
        }
        return objects;
    }
};
}

TuningController::TuningController(Score* score, staff_idx_t staffIdx)
    : m_score(score), m_staffIdx(staffIdx)
{
}

bool TuningController::collectSpans(std::vector<Span>& spans) const
{
    if (!m_score || m_staffIdx >= m_score->nstaves()) {
        return false;
    }
    Staff* selected = m_score->staff(m_staffIdx);
    const StaffType* selectedBase = selected ? selected->staffType(Fraction(0, 1)) : nullptr;
    if (!selectedBase || !selectedBase->isJiMS()) {
        return false;
    }

    for (staff_idx_t staffIdx = 0; staffIdx < m_score->nstaves(); ++staffIdx) {
        Staff* staff = m_score->staff(staffIdx);
        StaffType* base = staff ? staff->staffType(Fraction(0, 1)) : nullptr;
        if (!base || !base->isJiMS()) {
            continue;
        }
        spans.push_back({ staff, Fraction(0, 1), base->jimsStateJson() });
        for (MeasureBase* mb = m_score->first(); mb; mb = mb->next()) {
            if (!mb->isMeasure()) {
                continue;
            }
            for (EngravingItem* el : mb->el()) {
                if (el && el->isStaffTypeChange() && el->staffIdx() == staffIdx) {
                    StaffTypeChange* change = toStaffTypeChange(el);
                    if (change->staffType() && change->staffType()->isJiMS()) {
                        spans.push_back({ staff, mb->tick(), change->staffType()->jimsStateJson() });
                    }
                }
            }
        }
    }
    return true;
}

double TuningController::currentGeneratorCents() const
{
    if (!m_score || m_staffIdx >= m_score->nstaves()) {
        return 0.0;
    }
    const Staff* staff = m_score->staff(m_staffIdx);
    const StaffType* type = staff ? staff->staffType(Fraction(0, 1)) : nullptr;
    if (!type || !type->isJiMS()) {
        return 0.0;
    }
    double generatorCents = 0.0, periodCents = 0.0;
    return staffMetrics(type->jimsStateJson(), generatorCents, periodCents) ? generatorCents : 0.0;
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
    if (!m_score) {
        return false;
    }
    std::vector<Span> before;
    if (!collectSpans(before)) {
        return false;
    }
    for (const Span& span : before) {
        StaffType* type = span.staff ? span.staff->staffType(span.tick) : nullptr;
        if (!type || !type->isJiMS()) {
            restoreSpans(before);
            return false;
        }
        type->setJimsStateJson(withGeneratorCents(type->jimsStateJson(), generatorCents));
    }
    size_t repairs = 0;
    String error;
    if (!normalizeStoredPitchesAfterLoad(m_score, repairs, error, false)) {
        restoreSpans(before);
        return false;
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
    for (staff_idx_t staffIdx = 0; staffIdx < m_score->nstaves(); ++staffIdx) {
        const Staff* staff = m_score->staff(staffIdx);
        const StaffType* base = staff ? staff->staffType(Fraction(0, 1)) : nullptr;
        if (!base || !base->isJiMS()) {
            continue;
        }
        for (Segment* seg = m_score->firstSegment(SegmentType::ChordRest); seg;
             seg = seg->next1(SegmentType::ChordRest)) {
            for (track_idx_t track = staffIdx * VOICES; track < (staffIdx + 1) * VOICES; ++track) {
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
        changes.staffIdxFrom = 0;
        changes.staffIdxTo = m_score->nstaves() ? m_score->nstaves() - 1 : 0;
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
    if (!m_score || spans.empty()) {
        return;
    }
    for (const Span& span : spans) {
        StaffType* type = span.staff ? span.staff->staffType(span.tick) : nullptr;
        if (type) {
            type->setJimsStateJson(span.stateJson);
        }
    }
    size_t repairs = 0;
    String error;
    normalizeStoredPitchesAfterLoad(m_score, repairs, error, false);
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

    std::vector<Staff*> staves;
    std::vector<Fraction> ticks;
    std::vector<String> states;
    for (const Span& span : original) {
        staves.push_back(span.staff);
        ticks.push_back(span.tick);
        states.push_back(withGeneratorCents(span.stateJson, generatorCents));
    }
    // Preflight the complete target projection before opening the undo
    // transaction, then restore the original preview baseline.
    if (!applyToSpans(generatorCents)) {
        return false;
    }
    restoreSpans(original);
    const auto t0 = std::chrono::steady_clock::now();
    m_score->startCmd(TranslatableString("undoableAction", "Change JiMS tuning"));
    m_score->undo(new JimsChangeStaffStates(std::move(staves), std::move(ticks), std::move(states)));
    size_t repairs = 0;
    String error;
    if (!normalizeStoredPitchesAfterLoad(m_score, repairs, error, true, true)) {
        m_score->endCmd();
        m_score->undoRedo(true, nullptr);
        return false;
    }
    m_score->endCmd();
    invalidateAndLayout();
    m_lastApplyMs = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t0).count();
    return true;
}
}
