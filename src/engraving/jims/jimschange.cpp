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
#include "dom/score.h"
#include "dom/staff.h"
#include "dom/stafftype.h"
#include "dom/stafftypechange.h"
#include "dom/system.h"

using namespace muse;

namespace mu::engraving::jims {
const StaffTypeChange* changeCarrier(const Measure* measure, staff_idx_t staffIdx)
{
    if (!measure) {
        return nullptr;
    }
    for (const EngravingItem* el : measure->el()) {
        if (el && el->isStaffTypeChange() && el->staffIdx() == staffIdx) {
            const StaffTypeChange* stc = toStaffTypeChange(el);
            if (stc->staffType() && stc->staffType()->isJiMS()) {
                return stc;
            }
        }
    }
    return nullptr;
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
    const Staff* staff = measure->score()->staff(staffIdx);
    if (!staff) {
        return false;
    }
    const StaffType* newSt = staff->staffType(measure->tick());
    const Fraction before = Fraction::fromTicks(std::max(0, measure->tick().ticks() - 1));
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

double changeAnchorPeriodCents(const StaffType::JimsFrameView& view, const ChangeIndicator& model, double periodCents)
{
    const double eps = 1e-6;
    const double fallback = std::floor(view.bottomCents() / periodCents + eps) * periodCents;
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
            const double first = std::ceil((seg.lowerCents - eps) / periodCents) * periodCents;
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
}
