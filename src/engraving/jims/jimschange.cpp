/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 *
 * JiMStaff Milestone 5 — change-indicator seam. See jimschange.h.
 */
#include "jimschange.h"

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
}
