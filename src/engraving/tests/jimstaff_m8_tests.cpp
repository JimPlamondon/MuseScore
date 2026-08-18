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

// JiMStaff Milestone 8 — octave-band elision ("hollow stacks"; owner-approved
// plan Plans/Add_JiMS_to_MuseScore/M8_OctaveBandElision_Plan.md, 2026-08-18).
// The Kernel decides which periods survive, each band's bounds and label,
// and the omitted count; the fork slices the melody per system, resolves the
// presentation policy (score style + first-system rule + per-staff
// Auto/On/Off — MuseScore's hide-empty-staves shape), caches, maps y<->cents
// piecewise, and draws. Elision is off by default and changes nothing then.

#include <gtest/gtest.h>

#include "engraving/dom/barline.h"
#include "engraving/dom/chord.h"
#include "engraving/dom/masterscore.h"
#include "engraving/dom/measure.h"
#include "engraving/dom/note.h"
#include "engraving/dom/part.h"
#include "engraving/dom/segment.h"
#include "engraving/dom/staff.h"
#include "engraving/dom/stafflines.h"
#include "engraving/dom/stafftype.h"
#include "engraving/dom/system.h"
#include "engraving/iengravingfont.h"
#include "engraving/infrastructure/mscwriter.h"
#include "engraving/jims/jimsbridge.h"
#include "engraving/rendering/iscorerenderer.h"
#include "engraving/rw/mscsaver.h"
#include "engraving/style/style.h"
#include "draw/bufferedpaintprovider.h"
#include "draw/painter.h"
#include "io/file.h"
#include "io/dir.h"

#include "utils/scorerw.h"

using namespace mu::engraving;
using namespace mu::engraving::rendering;
using namespace muse;
using namespace muse::draw;

namespace {
const String TWO_HAND(u"jimstaff_data/m8-two-hand.mscx");
const String TWO_STAVES(u"jimstaff_data/m8-two-staves.mscx");
const String SINGLE_OCTAVE(u"jimstaff_data/collision.mscx");

const double EPS = 1e-9;

class Engraving_JiMStaffM8BandElisionTests : public ::testing::Test
{
protected:
    static const StaffType* st(Score* score, staff_idx_t staffIdx = 0)
    {
        return score->staff(staffIdx)->staffType(Fraction(0, 1));
    }

    static StaffType* mutSt(Score* score, staff_idx_t staffIdx = 0)
    {
        return score->staff(staffIdx)->staffType(Fraction(0, 1));
    }

    static std::vector<System*> measureSystems(Score* score)
    {
        std::vector<System*> out;
        for (System* s : score->systems()) {
            if (s->firstMeasure()) {
                out.push_back(s);
            }
        }
        return out;
    }

    static const StaffType::JimsFrameView& viewOn(Score* score, System* system, staff_idx_t staffIdx = 0)
    {
        return st(score, staffIdx)->jimsFrameView(score, staffIdx, system);
    }

    static void setElision(Score* score, bool on)
    {
        score->style().set(Sid::jimsElideEmptyOctaves, on);
        score->setLayoutAll();
        score->doLayout();
    }

    static void setFirstSystemAll(Score* score, bool on)
    {
        score->style().set(Sid::jimsShowAllOctavesInFirstSystem, on);
        score->setLayoutAll();
        score->doLayout();
    }

    static void setOverride(Score* score, JimsElideOctaves mode, staff_idx_t staffIdx = 0)
    {
        mutSt(score, staffIdx)->setJimsElideOctaves(mode);
        score->setLayoutAll();
        score->doLayout();
    }

    static Measure* measureNo(Score* score, int n)
    {
        Measure* m = score->firstMeasure();
        for (int i = 1; m && i < n; ++i) {
            m = m->nextMeasure();
        }
        return m;
    }

    static std::vector<Chord*> chordsOf(Measure* m, staff_idx_t staffIdx = 0)
    {
        std::vector<Chord*> out;
        for (Segment* s = m->first(SegmentType::ChordRest); s; s = s->next(SegmentType::ChordRest)) {
            for (voice_idx_t v = 0; v < VOICES; ++v) {
                EngravingItem* e = s->element(staffIdx * VOICES + v);
                if (e && e->isChord()) {
                    out.push_back(toChord(e));
                }
            }
        }
        return out;
    }

    static int redDoLineCount(const StaffLines* lines)
    {
        int n = 0;
        for (const StaffLines::JimsGuideLine& g : lines->jimsGuideLines()) {
            if (!g.dashed && g.rgb == 0xE03030) {
                ++n;
            }
        }
        return n;
    }

    /// The fixture text with extra Re notes injected into bar `bar`'s first
    /// RH chord (period indices given), written to a scratch file.
    static String fixtureWithExtraNotes(const std::vector<int>& periods, int bar, const char* name)
    {
        io::File f(ScoreRW::rootPath() + u"/" + TWO_HAND);
        EXPECT_TRUE(f.open(io::IODevice::ReadOnly));
        String text = String::fromUtf8(f.readAll());
        // Find the bar: the (bar)-th "<Measure>" occurrence, then its first "<Chord>".
        size_t pos = 0;
        for (int i = 0; i < bar; ++i) {
            pos = text.indexOf(u"<Measure>", pos + 1);
        }
        const size_t chordEnd = text.indexOf(u"</Note>", pos) + String(u"</Note>").size();
        String extra;
        for (int p : periods) {
            // Re in period p: lattice (p, 0); MIDI D(p+4) = 62 + 12*p, tpc 16.
            extra += String(u"\n            <Note>\n              <pitch>%1</pitch>\n              <tpc>16</tpc>\n"
                            u"              <jimsNPer>%2</jimsNPer>\n              <jimsNGen>0</jimsNGen>\n              </Note>")
                     .arg(62 + 12 * p).arg(p);
        }
        text.insert(chordEnd, extra);
        const String dir = ScoreRW::rootPath() + u"/../../../build.release/jims-m8-scratch";
        io::Dir::mkpath(dir);
        const String path = dir + u"/" + String::fromUtf8(name);
        io::File out(path);
        EXPECT_TRUE(out.open(io::IODevice::WriteOnly));
        out.write(text.toUtf8());
        out.close();
        return path;
    }
};

// One-band structural identity (Phase 2): the whole-piece view is one band at
// yTop 0 whose map is the legacy seam bit for bit, and the inverse round-trips.
TEST_F(Engraving_JiMStaffM8BandElisionTests, m8WholeViewIsOneBandWithLegacyGeometry)
{
    MasterScore* score = ScoreRW::readScore(TWO_HAND);
    ASSERT_TRUE(score);
    score->doLayout();
    const StaffType* jst = st(score);
    ASSERT_TRUE(jst->isJiMS());
    const StaffType::JimsFrameView& whole = jst->jimsWholeFrameView(score, 0);
    ASSERT_EQ(whole.bands.size(), 1u);
    EXPECT_FALSE(whole.banded);
    EXPECT_EQ(whole.omittedPeriodCount, 0);
    EXPECT_EQ(whole.bands[0].yTopLd, 0.0);
    EXPECT_EQ(whole.bands[0].segments.size(), jst->jimsFrameSegments().size());
    EXPECT_EQ(whole.bands[0].segments.size(), 5u);   // periods -2..2
    // Owner finding 2 (2026-08-18): the whole frame's "[PitchN]:" names the
    // octave of its lowest drawn tonic row — period -2 here, C2 — not the
    // extent's period-0 tonic.
    EXPECT_EQ(whole.bands[0].labelPeriodIndex, -2);
    EXPECT_EQ(whole.bands[0].tonicLabel, u"C2");
    for (double cents : { -2400.0, -1500.0, 0.0, 1237.5, 3500.0, 3600.0 }) {
        EXPECT_EQ(jst->jimsYFromCents(cents, whole), jst->jimsYFromCents(cents)) << cents;
        EXPECT_NEAR(whole.centsFromYLd(whole.yLdFromCents(cents)), cents, EPS) << cents;
    }
    // Every system uses the whole view when elision is off, and no chord moved.
    for (System* system : measureSystems(score)) {
        const StaffType::JimsFrameView& v = viewOn(score, system);
        EXPECT_FALSE(v.banded);
        EXPECT_EQ(v.bands.size(), 1u);
        for (MeasureBase* mb : system->measures()) {
            if (!mb->isMeasure()) {
                continue;
            }
            for (Chord* c : chordsOf(toMeasure(mb))) {
                EXPECT_EQ(c->ldata()->pos().y(), 0.0);
            }
        }
    }
    delete score;
}

// (i) Elision off (the default): every system draws the whole stack; the
// three switches read their defaults; the staff type override is Auto.
TEST_F(Engraving_JiMStaffM8BandElisionTests, m8ElisionOffMatchesPhase2Baseline)
{
    MasterScore* score = ScoreRW::readScore(TWO_HAND);
    ASSERT_TRUE(score);
    score->doLayout();
    EXPECT_FALSE(score->style().styleB(Sid::jimsElideEmptyOctaves));
    EXPECT_TRUE(score->style().styleB(Sid::jimsShowAllOctavesInFirstSystem));
    EXPECT_EQ(st(score)->jimsElideOctaves(), JimsElideOctaves::Auto);
    const std::vector<System*> systems = measureSystems(score);
    ASSERT_EQ(systems.size(), 4u);
    for (System* system : systems) {
        EXPECT_FALSE(st(score)->jimsElisionActive(score, 0, system));
        const StaffType::JimsFrameView& v = viewOn(score, system);
        EXPECT_EQ(v.bands.size(), 1u);
        EXPECT_NEAR(v.bottomCents(), -2400.0, EPS);
        EXPECT_NEAR(v.topCents(), 3600.0, EPS);
        Measure* m = system->firstMeasure();
        // Every whole period draws its lower and upper Do line (shared
        // boundaries drawn once per period, today's behaviour): 5 x 2.
        EXPECT_EQ(redDoLineCount(m->staffLines(0)), 10);
    }
    delete score;
}

// (ii) Style on + staff Auto: system 1 whole (first-system rule), later
// systems two bands with three periods omitted; per-band labels and Do-line
// counts; staff height = band heights + one staffDistance gap.
TEST_F(Engraving_JiMStaffM8BandElisionTests, m8StyleOnBandsLaterSystemsWithLabelsAndHeight)
{
    MasterScore* score = ScoreRW::readScore(TWO_HAND);
    ASSERT_TRUE(score);
    score->doLayout();
    setElision(score, true);
    const std::vector<System*> systems = measureSystems(score);
    ASSERT_EQ(systems.size(), 4u);
    // System 1: the whole-piece frame.
    {
        const StaffType::JimsFrameView& v = viewOn(score, systems[0]);
        EXPECT_FALSE(v.banded);
        EXPECT_EQ(v.bands.size(), 1u);
        EXPECT_EQ(redDoLineCount(systems[0]->firstMeasure()->staffLines(0)), 10);
    }
    const double ld = st(score)->lineDistance().val();
    const double gapLd = score->style().styleS(Sid::staffDistance).val() / ld;
    for (size_t i = 1; i < systems.size(); ++i) {
        const StaffType::JimsFrameView& v = viewOn(score, systems[i]);
        EXPECT_TRUE(v.banded) << "system " << i + 1;
        ASSERT_EQ(v.bands.size(), 2u) << "system " << i + 1;
        EXPECT_EQ(v.omittedPeriodCount, 3);
        EXPECT_NEAR(v.bands[0].lowerCents, -2400.0, EPS);
        EXPECT_NEAR(v.bands[0].upperCents, -1200.0, EPS);
        EXPECT_NEAR(v.bands[1].lowerCents, 2400.0, EPS);
        EXPECT_NEAR(v.bands[1].upperCents, 3600.0, EPS);
        EXPECT_EQ(v.bands[0].tonicLabel, u"C2");
        EXPECT_EQ(v.bands[1].tonicLabel, u"C6");
        EXPECT_EQ(v.bands[0].labelPeriodIndex, -2);
        EXPECT_EQ(v.bands[1].labelPeriodIndex, 2);
        // Geometry: top band at 0, bottom band below it plus one gap.
        EXPECT_NEAR(v.bands[1].yTopLd, 0.0, EPS);
        EXPECT_NEAR(v.bands[0].yTopLd, 12.0 + gapLd, EPS);
        EXPECT_NEAR(v.heightLd(), 24.0 + gapLd, EPS);
        EXPECT_NEAR(v.gapLd, gapLd, EPS);
        // Two Do lines per whole-period band (2 bands x 2), none in the gap.
        Measure* m = systems[i]->firstMeasure();
        EXPECT_EQ(redDoLineCount(m->staffLines(0)), 4);
        // The staff lines' bbox is the drawn height (band heights + gap).
        const double spatium = score->style().spatium();
        const StaffLines* lines = systems[i]->lastMeasure()->staffLines(0);
        EXPECT_NEAR(lines->ldata()->bbox().height(), v.heightLd() * ld * spatium + lines->lw(), 1e-6);
        // Every chord sits in a band: LH chords shifted to the bottom band,
        // RH chords in the top band (offset 0 relative to the whole frame's
        // top, which is also the top band's top here).
        for (MeasureBase* mb : systems[i]->measures()) {
            if (!mb->isMeasure()) {
                continue;
            }
            for (Chord* c : chordsOf(toMeasure(mb))) {
                const Note* n = c->notes().front();
                const StaffType::JimsFrameBand* band = v.bandForCents(n->jimsCentsAboveDo());
                ASSERT_TRUE(band);
                const double expectedLd = band->yTopLd + (band->upperCents - v.topCents()) / StaffType::JIMS_CENTS_PER_LINE_DISTANCE;
                EXPECT_NEAR(c->ldata()->pos().y(), expectedLd * ld * c->spatium(), 1e-6);
                // And the note's page y equals the piecewise map of its cents.
                const double noteYLd = (c->ldata()->pos().y() + n->ldata()->pos().y()) / (ld * c->spatium());
                EXPECT_NEAR(noteYLd, v.yLdFromCents(n->jimsCentsAboveDo()), 0.2) << "notehead centroid correction aside";
            }
        }
    }
    delete score;
}

// (iv) First-system switch off: system 1 is banded too, and the header time
// signature (tick 0) sits inside a band — the band holding the stack's
// vertical middle, or the band above the gap when the middle falls in it —
// never in the gap and never below the stack.
TEST_F(Engraving_JiMStaffM8BandElisionTests, m8FirstSystemSwitchOffBandsSystemOneToo)
{
    MasterScore* score = ScoreRW::readScore(TWO_HAND);
    ASSERT_TRUE(score);
    score->doLayout();
    setElision(score, true);
    setFirstSystemAll(score, false);
    const std::vector<System*> systems = measureSystems(score);
    ASSERT_EQ(systems.size(), 4u);
    for (System* system : systems) {
        const StaffType::JimsFrameView& v = viewOn(score, system);
        EXPECT_TRUE(v.banded);
        EXPECT_EQ(v.bands.size(), 2u);
        EXPECT_EQ(v.omittedPeriodCount, 3);
    }
    {
        Measure* m1 = systems[0]->firstMeasure();
        Segment* tsSeg = m1->findSegmentR(SegmentType::TimeSig, Fraction(0, 1));
        ASSERT_TRUE(tsSeg);
        EngravingItem* ts = tsSeg->element(0);
        ASSERT_TRUE(ts && ts->isTimeSig());
        const StaffType::JimsFrameView& v = viewOn(score, systems[0]);
        const double ld = st(score)->lineDistance().val() * ts->spatium();
        // The time signature's vertical centre, in line distances below the staff top.
        const double centerLd = (ts->ldata()->pos().y() + ts->ldata()->bbox().center().y()) / ld;
        const StaffType::JimsFrameBand& top = v.bands.back();
        EXPECT_GE(centerLd, top.yTopLd - 0.5);
        EXPECT_LE(centerLd, top.yTopLd + top.heightLd() + 0.5);
        EXPECT_NEAR(centerLd, top.yTopLd + top.heightLd() / 2.0, 1.0);
    }
    setFirstSystemAll(score, true);
    EXPECT_FALSE(viewOn(score, measureSystems(score)[0]).banded);
    delete score;
}

// (iii) Staff Off beats style On; staff On beats style Off; Auto follows.
TEST_F(Engraving_JiMStaffM8BandElisionTests, m8StaffOverrideBeatsStyleInBothDirections)
{
    MasterScore* score = ScoreRW::readScore(TWO_HAND);
    ASSERT_TRUE(score);
    score->doLayout();
    setElision(score, true);
    ASSERT_TRUE(viewOn(score, measureSystems(score)[1]).banded);
    setOverride(score, JimsElideOctaves::Off);
    for (System* system : measureSystems(score)) {
        EXPECT_FALSE(viewOn(score, system).banded);
    }
    setElision(score, false);
    EXPECT_FALSE(viewOn(score, measureSystems(score)[1]).banded);
    setOverride(score, JimsElideOctaves::On);
    EXPECT_FALSE(viewOn(score, measureSystems(score)[0]).banded);   // first-system rule still applies
    EXPECT_TRUE(viewOn(score, measureSystems(score)[1]).banded);
    setOverride(score, JimsElideOctaves::Auto);
    EXPECT_FALSE(viewOn(score, measureSystems(score)[1]).banded);   // Auto follows the (off) style
    delete score;
}

// Single-octave melody: one band, unchanged from the whole frame.
TEST_F(Engraving_JiMStaffM8BandElisionTests, m8SingleOctaveMelodyIsOneBandUnchanged)
{
    MasterScore* score = ScoreRW::readScore(SINGLE_OCTAVE);
    ASSERT_TRUE(score);
    score->doLayout();
    std::vector<std::vector<StaffType::JimsSegment> > before;
    for (System* system : measureSystems(score)) {
        before.push_back(viewOn(score, system).bands.front().segments);
    }
    setElision(score, true);
    setFirstSystemAll(score, false);
    const std::vector<System*> systems = measureSystems(score);
    ASSERT_EQ(systems.size(), before.size());
    for (size_t i = 0; i < systems.size(); ++i) {
        const StaffType::JimsFrameView& v = viewOn(score, systems[i]);
        EXPECT_EQ(v.bands.size(), 1u);
        EXPECT_EQ(v.omittedPeriodCount, 0);
        ASSERT_EQ(v.bands.front().segments.size(), before[i].size());
        for (size_t k = 0; k < before[i].size(); ++k) {
            EXPECT_NEAR(v.bands.front().segments[k].lowerCents, before[i][k].lowerCents, EPS);
            EXPECT_NEAR(v.bands.front().segments[k].upperCents, before[i][k].upperCents, EPS);
            EXPECT_EQ(v.bands.front().segments[k].whole, before[i][k].whole);
        }
        EXPECT_NEAR(v.heightLd(), (v.topCents() - v.bottomCents()) / StaffType::JIMS_CENTS_PER_LINE_DISTANCE, EPS);
    }
    delete score;
}

// Every octave touched on a system: one band there, bands elsewhere.
TEST_F(Engraving_JiMStaffM8BandElisionTests, m8EveryOctaveTouchedIsOneBand)
{
    const String path = fixtureWithExtraNotes({ -1, 0, 1 }, 3, "m8-every-octave.mscx");
    MasterScore* score = ScoreRW::readScore(path, true);
    ASSERT_TRUE(score);
    score->doLayout();
    setElision(score, true);
    const std::vector<System*> systems = measureSystems(score);
    ASSERT_EQ(systems.size(), 4u);
    const StaffType::JimsFrameView& sys2 = viewOn(score, systems[1]);
    EXPECT_TRUE(sys2.banded);
    EXPECT_EQ(sys2.bands.size(), 1u);
    EXPECT_EQ(sys2.omittedPeriodCount, 0);
    EXPECT_EQ(sys2.bands.front().segments.size(), 5u);
    EXPECT_EQ(viewOn(score, systems[2]).bands.size(), 2u);
    delete score;
}

// Gap clicks snap to the nearest band edge; the exact midpoint resolves toward
// the lower-pitched band; the model inverse and note entry agree.
TEST_F(Engraving_JiMStaffM8BandElisionTests, m8GapClickSnapsToNearestBandEdgeAndTieGoesLow)
{
    MasterScore* score = ScoreRW::readScore(TWO_HAND);
    ASSERT_TRUE(score);
    score->doLayout();
    setElision(score, true);
    System* system2 = measureSystems(score)[1];
    const StaffType::JimsFrameView& v = viewOn(score, system2);
    ASSERT_EQ(v.bands.size(), 2u);
    const StaffType::JimsFrameBand& top = v.bands[1];
    const StaffType::JimsFrameBand& bottom = v.bands[0];
    const double gapTop = top.yTopLd + top.heightLd();     // top band's bottom edge (2400 c)
    const double gapBottom = bottom.yTopLd;                // bottom band's top edge (-1200 c)
    ASSERT_GT(gapBottom, gapTop);
    // Model inverse.
    EXPECT_NEAR(v.centsFromYLd(gapTop + 0.1), top.lowerCents, EPS);
    EXPECT_NEAR(v.centsFromYLd(gapBottom - 0.1), bottom.upperCents, EPS);
    EXPECT_NEAR(v.centsFromYLd((gapTop + gapBottom) / 2.0), bottom.upperCents, EPS);   // tie -> lower-pitched
    // Inside bands: the affine inverse.
    EXPECT_NEAR(v.centsFromYLd(top.yTopLd + 6.0), top.upperCents - 600.0, EPS);
    EXPECT_NEAR(v.centsFromYLd(bottom.yTopLd + 6.0), bottom.upperCents - 600.0, EPS);
    // Note entry through Score::noteValForPosition: line counts half line
    // distances below the staff top; the snapped cents quantize to the
    // nearest lattice pitch (C6 = 84 at the top band's bottom edge, C3 = 48
    // at the bottom band's top edge, the tie to C3).
    Measure* m3 = system2->firstMeasure();
    Segment* seg = m3->first(SegmentType::ChordRest);
    ASSERT_TRUE(seg);
    auto entryPitch = [&](double yLd) {
        Position pos;
        pos.segment = seg;
        pos.staffIdx = 0;
        pos.line = int(std::lround(yLd * 2.0));
        pos.fret = INVALID_FRET_INDEX;
        bool error = false;
        NoteVal nval = score->noteValForPosition(pos, AccidentalType::NONE, error);
        EXPECT_FALSE(error);
        return nval.pitch;
    };
    EXPECT_EQ(entryPitch(gapTop + 0.5), 84);
    EXPECT_EQ(entryPitch(gapBottom - 0.5), 48);
    EXPECT_EQ(entryPitch((gapTop + gapBottom) / 2.0), 48);
    delete score;
}

// Drag freeze: while frozen the system view never re-derives; the drop
// (unfreeze + layout) re-derives once and grows the band.
TEST_F(Engraving_JiMStaffM8BandElisionTests, m8DragFreezeThenDropRederives)
{
    MasterScore* score = ScoreRW::readScore(TWO_HAND);
    ASSERT_TRUE(score);
    score->doLayout();
    setElision(score, true);
    System* system2 = measureSystems(score)[1];
    ASSERT_EQ(viewOn(score, system2).bands.size(), 2u);
    Chord* rh = chordsOf(system2->firstMeasure()).front();
    Note* n = rh->notes().front();
    ASSERT_EQ(n->jimsNPer(), 2);   // D6 (2,0)
    st(score)->jimsSetFrameFrozen(true);
    // Move the note into the top of the elided region (period 1: D5 (1,0)).
    n->setJimsPitch(1, 0);
    n->setPitch(74);
    score->setLayoutAll();
    score->doLayout();
    // Frozen: the two-band view survives. (System objects are recreated by
    // every layout — always re-fetch them; the view cache itself is keyed by
    // tick range, never by System pointer.)
    system2 = measureSystems(score)[1];
    EXPECT_EQ(viewOn(score, system2).bands.size(), 2u);
    EXPECT_EQ(viewOn(score, system2).omittedPeriodCount, 3);
    st(score)->jimsSetFrameFrozen(false);
    score->setLayoutAll();
    score->doLayout();
    system2 = measureSystems(score)[1];
    const StaffType::JimsFrameView& after = viewOn(score, system2);
    ASSERT_EQ(after.bands.size(), 2u);
    EXPECT_EQ(after.omittedPeriodCount, 2);              // periods -1, 0 omitted
    EXPECT_NEAR(after.bands[1].lowerCents, 1200.0, EPS);  // top band grew down to period 1
    delete score;
}

// A keyboard octave step into a missing register grows only that system.
TEST_F(Engraving_JiMStaffM8BandElisionTests, m8KeyboardOctaveStepGrowsOnlyTheAffectedSystem)
{
    MasterScore* score = ScoreRW::readScore(TWO_HAND);
    ASSERT_TRUE(score);
    score->doLayout();
    setElision(score, true);
    std::vector<System*> systems = measureSystems(score);
    Chord* rh = chordsOf(systems[1]->firstMeasure()).front();
    Note* n = rh->notes().front();
    ASSERT_EQ(n->jimsNPer(), 2);
    score->select(n);
    score->startCmd(TranslatableString::untranslatable("M8 test octave step"));
    score->upDown(false, UpDownMode::OCTAVE);
    score->endCmd();
    score->doLayout();
    EXPECT_EQ(n->jimsNPer(), 1);
    systems = measureSystems(score);
    ASSERT_EQ(systems.size(), 4u);
    const StaffType::JimsFrameView& sys2 = viewOn(score, systems[1]);
    EXPECT_EQ(sys2.bands.size(), 2u);
    EXPECT_EQ(sys2.omittedPeriodCount, 2);
    EXPECT_NEAR(sys2.bands[1].lowerCents, 1200.0, EPS);
    for (size_t i : { 2u, 3u }) {
        const StaffType::JimsFrameView& other = viewOn(score, systems[i]);
        EXPECT_EQ(other.bands.size(), 2u) << "system " << i + 1;
        EXPECT_EQ(other.omittedPeriodCount, 3) << "system " << i + 1;
    }
    // Undo restores the three-omitted view on system 2.
    score->undoRedo(true, nullptr);
    score->doLayout();
    EXPECT_EQ(viewOn(score, measureSystems(score)[1]).omittedPeriodCount, 3);
    delete score;
}

// Owner ruling 3b (2026-08-18, keyboard precedent): on a banded system every
// barline form runs continuously from the top band to the bottom band —
// through the gap — with repeat dots at each band's middle rows; the tips of
// a repeat sit at the stack's ends. Elision off: today's single span, no
// band dot rows.
TEST_F(Engraving_JiMStaffM8BandElisionTests, m8BarlinesRunThroughTheGapWithDotsInEachBand)
{
    MasterScore* score = ScoreRW::readScore(TWO_HAND);
    ASSERT_TRUE(score);
    score->doLayout();
    // Give the last two bars of system 2 a double and an end-repeat barline.
    Measure* m3 = measureNo(score, 3);
    Measure* m4 = measureNo(score, 4);
    score->startCmd(TranslatableString::untranslatable("M8 test barlines"));
    m3->undoChangeProperty(Pid::REPEAT_END, true);
    m4->undoChangeProperty(Pid::REPEAT_START, true);
    score->endCmd();
    score->doLayout();
    score->startCmd(TranslatableString::untranslatable("M8 test double barline"));
    if (Segment* s = m4->findSegmentR(SegmentType::EndBarLine, m4->ticks())) {
        if (BarLine* bl = toBarLine(s->element(0))) {
            score->undoChangeBarLineType(bl, BarLineType::DOUBLE, true);
        }
    }
    score->endCmd();
    setElision(score, true);
    System* system2 = measureSystems(score)[1];
    const StaffType::JimsFrameView& v = viewOn(score, system2);
    ASSERT_EQ(v.bands.size(), 2u);
    const double ld = st(score)->lineDistance().val();
    int checked = 0;
    for (MeasureBase* mb : system2->measures()) {
        if (!mb->isMeasure()) {
            continue;
        }
        for (Segment* s = toMeasure(mb)->first(SegmentType::BarLineType); s; s = s->next(SegmentType::BarLineType)) {
            BarLine* bl = toBarLine(s->element(0));
            if (!bl) {
                continue;
            }
            const BarLine::LayoutData* data = bl->ldata();
            const double lineDistance = ld * bl->spatium();
            const double lw = score->style().styleS(Sid::staffLineWidth).val() * bl->spatium() * .5;
            // One continuous span over the whole stack (through the gap).
            EXPECT_NEAR(data->y1, -lw, 1e-6) << "barline type " << int(bl->barLineType());
            EXPECT_NEAR(data->y2, v.heightLd() * lineDistance + lw, 1e-6) << "barline type " << int(bl->barLineType());
            // Dot rows: one pair per band, inside that band, straddling its middle.
            ASSERT_EQ(data->jimsBandDotRows.size(), 2u);
            for (size_t i = 0; i < 2; ++i) {
                const StaffType::JimsFrameBand& band = v.bands[v.bands.size() - 1 - i];   // top to bottom
                const double bandTop = band.yTopLd * lineDistance;
                const double bandBottom = (band.yTopLd + band.heightLd()) * lineDistance;
                EXPECT_GT(data->jimsBandDotRows[i].y1, bandTop);
                EXPECT_LT(data->jimsBandDotRows[i].y2, bandBottom);
                EXPECT_NEAR((data->jimsBandDotRows[i].y1 + data->jimsBandDotRows[i].y2) / 2.0,
                            (bandTop + bandBottom) / 2.0, 1e-6);
            }
            ++checked;
        }
    }
    EXPECT_GE(checked, 3);
    // With elision off: one span (today's y1..y2), no band dot rows.
    setElision(score, false);
    for (Segment* s = measureNo(score, 3)->first(SegmentType::BarLineType); s; s = s->next(SegmentType::BarLineType)) {
        if (BarLine* bl = toBarLine(s->element(0))) {
            EXPECT_TRUE(bl->ldata()->jimsBandDotRows.empty());
        }
    }
    delete score;
}

// Owner ruling 3b: a brace joins the bands of a hollow stack at the system
// head (MuseScore's keyboard brace: the SMuFL brace glyph, x-magnified by the
// Bracket span rule, stretched to the stack); the header reserves its width;
// a whole stack has none.
TEST_F(Engraving_JiMStaffM8BandElisionTests, m8BraceJoinsTheBandsOfAHollowStack)
{
    MasterScore* score = ScoreRW::readScore(TWO_HAND);
    ASSERT_TRUE(score);
    score->doLayout();
    const String braceGlyph = score->engravingFont()->toString(SymId::brace);
    auto textsOf = [](const StaffLines* lines) {
        std::shared_ptr<BufferedPaintProvider> prv = std::make_shared<BufferedPaintProvider>();
        Painter p(prv, "m8");
        p.setViewport(RectF(0, 0, 4000, 4000));
        PaintOptions opt;
        lines->renderer()->drawItem(lines, &p, opt);
        p.endDraw();
        std::vector<String> out;
        std::function<void(const DrawData::Item&)> walk = [&](const DrawData::Item& item) {
            for (const DrawData::Data& d : item.datas) {
                for (const DrawText& t : d.texts) {
                    out.push_back(t.text);
                }
            }
            for (const DrawData::Item& c : item.chilren) {
                walk(c);
            }
        };
        walk(prv->drawData()->item);
        return out;
    };
    auto hasBrace = [&](const std::vector<String>& texts) {
        for (const String& t : texts) {
            if (t == braceGlyph) {
                return true;
            }
        }
        return false;
    };
    const double sp = score->style().spatium();
    const double dsp = score->style().defaultSpatium();
    // Whole stack: no brace, nothing reserved.
    {
        System* system2 = measureSystems(score)[1];
        const StaffType::JimsFrameView& v = viewOn(score, system2);
        EXPECT_EQ(st(score)->jimsHeaderGeometry(sp, dsp, &v).braceWidth, 0.0);
        EXPECT_FALSE(hasBrace(textsOf(system2->firstMeasure()->staffLines(0))));
    }
    setElision(score, true);
    System* system2 = measureSystems(score)[1];
    const StaffType::JimsFrameView& v = viewOn(score, system2);
    ASSERT_EQ(v.bands.size(), 2u);
    const StaffType::JimsHeaderGeometry g = st(score)->jimsHeaderGeometry(sp, dsp, &v);
    EXPECT_GT(g.braceWidth, 0.0);
    EXPECT_NEAR(g.braceMagX, 2 + 1.625, 1e-9);   // MuseScore's brace x-magnification for a two-staff span
    EXPECT_TRUE(hasBrace(textsOf(system2->firstMeasure()->staffLines(0))));
    EXPECT_FALSE(hasBrace(textsOf(system2->lastMeasure()->staffLines(0))));   // system head only
    // System 1 (whole stack under the first-system rule): no brace.
    EXPECT_FALSE(hasBrace(textsOf(measureSystems(score)[0]->firstMeasure()->staffLines(0))));
    delete score;
}

// Owner finding 1 (2026-08-18): a JiMStaff's height on a system is its frame
// view's height (bands + gaps), so systems keep the minimum system distance
// and a following staff keeps the staff distance — instead of collapsing to
// the skyline minimum as with the nominal one-period height.
TEST_F(Engraving_JiMStaffM8BandElisionTests, m8StaffHeightAndSystemDistanceUseTheFrame)
{
    for (bool elide : { false, true }) {
        MasterScore* score = ScoreRW::readScore(TWO_HAND);
        ASSERT_TRUE(score);
        score->doLayout();
        setElision(score, elide);
        const std::vector<System*> systems = measureSystems(score);
        ASSERT_EQ(systems.size(), 4u);
        const double ld = st(score)->lineDistance().val();
        const bool spread = score->style().styleB(Sid::enableVerticalSpread);
        const double minSystem = score->style().styleMM(spread ? Sid::minSystemSpread : Sid::minSystemDistance);
        for (size_t i = 0; i < systems.size(); ++i) {
            System* system = systems[i];
            const StaffType::JimsFrameView& v = viewOn(score, system);
            const double frameH = v.heightLd() * ld * score->staff(0)->spatium(system->firstMeasure()->tick());
            EXPECT_NEAR(system->staff(0)->bbox().height(), frameH, 1e-6) << "elide=" << elide << " system " << i + 1;
            EXPECT_NEAR(system->height(), system->staff(0)->bbox().bottom(), 1e-6);
            if (i + 1 < systems.size() && systems[i + 1]->page() == system->page()) {
                const double gap = systems[i + 1]->y() - (system->y() + system->height());
                EXPECT_GE(gap, minSystem - 1e-6) << "elide=" << elide << " between systems " << i + 1 << " and " << i + 2;
            }
        }
        delete score;
    }
    // Two staves: the second staff sits at least staffDistance below the
    // first staff's FRAME bottom (whole stack, hide-empty off).
    MasterScore* two = ScoreRW::readScore(TWO_STAVES);
    ASSERT_TRUE(two);
    two->doLayout();
    System* first = measureSystems(two)[0];
    const StaffType::JimsFrameView& v = viewOn(two, first);
    const double frameH = v.heightLd() * st(two)->lineDistance().val() * two->staff(0)->spatium(Fraction(0, 1));
    EXPECT_NEAR(first->staff(0)->bbox().height(), frameH, 1e-6);
    const double between = first->staff(1)->y() - (first->staff(0)->y() + first->staff(0)->bbox().height());
    EXPECT_GE(between, two->style().styleMM(Sid::staffDistance) - 1e-6);
    delete two;
}

// Owner finding 2 (2026-08-18): every "[PitchN]:" names the octave of the row
// it sits on — header labels of whole and banded stacks and the change
// indicator's terrain label — checked against the Kernel's per-period label
// for the row the text is drawn on.
TEST_F(Engraving_JiMStaffM8BandElisionTests, m8OctaveLabelsNameTheirRowEverywhere)
{
    struct Labeled {
        String text;
        double y;
    };
    auto labelsOf = [](const StaffLines* lines) {
        std::shared_ptr<BufferedPaintProvider> prv = std::make_shared<BufferedPaintProvider>();
        Painter p(prv, "m8");
        p.setViewport(RectF(0, 0, 4000, 4000));
        PaintOptions opt;
        lines->renderer()->drawItem(lines, &p, opt);
        p.endDraw();
        std::vector<Labeled> out;
        std::function<void(const DrawData::Item&)> walk = [&](const DrawData::Item& item) {
            for (const DrawData::Data& d : item.datas) {
                for (const DrawText& t : d.texts) {
                    // "<Letter><accidentals><octave>: ..." e.g. "C2: Do", "Eb-1:"
                    const size_t colon = t.text.indexOf(u':');
                    if (colon != muse::nidx && colon >= 2 && colon <= 5) {
                        const Char first = t.text.at(0);
                        const Char last = t.text.at(colon - 1);
                        if (first >= u'A' && first <= u'G' && last.isDigit()) {
                            out.push_back({ t.text.left(colon), t.rect.top() });
                        }
                    }
                }
            }
            for (const DrawData::Item& c : item.chilren) {
                walk(c);
            }
        };
        walk(prv->drawData()->item);
        return out;
    };
    // The nearest tonic row decides which period a label sits on; its text
    // must be the Kernel's label for that period.
    auto checkLabels = [&](Score* score, const StaffType* jst, const StaffLines* lines,
                           const StaffType::JimsFrameView& v, const char* what) {
        const double topY = lines->pos().y();
        const double ld = jst->lineDistance().val();
        const double periodCents = jst->jimsPeriodCents();
        double tonicCents = 0.0;
        ASSERT_TRUE(jims::tonicCentsAboveDo(jst->jimsStateJson(), tonicCents));
        const std::vector<Labeled> labels = labelsOf(lines);
        ASSERT_GE(labels.size(), 1u) << what;
        for (const Labeled& l : labels) {
            const double yLd = (l.y - topY) / (ld * lines->spatium());
            const double cents = v.centsFromYLd(yLd);
            const int k = int(std::lround((cents - tonicCents) / periodCents));
            jims::TonicPitchLabel expected;
            ASSERT_TRUE(jims::tonicPitchLabelInPeriod(jst->jimsStateJson(), k, expected)) << what;
            EXPECT_EQ(l.text, expected.label) << what << " row period " << k;
        }
        UNUSED(score);
    };
    MasterScore* score = ScoreRW::readScore(TWO_HAND);
    ASSERT_TRUE(score);
    score->doLayout();
    // Whole stacks: the lowest drawn tonic row is period -2 -> "C2".
    for (System* system : measureSystems(score)) {
        const StaffLines* lines = system->firstMeasure()->staffLines(0);
        checkLabels(score, st(score), lines, viewOn(score, system), "whole stack");
        const std::vector<Labeled> labels = labelsOf(lines);
        ASSERT_EQ(labels.size(), 1u);
        EXPECT_EQ(labels[0].text, u"C2");
    }
    setElision(score, true);
    for (System* system : measureSystems(score)) {
        const StaffType::JimsFrameView& v = viewOn(score, system);
        const StaffLines* lines = system->firstMeasure()->staffLines(0);
        checkLabels(score, st(score), lines, v, "banded");
        EXPECT_EQ(labelsOf(lines).size(), v.bands.size());   // one label per band
    }
    delete score;
    // Change indicator terrain (M6/M7 gate: bar 2 moves to reference 53, La-mode):
    // the terrain's "[PitchN]:" is the Kernel label of ITS row's period.
    MasterScore* gate = ScoreRW::readScore(u"jimstaff_data/m7-gate.mscz");
    ASSERT_TRUE(gate);
    gate->doLayout();
    Measure* m2 = measureNo(gate, 2);
    ASSERT_TRUE(m2);
    const StaffType* changeSt = gate->staff(0)->staffType(m2->tick());
    ASSERT_TRUE(changeSt && changeSt->isJiMS());
    const StaffType::JimsFrameView& gv = changeSt->jimsFrameView(gate, 0, m2->system());
    ASSERT_FALSE(gv.empty());
    checkLabels(gate, changeSt, m2->staffLines(0), gv, "terrain");
    delete gate;
}

// Regression net for owner finding 5 (M8 gate 2026-08-18; sighting of
// 2026-08-17: "partial staves in which Do was not the tonic, the red staff
// line was not on Do, other lines at wrong heights relative to the crescent
// clef, which did not point at the Do line"). The sweep did not reproduce it;
// this pins the invariants a correct display has, on off-Do (La-mode,
// tonic-bounded) partial staves at 686/700/720 cents, the M7 gate terrain
// (La-mode section, reference 53), and a banded hollow stack:
//   (a) every solid red guide line is a Do row (cents = 0 mod period), every
//       Do row inside a drawn segment has one, and no dashed scaffold line
//       lies on a Do row — in every measure, head or not;
//   (b) at every system head, every crescent horn is a Do row and every drawn
//       red line has a crescent horn on it (the clef points at the Do line);
//   (c) the Kernel's Do dot stack sits at 0 cents and its glyph is drawn on
//       the red line of every period drawn.
TEST_F(Engraving_JiMStaffM8BandElisionTests, m8DoRowsCarryRedLinesCrescentHornsAndDoDots)
{
    const Color RED(0xE0, 0x30, 0x30);
    struct HeadPaint {
        std::vector<double> redYs;                        // solid red polylines
        std::vector<std::pair<double, double> > horns;    // crescent top / bottom
        std::vector<PointF> glyphs;                       // single-codepoint (music-font) glyph origins
    };
    auto paintOf = [&](const StaffLines* lines) {
        std::shared_ptr<BufferedPaintProvider> prv = std::make_shared<BufferedPaintProvider>();
        Painter p(prv, "m8");
        p.setViewport(RectF(0, 0, 4000, 4000));
        PaintOptions opt;
        lines->renderer()->drawItem(lines, &p, opt);
        p.endDraw();
        HeadPaint out;
        const DrawDataPtr dd = prv->drawData();
        std::function<void(const DrawData::Item&)> walk = [&](const DrawData::Item& item) {
            for (const DrawData::Data& d : item.datas) {
                const DrawData::State& state = dd->states.at(d.state);
                for (const DrawPolygon& poly : d.polygons) {
                    if (poly.mode == PolygonMode::Polyline && poly.polygon.size() == 2
                        && state.pen.style() == PenStyle::SolidLine && state.pen.color() == RED
                        && std::abs(poly.polygon[0].y() - poly.polygon[1].y()) < EPS) {
                        out.redYs.push_back(poly.polygon[0].y());
                    }
                }
                for (const DrawPath& path : d.paths) {
                    // The crescent is the only white-filled stroked path.
                    if (path.mode == DrawMode::StrokeAndFill && path.brush.color() == Color::WHITE) {
                        const RectF r = path.path.boundingRect();
                        out.horns.push_back({ r.top(), r.bottom() });
                    }
                }
                for (const DrawText& t : d.texts) {
                    if (t.mode == DrawText::Point && t.text.size() == 1) {
                        out.glyphs.push_back(t.rect.topLeft());
                    }
                }
            }
            for (const DrawData::Item& c : item.chilren) {
                walk(c);
            }
        };
        walk(dd->item);
        return out;
    };
    auto isDoRow = [](double cents, double period) {
        return std::abs(cents - std::round(cents / period) * period) < 1e-3;
    };
    // (c) first half: the Kernel's dot labelled "Do" (its own canonical-solfa
    // name; the lattice is centred on Re, so Do's generator coordinate is the
    // Kernel's to know) sits at 0 cents in both the labelled and the plain
    // dot-stack views.
    auto doGlyph = [&](Score* score, const StaffType* jst, SymId& sym, double& centroidDy) {
        std::vector<jims::LabeledDotStack> labelStacks;
        ASSERT_TRUE(jims::scaleDotLabels(jst->jimsStateJson(), labelStacks));
        int doMembers = 0;
        int doNGen = 0;
        for (const jims::LabeledDotStack& s : labelStacks) {
            for (const jims::LabeledDotMember& member : s.members) {
                if (member.label == u"Do") {
                    ++doMembers;
                    doNGen = member.nGen;
                    EXPECT_NEAR(s.cents, 0.0, 1e-9) << "the Do dot is not at 0 cents";
                }
            }
        }
        ASSERT_EQ(doMembers, 1);
        std::vector<jims::ScaleDotStack> stacks;
        ASSERT_TRUE(jims::scaleDots(jst->jimsStateJson(), stacks));
        int doStacks = 0;
        for (const jims::ScaleDotStack& s : stacks) {
            for (int nGen : s.frontToBack) {
                if (nGen == doNGen) {
                    ++doStacks;
                    EXPECT_NEAR(s.cents, 0.0, 1e-9) << "Do dot stack is not at 0 cents";
                }
            }
        }
        EXPECT_EQ(doStacks, 1);
        muse::String token;
        ASSERT_TRUE(jims::noteheadToken(jst->jimsStateJson(), doNGen, token));
        sym = SymId::noteheadHalf;
        if (token == u"triangle-vertex-up") {
            sym = SymId::noteheadTriangleUpBlack;
        } else if (token == u"triangle-vertex-down") {
            sym = SymId::noteheadTriangleDownBlack;
        } else if (token == u"square-vertex-up") {
            sym = SymId::noteheadDiamondBlack;
        } else if (token == u"square-edge-up") {
            sym = SymId::noteheadSquareBlack;
        }
        const RectF gb = score->engravingFont()->bbox(sym, 1.0);
        centroidDy = 0.0;
        if (sym == SymId::noteheadTriangleUpBlack) {
            centroidDy = -gb.height() / 6.0;
        } else if (sym == SymId::noteheadTriangleDownBlack) {
            centroidDy = gb.height() / 6.0;
        }
    };
    // Every measure of every system: (a); system heads additionally (b), (c).
    auto checkScore = [&](Score* score, const char* what, bool* sawPartial) {
        for (System* system : measureSystems(score)) {
            for (MeasureBase* mb : system->measures()) {
                if (!mb->isMeasure()) {
                    continue;
                }
                Measure* m = toMeasure(mb);
                const StaffType* jst = score->staff(0)->staffType(m->tick());
                ASSERT_TRUE(jst && jst->isJiMS()) << what;
                const StaffType::JimsFrameView& v = jst->jimsFrameView(score, 0, system);
                ASSERT_FALSE(v.empty()) << what;
                const double period = jst->jimsPeriodCents();
                const StaffLines* lines = m->staffLines(0);
                ASSERT_TRUE(lines) << what;
                const double topY = lines->pos().y();
                const double ldSp = jst->lineDistance().val() * lines->spatium();
                auto centsOfY = [&](double y) { return v.centsFromYLd((y - topY) / ldSp); };
                // (a) red = Do row, dashed != Do row; one red line per Do row in a segment.
                int red = 0;
                int expectedRed = 0;
                std::vector<double> redYs;
                for (const StaffLines::JimsGuideLine& g : lines->jimsGuideLines()) {
                    const double cents = centsOfY(g.line.y1());
                    if (!g.dashed && g.rgb == 0xE03030) {
                        ++red;
                        redYs.push_back(g.line.y1());
                        EXPECT_TRUE(isDoRow(cents, period)) << what << " red line at " << cents << " cents";
                    } else {
                        EXPECT_FALSE(isDoRow(cents, period)) << what << " scaffold line on a Do row at " << cents;
                    }
                }
                for (const StaffType::JimsFrameBand& band : v.bands) {
                    for (const StaffType::JimsSegment& seg : band.segments) {
                        if (!seg.whole) {
                            *sawPartial = true;
                        }
                        const double first = std::ceil((seg.lowerCents - 1e-6) / period) * period;
                        for (double b = first; b <= seg.upperCents + 1e-6; b += period) {
                            ++expectedRed;
                        }
                    }
                }
                EXPECT_EQ(red, expectedRed) << what << " tick " << m->tick().ticks();
                if (system->firstMeasure() != m) {
                    continue;
                }
                // (b) horns are Do rows; every red line has a horn on it.
                const HeadPaint paint = paintOf(lines);
                EXPECT_EQ(paint.redYs.size(), redYs.size()) << what;
                size_t segments = 0;
                for (const StaffType::JimsFrameBand& band : v.bands) {
                    segments += band.segments.size();
                }
                EXPECT_EQ(paint.horns.size(), segments) << what << " one crescent per segment";
                for (const auto& h : paint.horns) {
                    EXPECT_TRUE(isDoRow(centsOfY(h.first), period)) << what << " upper horn off Do";
                    EXPECT_TRUE(isDoRow(centsOfY(h.second), period)) << what << " lower horn off Do";
                }
                for (double y : redYs) {
                    bool hornOnLine = false;
                    for (const auto& h : paint.horns) {
                        hornOnLine = hornOnLine || std::abs(h.first - y) < 1e-6 || std::abs(h.second - y) < 1e-6;
                    }
                    EXPECT_TRUE(hornOnLine) << what << " no crescent horn on the Do line at "
                                            << centsOfY(y) << " cents";
                }
                // (c) the Do glyph sits on every drawn red line.
                SymId doSym = SymId::noSym;
                double doDy = 0.0;
                doGlyph(score, jst, doSym, doDy);
                // The glyph's recorded origin is the draw call's point (the
                // header draws at mag 1); the drawn codepoint is the engraving
                // font's business (a fallback font may substitute an alternate),
                // so the match is by position: in the dot column, on the row.
                for (double y : redYs) {
                    bool glyphOnLine = false;
                    for (const PointF& g : paint.glyphs) {
                        glyphOnLine = glyphOnLine
                                      || (g.x() < lines->pos().x() && std::abs(g.y() - (y + doDy)) < 1e-6);
                    }
                    EXPECT_TRUE(glyphOnLine) << what << " no Do dot on the Do line at " << centsOfY(y) << " cents";
                }
            }
        }
    };
    // Off-Do partial staves (La-mode, tonic-bounded, with a mid-piece
    // change back to Do-mode) at three generator widths.
    for (double g : { 686.0, 700.0, 720.0 }) {
        io::File f(ScoreRW::rootPath() + u"/jimstaff_data/m5-syshead.mscx");
        ASSERT_TRUE(f.open(io::IODevice::ReadOnly));
        String text = String::fromUtf8(f.readAll());
        text.replace(u"\"generator_cents\":700.0", u"\"generator_cents\":" + String::number(g, 1));
        const String dir = ScoreRW::rootPath() + u"/../../../build.release/jims-m8-scratch";
        io::Dir::mkpath(dir);
        const String path = dir + u"/m8-do-row-net-" + String::number(g, 0) + u".mscx";
        io::File out(path);
        ASSERT_TRUE(out.open(io::IODevice::WriteOnly));
        out.write(text.toUtf8());
        out.close();
        MasterScore* score = ScoreRW::readScore(path, true);
        ASSERT_TRUE(score) << g;
        score->doLayout();
        double generator = 0.0, period = 0.0;
        ASSERT_TRUE(jims::staffMetrics(st(score)->jimsStateJson(), generator, period));
        EXPECT_NEAR(generator, g, 1e-9);
        double tonic = 0.0;
        ASSERT_TRUE(jims::tonicCentsAboveDo(st(score)->jimsStateJson(), tonic));
        EXPECT_GT(tonic, 1.0) << "the fixture must be off-Do (La-mode)";
        bool sawPartial = false;
        const std::string what = "syshead@" + std::to_string(int(g));
        checkScore(score, what.c_str(), &sawPartial);
        EXPECT_TRUE(sawPartial) << what << " must exercise partial staves";
        delete score;
    }
    // The M7 gate terrain (bar 2: La-mode, reference 53).
    {
        MasterScore* gate = ScoreRW::readScore(u"jimstaff_data/m7-gate.mscz");
        ASSERT_TRUE(gate);
        gate->doLayout();
        bool sawPartial = false;
        checkScore(gate, "m7-gate", &sawPartial);
        delete gate;
    }
    // A banded hollow stack: Do rows in every band.
    {
        MasterScore* score = ScoreRW::readScore(TWO_HAND);
        ASSERT_TRUE(score);
        score->doLayout();
        setElision(score, true);
        bool banded = false;
        for (System* system : measureSystems(score)) {
            banded = banded || viewOn(score, system).banded;
        }
        ASSERT_TRUE(banded);
        bool sawPartial = false;
        checkScore(score, "two-hand banded", &sawPartial);
        delete score;
    }
}

// MuseScore's stock hide-empty-staves still hides a fully empty JiMStaff on
// a system, unchanged, whether or not elision is on.
TEST_F(Engraving_JiMStaffM8BandElisionTests, m8StockHideEmptyStavesStillHidesAnEmptyJiMStaff)
{
    MasterScore* score = ScoreRW::readScore(TWO_STAVES);
    ASSERT_TRUE(score);
    score->doLayout();
    ASSERT_EQ(score->nstaves(), 2u);
    for (bool elide : { false, true }) {
        score->style().set(Sid::jimsElideEmptyOctaves, elide);
        score->style().set(Sid::hideEmptyStaves, true);
        score->setLayoutAll();
        score->doLayout();
        const std::vector<System*> systems = measureSystems(score);
        ASSERT_EQ(systems.size(), 4u) << elide;
        EXPECT_TRUE(systems[0]->staff(1)->show()) << elide;    // first system: dontHideStavesInFirstSystem
        for (size_t i = 1; i < systems.size(); ++i) {
            EXPECT_FALSE(systems[i]->staff(1)->show()) << "system " << i + 1 << " elide=" << elide;
            EXPECT_TRUE(systems[i]->staff(0)->show());
        }
    }
    delete score;
}

// .mscz round trip preserves the two styles and the staff-type override;
// an absent override reads as Auto; unknown values read as Auto.
TEST_F(Engraving_JiMStaffM8BandElisionTests, m8RoundTripPreservesSwitchesAndAbsentOverrideIsAuto)
{
    MasterScore* score = ScoreRW::readScore(TWO_HAND);
    ASSERT_TRUE(score);
    score->doLayout();
    EXPECT_EQ(st(score)->jimsElideOctaves(), JimsElideOctaves::Auto);
    score->style().set(Sid::jimsElideEmptyOctaves, true);
    score->style().set(Sid::jimsShowAllOctavesInFirstSystem, false);
    mutSt(score)->setJimsElideOctaves(JimsElideOctaves::On);
    const String dir = ScoreRW::rootPath() + u"/../../../build.release/jims-m8-scratch";
    io::Dir::mkpath(dir);
    // A real .mscz container (MscSaver -> MscWriter zip), read back through
    // the ordinary reader.
    const String out = dir + u"/m8-roundtrip.mscz";
    io::File::remove(out);
    {
        io::File file(out);
        ASSERT_TRUE(file.open(io::IODevice::WriteOnly));
        MscWriter::Params params;
        params.device = &file;
        params.filePath = out;
        params.mode = MscIoMode::Zip;
        MscWriter writer(params);
        ASSERT_TRUE(writer.open());
        MscSaver saver(score->iocContext());
        ASSERT_TRUE(saver.writeMscz(score, writer, false));
        writer.close();
        file.close();
    }
    delete score;
    MasterScore* again = ScoreRW::readScore(out, true);
    ASSERT_TRUE(again);
    again->doLayout();
    EXPECT_TRUE(again->style().styleB(Sid::jimsElideEmptyOctaves));
    EXPECT_FALSE(again->style().styleB(Sid::jimsShowAllOctavesInFirstSystem));
    EXPECT_EQ(st(again)->jimsElideOctaves(), JimsElideOctaves::On);
    // Off round-trips too.
    mutSt(again)->setJimsElideOctaves(JimsElideOctaves::Off);
    const String out2 = dir + u"/m8-roundtrip-off.mscx";
    ASSERT_TRUE(ScoreRW::saveScore(again, out2));
    delete again;
    MasterScore* third = ScoreRW::readScore(out2, true);
    ASSERT_TRUE(third);
    EXPECT_EQ(st(third)->jimsElideOctaves(), JimsElideOctaves::Off);
    delete third;
    // Unknown value -> Auto (safe parsing, no invented behaviour).
    io::File f(out2);
    ASSERT_TRUE(f.open(io::IODevice::ReadOnly));
    String text = String::fromUtf8(f.readAll());
    f.close();
    ASSERT_TRUE(text.contains(u"<jimsElideOctaves>off</jimsElideOctaves>"));
    text.replace(u"<jimsElideOctaves>off</jimsElideOctaves>", u"<jimsElideOctaves>banana</jimsElideOctaves>");
    const String out3 = dir + u"/m8-roundtrip-unknown.mscx";
    io::File w(out3);
    ASSERT_TRUE(w.open(io::IODevice::WriteOnly));
    w.write(text.toUtf8());
    w.close();
    MasterScore* fourth = ScoreRW::readScore(out3, true);
    ASSERT_TRUE(fourth);
    EXPECT_EQ(st(fourth)->jimsElideOctaves(), JimsElideOctaves::Auto);
    delete fourth;
}

// None of the three settings enters the Kernel state; the notes' identities
// and Kernel sounding pitches (what playback consumes) are unchanged.
TEST_F(Engraving_JiMStaffM8BandElisionTests, m8SettingsNeverEnterKernelStateAndPlaybackIdentityIsUnchanged)
{
    MasterScore* score = ScoreRW::readScore(TWO_HAND);
    ASSERT_TRUE(score);
    score->doLayout();
    const String stateBefore = st(score)->jimsStateJson();
    struct Ident {
        int nPer;
        int nGen;
        int midi;
        double cents;
    };
    auto collect = [&]() {
        std::vector<Ident> out;
        for (Measure* m = score->firstMeasure(); m; m = m->nextMeasure()) {
            for (Chord* c : chordsOf(m)) {
                for (Note* n : c->notes()) {
                    jims::SoundingPitch sp;
                    EXPECT_TRUE(jims::noteSoundingPitch(st(score)->jimsStateJson(), n->jimsNPer(), n->jimsNGen(), sp));
                    out.push_back({ n->jimsNPer(), n->jimsNGen(), sp.midiKey, sp.centsOffset });
                }
            }
        }
        return out;
    };
    const std::vector<Ident> before = collect();
    ASSERT_EQ(before.size(), 32u);
    score->style().set(Sid::jimsElideEmptyOctaves, true);
    score->style().set(Sid::jimsShowAllOctavesInFirstSystem, false);
    mutSt(score)->setJimsElideOctaves(JimsElideOctaves::On);
    score->setLayoutAll();
    score->doLayout();
    EXPECT_EQ(st(score)->jimsStateJson(), stateBefore);
    EXPECT_FALSE(stateBefore.contains(u"elide"));
    const std::vector<Ident> after = collect();
    ASSERT_EQ(after.size(), before.size());
    for (size_t i = 0; i < before.size(); ++i) {
        EXPECT_EQ(after[i].nPer, before[i].nPer);
        EXPECT_EQ(after[i].nGen, before[i].nGen);
        EXPECT_EQ(after[i].midi, before[i].midi);
        EXPECT_NEAR(after[i].cents, before[i].cents, EPS);
    }
    delete score;
}
// Phase 4 (optional, screen-only): the "n octaves elided" gap indicator
// reuses the StaffVisibilityIndicator paint precedent (TDraw::draw(const
// IndicatorIcon*): drawn only when !isPrinting && showUnprintable) — it is
// present on screen for a banded system head, absent from every printing
// paint path (PNG/PDF/print/SVG all paint with isPrinting), absent when the
// score hides unprintables, and absent when elision is off. Its text comes
// from the Kernel's omitted-period count.
TEST_F(Engraving_JiMStaffM8BandElisionTests, m8GapIndicatorIsScreenOnlyAndNeverPrints)
{
    MasterScore* score = ScoreRW::readScore(TWO_HAND);
    ASSERT_TRUE(score);
    score->doLayout();
    struct Drawn {
        String text;
        double x;
    };
    auto drawnOf = [](const StaffLines* lines, bool printing) {
        std::shared_ptr<BufferedPaintProvider> prv = std::make_shared<BufferedPaintProvider>();
        Painter p(prv, "m8");
        p.setViewport(RectF(0, 0, 4000, 4000));
        PaintOptions opt;
        opt.isPrinting = printing;
        lines->renderer()->drawItem(lines, &p, opt);
        p.endDraw();
        std::vector<Drawn> out;
        std::function<void(const DrawData::Item&)> walk = [&](const DrawData::Item& item) {
            for (const DrawData::Data& d : item.datas) {
                for (const DrawText& t : d.texts) {
                    out.push_back({ t.text, t.rect.left() });
                }
            }
            for (const DrawData::Item& c : item.chilren) {
                walk(c);
            }
        };
        walk(prv->drawData()->item);
        return out;
    };
    auto textsOf = [&](const StaffLines* lines, bool printing) {
        std::vector<String> out;
        for (const Drawn& d : drawnOf(lines, printing)) {
            out.push_back(d.text);
        }
        return out;
    };
    auto hasIndicator = [](const std::vector<String>& texts) {
        for (const String& t : texts) {
            if (t.contains(u"hidden")) {
                return true;
            }
        }
        return false;
    };
    // Elision off: nothing to indicate.
    const StaffLines* offLines = measureSystems(score)[1]->firstMeasure()->staffLines(0);
    EXPECT_FALSE(hasIndicator(textsOf(offLines, false)));
    setElision(score, true);
    System* system2 = measureSystems(score)[1];
    ASSERT_EQ(viewOn(score, system2).bands.size(), 2u);
    const StaffLines* lines = system2->firstMeasure()->staffLines(0);
    const std::vector<String> screen = textsOf(lines, false);
    ASSERT_TRUE(hasIndicator(screen));
    bool sawCount = false;
    for (const String& t : screen) {
        if (t.contains(u"hidden")) {
            EXPECT_EQ(t, u"3 empty octaves hidden");   // owner wording 2026-08-18
            sawCount = true;
        }
    }
    EXPECT_TRUE(sawCount);
    // Placement (owner finding 2026-08-18): the text's left edge sits at the
    // right edge of the scale-dot column (no Split-mode right label stack in
    // this fixture), i.e. dotCenterX + indicatorW of the header geometry —
    // left of the crescent, never at the measure's start.
    {
        const StaffType::JimsFrameView& v = viewOn(score, system2);
        const StaffType::JimsHeaderGeometry g
            = st(score)->jimsHeaderGeometry(lines->spatium(), score->style().defaultSpatium(), &v);
        const double sp = lines->spatium();
        const double clefRight = lines->pos().x() - 0.3 * sp;
        const double clefLeft = clefRight - g.clefRx;
        const double dotCenterX = clefLeft - g.rightLabelBand - 2.0 * g.indicatorW + g.indicatorW;
        const double expectedLeft = dotCenterX + g.indicatorW + g.rightLabelBand;
        bool sawPlacement = false;
        for (const Drawn& d : drawnOf(lines, false)) {
            if (d.text.contains(u"hidden")) {
                EXPECT_NEAR(d.x, expectedLeft, 1e-6);
                EXPECT_LT(d.x, lines->pos().x());   // left of the staff's first measure
                sawPlacement = true;
            }
        }
        EXPECT_TRUE(sawPlacement);
    }
    // Printing paths never see it ...
    EXPECT_FALSE(hasIndicator(textsOf(lines, true)));
    // ... nor does the screen when unprintables are hidden ...
    score->setShowUnprintable(false);
    EXPECT_FALSE(hasIndicator(textsOf(lines, false)));
    score->setShowUnprintable(true);
    // ... nor a non-head measure of the system.
    EXPECT_FALSE(hasIndicator(textsOf(system2->lastMeasure()->staffLines(0), false)));
    // System 1 (whole stack under the first-system rule): nothing elided, no text.
    EXPECT_FALSE(hasIndicator(textsOf(measureSystems(score)[0]->firstMeasure()->staffLines(0), false)));
    delete score;
}
} // namespace
