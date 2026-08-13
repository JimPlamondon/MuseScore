/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 *
 * MuseScore Studio
 * Music Composition & Notation
 *
 * Copyright (C) 2021 MuseScore Limited and others
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

#ifndef MU_ENGRAVING_STAFFLINES_H
#define MU_ENGRAVING_STAFFLINES_H

#include <vector>

#include "engravingitem.h"

namespace mu::engraving {
//-------------------------------------------------------------------
//   @@ StaffLines
///    The StaffLines class is the graphic representation of a staff,
///    it draws the horizontal staff lines.
//-------------------------------------------------------------------

class StaffLines final : public EngravingItem
{
    OBJECT_ALLOCATOR(engraving, StaffLines)
    DECLARE_CLASSOF(ElementType::STAFF_LINES)

public:

    StaffLines* clone() const override { return new StaffLines(*this); }

    PointF pagePos() const override;      ///< position in page coordinates
    PointF canvasPos() const override;    ///< position in page coordinates

    const std::vector<LineF>& lines() const { return m_lines; }
    void setLines(const std::vector<LineF>& l) { m_lines = l; }

    // JiMStaff guide lines (Milestone 1): the only drawn lines on a JiMS
    // staff — solid red Do-lines at the period boundaries and the dashed
    // yellow mid-period line. Derived at layout, never serialized. Empty
    // on every non-JiMS staff, which keeps the stock draw path untouched.
    struct JimsGuideLine {
        LineF line;
        bool dashed = false;
        int rgb = 0;        // 0xRRGGBB
    };
    const std::vector<JimsGuideLine>& jimsGuideLines() const { return m_jimsGuideLines; }
    void setJimsGuideLines(const std::vector<JimsGuideLine>& l) { m_jimsGuideLines = l; }

    Measure* measure() const { return (Measure*)explicitParent(); }
    double y1() const;

    double lw() const { return m_lw; }
    void setLw(double w) { m_lw = w; }

    RectF hitBBox() const override;
    Shape hitShape() const override;

    bool collectForDrawing() const override;

private:
    friend class Factory;
    StaffLines(Measure* parent);

    double m_lw = 0.0;
    std::vector<LineF> m_lines;
    std::vector<JimsGuideLine> m_jimsGuideLines;
};
}

#endif // MU_LIBMSCORE_STAFFLINES_H
