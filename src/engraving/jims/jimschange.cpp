/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 *
 * JiMStaff Milestone 5 — change-indicator seam. See jimschange.h.
 */
#include "jimschange.h"

#include <algorithm>
#include <cmath>
#include <limits>

#include "dom/measure.h"
#include "dom/segment.h"
#include "dom/note.h"
#include "dom/part.h"
#include "dom/instrument.h"
#include "dom/chord.h"
#include "dom/score.h"
#include "dom/staff.h"
#include "dom/stafftype.h"
#include "dom/stafftypechange.h"
#include "dom/system.h"
#include "editing/undo.h"

using namespace muse;

namespace mu::engraving::jims {
namespace {
class ChangeJimsExtent : public UndoCommand
{
    OBJECT_ALLOCATOR(engraving, ChangeJimsExtent)

    Staff* m_staff = nullptr;
    Fraction m_tick;
    String m_state;

    void flip(EditData*) override
    {
        StaffType* st = m_staff ? m_staff->staffType(m_tick) : nullptr;
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
    ChangeJimsExtent(Staff* staff, const Fraction& tick, String state)
        : m_staff(staff), m_tick(tick), m_state(std::move(state)) {}
    UNDO_NAME("ChangeJimsExtent")
    UNDO_CHANGED_OBJECTS({ m_staff })
};
}

const StaffTypeChange* changeCarrier(const Measure* measure, staff_idx_t staffIdx)
{
    return measure ? changeCarrierAt(measure, staffIdx, measure->tick()) : nullptr;
}

const StaffTypeChange* changeCarrierAt(const Measure* measure, staff_idx_t staffIdx, const Fraction& tick)
{
    if (!measure) {
        return nullptr;
    }
    for (const EngravingItem* el : measure->el()) {
        if (el && el->isStaffTypeChange() && el->staffIdx() == staffIdx) {
            const StaffTypeChange* stc = toStaffTypeChange(el);
            if (stc->tick() == tick && stc->staffType() && stc->staffType()->isJiMS()) {
                return stc;
            }
        }
    }
    return nullptr;
}

std::vector<const StaffTypeChange*> changeCarriers(const Measure* measure, staff_idx_t staffIdx)
{
    std::vector<const StaffTypeChange*> result;
    if (!measure) {
        return result;
    }
    for (const EngravingItem* el : measure->el()) {
        if (!el || !el->isStaffTypeChange() || el->staffIdx() != staffIdx) {
            continue;
        }
        const StaffTypeChange* stc = toStaffTypeChange(el);
        if (stc->staffType() && stc->staffType()->isJiMS()) {
            result.push_back(stc);
        }
    }
    std::sort(result.begin(), result.end(), [](const StaffTypeChange* a, const StaffTypeChange* b) {
        return a->tick() < b->tick();
    });
    return result;
}

static bool indicatorForCarrier(const StaffTypeChange* stc, ChangeIndicator& out, const StaffType** newStaffType)
{
    if (!stc || !stc->measure() || !stc->staff()) {
        return false;
    }
    const Staff* staff = stc->staff();
    const StaffType* newSt = staff->staffType(stc->tick());
    const Fraction before = Fraction::fromTicks(std::max(0, stc->tick().ticks() - 1));
    const StaffType* oldSt = staff->staffType(before);
    if (!newSt || !oldSt || !newSt->isJiMS() || !oldSt->isJiMS() || newSt == oldSt) {
        return false;
    }
    if (!changeIndicator(oldSt->jimsStateJson(), newSt->jimsStateJson(), out)) {
        return false;
    }
    if (newStaffType) {
        *newStaffType = newSt;
    }
    return !out.empty();
}

bool midSystemChangeIndicator(const Measure* measure, staff_idx_t staffIdx,
                              ChangeIndicator& out, const StaffType** newStaffType)
{
    const StaffTypeChange* stc = changeCarrier(measure, staffIdx);
    if (!stc) {
        return false;
    }
    // System head: the full header already shows the new state.
    if (measure->system() && measure->system()->firstMeasure() == measure) {
        return false;
    }
    return indicatorForCarrier(stc, out, newStaffType);
}

bool midBarChangeIndicator(const StaffTypeChange* carrier, ChangeIndicator& out, const StaffType** newStaffType)
{
    if (!carrier || carrier->rtick().isZero()) {
        return false;
    }
    return indicatorForCarrier(carrier, out, newStaffType);
}

double changeTerrainWidth(const Measure* measure)
{
    if (!measure || !measure->score()) {
        return 0.0;
    }
    double width = 0.0;
    const Score* score = measure->score();
    for (staff_idx_t s = 0; s < score->nstaves(); ++s) {
        ChangeIndicator model;
        const StaffType* st = nullptr;
        if (midSystemChangeIndicator(measure, s, model, &st) && st) {
            const double sp = score->style().spatium();
            width = std::max(width, st->jimsHeaderGeometry(sp, score->style().defaultSpatium()).changeTerrainWidth);
        }
    }
    return width;
}

double changeTerrainWidthAt(const Measure* measure, const Fraction& tick)
{
    if (!measure || !measure->score()) {
        return 0.0;
    }
    double width = 0.0;
    const Score* score = measure->score();
    for (staff_idx_t s = 0; s < score->nstaves(); ++s) {
        const StaffTypeChange* carrier = changeCarrierAt(measure, s, tick);
        ChangeIndicator model;
        const StaffType* st = nullptr;
        if (midBarChangeIndicator(carrier, model, &st) && st) {
            const double sp = score->style().spatium();
            width = std::max(width, st->jimsHeaderGeometry(sp, score->style().defaultSpatium()).changeTerrainWidth);
        }
    }
    return width;
}

bool courtesyChangeIndicator(const Measure* measure, staff_idx_t staffIdx,
                             ChangeIndicator& out, const StaffType** stateStaffType)
{
    if (!measure || !measure->system() || measure->system()->lastMeasure() != measure) {
        return false;
    }
    const Measure* next = measure->nextMeasure();
    if (!next || !changeCarrier(next, staffIdx)) {
        return false;
    }
    const Staff* staff = measure->score()->staff(staffIdx);
    if (!staff) {
        return false;
    }
    const StaffType* oldSt = staff->staffType(measure->tick());
    const StaffType* newSt = staff->staffType(next->tick());
    if (!oldSt || !newSt || !oldSt->isJiMS() || !newSt->isJiMS() || oldSt == newSt) {
        return false;
    }
    if (!changeIndicator(oldSt->jimsStateJson(), newSt->jimsStateJson(), out)) {
        return false;
    }
    if (stateStaffType) {
        *stateStaffType = oldSt;   // the courtesy terrain sits on the OLD staff (this system)
    }
    return !out.empty();
}

double courtesyTerrainWidth(const Measure* measure)
{
    if (!measure || !measure->score()) {
        return 0.0;
    }
    double width = 0.0;
    const Score* score = measure->score();
    for (staff_idx_t s = 0; s < score->nstaves(); ++s) {
        ChangeIndicator model;
        const StaffType* st = nullptr;
        if (courtesyChangeIndicator(measure, s, model, &st) && st) {
            const double sp = score->style().spatium();
            width = std::max(width, st->jimsHeaderGeometry(sp, score->style().defaultSpatium()).changeTerrainWidth);
        }
    }
    return width;
}

double changeAnchorPeriodCents(const StaffType::JimsFrameView& view, const ChangeIndicator& model, double periodCents,
                               double doCentsAboveExtentLower)
{
    const double eps = 1e-6;
    const double fallback = doCentsAboveExtentLower
                            + std::floor((view.bottomCents() - doCentsAboveExtentLower) / periodCents + eps) * periodCents;
    if (view.empty() || periodCents <= 0.0) {
        return fallback;
    }
    // The points that move with the anchor: tonic indicators and arrow ends,
    // as (periodOffset + ordinate) in periods.
    std::vector<double> offsets;
    for (const ChangePoint& p : model.tonicIndicators) {
        offsets.push_back(p.periodOffset + p.ordinate);
    }
    for (const ChangeArrow& a : model.arrows) {
        offsets.push_back(a.from.periodOffset + a.from.ordinate);
        offsets.push_back(a.to.periodOffset + a.to.ordinate);
    }
    if (offsets.empty()) {
        return fallback;
    }
    // Candidate anchors: every Do-line inside a drawn segment, ascending.
    std::vector<double> candidates;
    for (const StaffType::JimsFrameBand& band : view.bands) {
        for (const StaffType::JimsSegment& seg : band.segments) {
            const double first = doCentsAboveExtentLower
                                 + std::ceil((seg.lowerCents - doCentsAboveExtentLower - eps) / periodCents) * periodCents;
            for (double b = first; b <= seg.upperCents + eps; b += periodCents) {
                if (candidates.empty() || std::abs(candidates.back() - b) > eps) {
                    candidates.push_back(b);
                }
            }
        }
    }
    std::sort(candidates.begin(), candidates.end());
    if (candidates.empty()) {
        return fallback;
    }
    // Overflow of a point: its distance outside the nearest drawn segment.
    auto overflowOf = [&](double cents) {
        double best = std::numeric_limits<double>::infinity();
        for (const StaffType::JimsFrameBand& band : view.bands) {
            for (const StaffType::JimsSegment& seg : band.segments) {
                const double d = std::max({ 0.0, seg.lowerCents - cents, cents - seg.upperCents });
                best = std::min(best, d);
            }
        }
        return best <= eps ? 0.0 : best;
    };
    double bestAnchor = candidates.front();
    double bestOverflow = std::numeric_limits<double>::infinity();
    for (double anchor : candidates) {
        double overflow = 0.0;
        for (double off : offsets) {
            overflow += overflowOf(anchor + off * periodCents);
        }
        // Strictly better only: ties keep the lowest candidate.
        if (overflow < bestOverflow - eps) {
            bestOverflow = overflow;
            bestAnchor = anchor;
        }
        if (bestOverflow == 0.0) {
            break;   // the lowest fully fitting Do-line wins
        }
    }
    return bestAnchor;
}

bool changeIndicatorIntoStaffType(const Score* score, staff_idx_t staffIdx, const StaffType* newStaffType,
                                  ChangeIndicator& out)
{
    if (!score || !newStaffType || !newStaffType->isJiMS()) {
        return false;
    }
    const Staff* staff = score->staff(staffIdx);
    if (!staff) {
        return false;
    }
    for (const Measure* m = score->firstMeasure(); m; m = m->nextMeasure()) {
        for (const StaffTypeChange* carrier : changeCarriers(m, staffIdx)) {
            if (staff->staffType(carrier->tick()) != newStaffType) {
                continue;
            }
            const Fraction before = Fraction::fromTicks(std::max(0, carrier->tick().ticks() - 1));
            const StaffType* oldSt = staff->staffType(before);
            if (!oldSt || !oldSt->isJiMS() || oldSt == newStaffType) {
                return false;
            }
            return changeIndicator(oldSt->jimsStateJson(), newStaffType->jimsStateJson(), out);
        }
    }
    return false;
}

bool changeIndicatorsTouchingStaffType(const Score* score, staff_idx_t staffIdx, const StaffType* staffType,
                                       ChangeIndicator& out)
{
    out = {};
    if (!score || !staffType || !staffType->isJiMS()) {
        return false;
    }
    const Staff* staff = score->staff(staffIdx);
    if (!staff) {
        return false;
    }
    auto append = [&](const ChangeIndicator& model) {
        for (const String& kind : model.kinds) {
            if (std::find(out.kinds.begin(), out.kinds.end(), kind) == out.kinds.end()) {
                out.kinds.push_back(kind);
            }
        }
        out.dotStacks.insert(out.dotStacks.end(), model.dotStacks.begin(), model.dotStacks.end());
        out.tonicIndicators.insert(out.tonicIndicators.end(), model.tonicIndicators.begin(), model.tonicIndicators.end());
        out.arrows.insert(out.arrows.end(), model.arrows.begin(), model.arrows.end());
    };
    for (const Measure* measure = score->firstMeasure(); measure; measure = measure->nextMeasure()) {
        for (const StaffTypeChange* carrier : changeCarriers(measure, staffIdx)) {
            const StaffType* newStaffType = staff->staffType(carrier->tick());
            const Fraction before = Fraction::fromTicks(std::max(0, carrier->tick().ticks() - 1));
            const StaffType* oldStaffType = staff->staffType(before);
            if (!oldStaffType || !newStaffType || oldStaffType == newStaffType) {
                continue;
            }
            const bool midBar = !carrier->rtick().isZero();
            const bool drawsAgainstStaffType = midBar ? oldStaffType == staffType : newStaffType == staffType;
            if (!drawsAgainstStaffType) {
                continue;
            }
            ChangeIndicator model;
            if (changeIndicator(oldStaffType->jimsStateJson(), newStaffType->jimsStateJson(), model)
                && !model.empty()) {
                append(model);
            }
        }
    }
    return !out.empty();
}

std::vector<double> changeIndicatorOverflowCents(const StaffType::JimsFrameView& view, const ChangeIndicator& model,
                                                 double periodCents, double doCentsAboveExtentLower)
{
    std::vector<double> out;
    if (view.empty() || periodCents <= 0.0) {
        return out;
    }
    const double eps = 1e-6;
    const double anchor = changeAnchorPeriodCents(view, model, periodCents, doCentsAboveExtentLower);
    auto inside = [&](double cents) {
        for (const StaffType::JimsFrameBand& band : view.bands) {
            for (const StaffType::JimsSegment& seg : band.segments) {
                if (cents >= seg.lowerCents - eps && cents <= seg.upperCents + eps) {
                    return true;
                }
            }
        }
        return false;
    };
    auto consider = [&](const ChangePoint& p) {
        const double cents = anchor + (p.periodOffset + p.ordinate) * periodCents;
        if (!inside(cents)) {
            out.push_back(cents);
        }
    };
    for (const ChangePoint& p : model.tonicIndicators) {
        consider(p);
    }
    for (const ChangeArrow& a : model.arrows) {
        consider(a.from);
        consider(a.to);
    }
    return out;
}

static bool partHasVocalRole(const Part* part, const String& role)
{
    if (!part) {
        return false;
    }
    const String id = part->instrumentId();
    return id == role || id == u"voice." + role;
}

int deriveTonicAmbits(Score* score)
{
    if (!score) {
        return 0;
    }
    const String wanted = melodyPartToken(score->jimsMelodyPart());
    Staff* melodyStaff = nullptr;
    for (Part* part : score->parts()) {
        if (partHasVocalRole(part, wanted) && !part->staves().empty()) {
            melodyStaff = part->staves().front();
            break;
        }
    }
    if (!melodyStaff) {
        return 0;
    }
    const staff_idx_t melodyStaffIdx = melodyStaff->idx();
    int changed = 0;
    // Section starts come from the explicitly designated melody staff.
    std::vector<Fraction> starts = { Fraction(0, 1) };
    for (const Measure* m = score->firstMeasure(); m; m = m->nextMeasure()) {
        for (const StaffTypeChange* carrier : changeCarriers(m, melodyStaffIdx)) {
            if (carrier->tick() > Fraction(0, 1)) {
                starts.push_back(carrier->tick());
            }
        }
    }
    for (size_t i = 0; i < starts.size(); ++i) {
        StaffType* authority = melodyStaff->staffType(starts[i]);
        if (!authority || !authority->isJiMS() || authority->jimsStateJson().isEmpty()) {
            continue;
        }
        const bool bounded = i + 1 < starts.size();
        const Fraction end = bounded ? starts[i + 1] : Fraction(0, 1);
        String melody = u"{\"notes\":[";
        bool first = true;
        for (const Segment* seg = score->firstSegment(SegmentType::ChordRest); seg;
             seg = seg->next1(SegmentType::ChordRest)) {
            if (seg->tick() < starts[i]) {
                continue;
            }
            if (bounded && seg->tick() >= end) {
                break;
            }
            for (track_idx_t track = melodyStaffIdx * VOICES; track < (melodyStaffIdx + 1) * VOICES; ++track) {
                const EngravingItem* el = seg->element(track);
                if (!el || !el->isChord()) {
                    continue;
                }
                for (const Note* note : toChord(el)->notes()) {
                    if (!note->hasJimsPitch()) {
                        continue;
                    }
                    if (!first) {
                        melody += u",";
                    }
                    melody += String(u"{\"nPer\":%1,\"nGen\":%2}").arg(note->jimsNPer()).arg(note->jimsNGen());
                    first = false;
                }
            }
        }
        melody += u"]}";
        if (first) {
            continue;
        }
        String token;
        String error;
        if (!tonicAmbitForMelody(authority->jimsStateJson(), melody, token, &error)) {
            continue;
        }
        // The identical Kernel token is repeated through every staff carrier;
        // repetition is transport, never a second authority.
        for (staff_idx_t staffIdx = 0; staffIdx < score->nstaves(); ++staffIdx) {
            Staff* staff = score->staff(staffIdx);
            StaffType* st = staff ? staff->staffType(starts[i]) : nullptr;
            if (!st || !st->isJiMS() || token == st->jimsTonicAmbit()) {
                continue;
            }
            String state = st->jimsStateJson();
            static const String key = u"\"tonic_ambit\":\"";
            const size_t at = state.indexOf(key);
            if (at != muse::nidx) {
                const size_t from = at + key.size();
                const size_t to = state.indexOf(u'"', from);
                if (to == muse::nidx) {
                    continue;
                }
                state = state.left(from) + token + state.mid(to);
            } else {
                const size_t close = state.lastIndexOf(u'}');
                if (close == muse::nidx) {
                    continue;
                }
                state = state.left(close) + u",\"tonic_ambit\":\"" + token + u"\"}";
            }
            st->setJimsStateJson(state);
            ++changed;
        }
    }
    return changed;
}

static const char* vocalRole(const Part* part)
{
    if (partHasVocalRole(part, u"soprano")) {
        return "soprano";
    }
    if (partHasVocalRole(part, u"alto")) {
        return "alto";
    }
    if (partHasVocalRole(part, u"tenor")) {
        return "tenor";
    }
    if (partHasVocalRole(part, u"bass")) {
        return "bass";
    }
    return nullptr;
}

bool defaultExtentForEmptyStaffSpan(const Staff* staff, const Fraction& start,
                                    const Fraction& stop, const String& state,
                                    String& updated)
{
    updated = state;
    if (!staff || !staff->score()) {
        return false;
    }
    const char* role = vocalRole(staff->part());
    if (!role) {
        return true;
    }
    const staff_idx_t staffIdx = staff->idx();
    for (const Segment* seg = staff->score()->firstSegment(SegmentType::ChordRest); seg;
         seg = seg->next1(SegmentType::ChordRest)) {
        if (seg->tick() < start) {
            continue;
        }
        if (!stop.negative() && seg->tick() >= stop) {
            break;
        }
        for (track_idx_t track = staffIdx * VOICES; track < (staffIdx + 1) * VOICES; ++track) {
            const EngravingItem* el = seg->element(track);
            if (!el || !el->isChord()) {
                continue;
            }
            for (const Note* note : toChord(el)->notes()) {
                if (note->hasJimsPitch()) {
                    return true;
                }
            }
        }
    }
    const Instrument* instrument = staff->part()->instrument();
    return instrument
           && defaultVocalExtent(state, instrument->minPitchA(), instrument->maxPitchA(), role, updated);
}

int reconcileExtents(Score* score)
{
    if (!score) {
        return 0;
    }
    int changed = 0;
    for (staff_idx_t staffIdx = 0; staffIdx < score->nstaves(); ++staffIdx) {
        Staff* staff = score->staff(staffIdx);
        if (!staff) {
            continue;
        }
        std::vector<Fraction> starts = { Fraction(0, 1) };
        for (const Measure* m = score->firstMeasure(); m; m = m->nextMeasure()) {
            for (const StaffTypeChange* carrier : changeCarriers(m, staffIdx)) {
                if (carrier->tick() > Fraction(0, 1)) {
                    starts.push_back(carrier->tick());
                }
            }
        }
        for (size_t i = 0; i < starts.size(); ++i) {
            StaffType* st = staff->staffType(starts[i]);
            if (!st || !st->isJiMS()) {
                continue;
            }
            const bool bounded = i + 1 < starts.size();
            const Fraction end = bounded ? starts[i + 1] : Fraction(0, 1);
            String melody = u"{\"notes\":[";
            bool first = true;
            for (const Segment* seg = score->firstSegment(SegmentType::ChordRest); seg; seg = seg->next1(SegmentType::ChordRest)) {
                if (seg->tick() < starts[i]) {
                    continue;
                }
                if (bounded && seg->tick() >= end) {
                    break;
                }
                for (track_idx_t track = staffIdx * VOICES; track < (staffIdx + 1) * VOICES; ++track) {
                    const EngravingItem* el = seg->element(track);
                    if (!el || !el->isChord()) {
                        continue;
                    }
                    for (const Note* note : toChord(el)->notes()) {
                        if (!note->hasJimsPitch()) {
                            continue;
                        }
                        if (!first) {
                            melody += u",";
                        }
                        melody += String(u"{\"nPer\":%1,\"nGen\":%2}").arg(note->jimsNPer()).arg(note->jimsNGen());
                        first = false;
                    }
                }
            }
            melody += u"]}";
            String updated;
            bool ok = false;
            if (!first) {
                ok = fitExtent(st->jimsStateJson(), melody, updated);
            } else {
                ok = defaultExtentForEmptyStaffSpan(staff, starts[i], bounded ? end : Fraction(-1, 1),
                                                    st->jimsStateJson(), updated);
            }
            if (ok && updated != st->jimsStateJson()) {
                st->setJimsStateJson(updated);
                ++changed;
            }
        }
    }
    return changed;
}

bool widenExtentForNote(Note* note)
{
    if (!note || !note->staff() || !note->hasJimsPitch()) {
        return false;
    }
    StaffType* st = note->staff()->staffType(note->tick());
    if (!st || !st->isJiMS()) {
        return false;
    }
    String updated;
    const bool changed = widenExtent(st->jimsStateJson(), note->jimsNPer(), note->jimsNGen(), updated)
                         && updated != st->jimsStateJson();
    if (changed) {
        note->score()->undo(new ChangeJimsExtent(note->staff(), note->tick(), updated));
    }
    designatedMelodyNoteChanged(note);
    return changed;
}

void designatedMelodyNoteChanged(Note* note)
{
    if (!note || !note->score() || !note->part()) {
        return;
    }
    const String wanted = melodyPartToken(note->score()->jimsMelodyPart());
    if (partHasVocalRole(note->part(), wanted)) {
        deriveTonicAmbits(note->score());
    }
}
}
