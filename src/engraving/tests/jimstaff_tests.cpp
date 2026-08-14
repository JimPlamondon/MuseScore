/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 *
 * JiMStaff Milestone 2 Phase 6 — the CLI round-trip checks of Milestones
 * 1–2, ported into the standard engraving_tests binary. Every musical
 * expectation below is a Kernel-pinned fact (the jims repo's contract
 * tests pin the same numbers); these tests prove the fork's bridge seam
 * delivers those facts unchanged, at three tunings, on the continuous
 * cents axis (owner ruling 2026-08-14: positions are cents, never
 * location counts).
 */

#include <gtest/gtest.h>

#include "engraving/jims/jimsbridge.h"

#include "engraving/iengravingfontsprovider.h"
#include "modularity/ioc.h"

using namespace mu::engraving;

namespace {
// The diatonic reference state at a given generator width. Register-4
// two-period extent, Do-mode unless rotated; matches the Kernel contract
// fixtures byte-for-byte in the fields that matter.
muse::String jimsState(double generatorCents, int modeRotation = 0)
{
    return muse::String(
        u"{\"scale\":[\"M2\",\"m2\",\"M2\",\"M2\",\"M2\",\"m2\",\"M2\"],"
        u"\"collection_rotation\":0,\"mode_rotation\":%1,"
        u"\"generator_cents\":%2,\"period_cents\":1200.0,"
        u"\"embedding\":{\"large_steps\":5,\"small_steps\":2},"
        u"\"extent\":{\"lower_do_register\":4,\"period_count\":2},"
        u"\"reference\":\"none\"}")
           .arg(modeRotation)
           .arg(muse::String::number(generatorCents, 12));
}

constexpr double G12 = 700.0;
constexpr double G17 = 1200.0 * 10.0 / 17.0; // 705.882352...
constexpr double G19 = 1200.0 * 11.0 / 19.0; // 694.736842...
constexpr double EPS = 1e-6;
}

// The bridge speaks ABI 2 (JiMStaffStateV2 contract, Milestone 2 Phase 4).
TEST(JiMStaffTests, bridgeSpeaksAbi2)
{
    EXPECT_TRUE(jims::available());
}

// Cents positions at three tunings: the SAME lattice identities, whose
// heights move with the generator. Relative to the staff's lower Do
// (C4 = (1,-2), abs 1200 - 2g): G4 = (1,-1) sits exactly one generator
// up, and E4 = (-1,2) (abs 2g - 1200) sits 4g - 2400 up — 400 at
// 12-TET, 423.53 at 17-TET, 378.95 at 19-TET. Tuning-variant cents,
// tuning-invariant identities.
TEST(JiMStaffTests, centsPositionsFollowTheGeneratorAcrossTunings)
{
    for (double g : { G12, G17, G19 }) {
        muse::String state = jimsState(g);
        double cents = 0.0;
        // C4 = (1,-2): the lower Do of the register-4 staff, at 0 by anchor.
        ASSERT_TRUE(jims::noteCentsAboveDo(state, 1, -2, cents));
        EXPECT_NEAR(cents, 0.0, EPS);
        // G4 = (1,-1): exactly one generator up.
        ASSERT_TRUE(jims::noteCentsAboveDo(state, 1, -1, cents));
        EXPECT_NEAR(cents, g, EPS);
        // E4 = (-1,2): four generators above the lower Do once the
        // anchor's own motion is counted.
        ASSERT_TRUE(jims::noteCentsAboveDo(state, -1, 2, cents));
        EXPECT_NEAR(cents, 4.0 * g - 2400.0, EPS);
    }
}

// Entry round-trip: standard spelling -> Kernel identity -> cents ->
// nearest realizable pitch recovers the identity and the compatibility
// spelling, at all three tunings.
TEST(JiMStaffTests, entryAndQuantizationRoundTrip)
{
    int nPer = 0, nGen = 0;
    ASSERT_TRUE(jims::entryFromStandardPitch('C', 0, 4, nPer, nGen));
    EXPECT_EQ(nPer, 1);
    EXPECT_EQ(nGen, -2);

    for (double g : { G12, G17, G19 }) {
        muse::String state = jimsState(g);
        double cents = 0.0;
        ASSERT_TRUE(jims::noteCentsAboveDo(state, nPer, nGen, cents));
        jims::PitchHit hit;
        ASSERT_TRUE(jims::nearestPitch(state, cents, true, nPer, nGen, hit));
        EXPECT_EQ(hit.nPer, 1);
        EXPECT_EQ(hit.nGen, -2);
        EXPECT_EQ(hit.step, 'C');
        EXPECT_EQ(hit.alter, 0);
        EXPECT_EQ(hit.octave, 4);
    }
}

// Tuning-true quantization (Kernel-pinned): the same physical 360-cent
// drag height lands on DIFFERENT lattice identities as the generator
// moves — Mi (nGen 2) at 12-TET and 19-TET, Ri (nGen 7) at 17-TET.
TEST(JiMStaffTests, dragTargetsAreTuningTrueNeverTwelveTetArithmetic)
{
    jims::PitchHit hit;
    ASSERT_TRUE(jims::nearestPitch(jimsState(G12), 360.0, false, 0, 0, hit));
    EXPECT_EQ(hit.nGen, 2) << "12-TET: Mi at 400";
    ASSERT_TRUE(jims::nearestPitch(jimsState(G17), 360.0, false, 0, 0, hit));
    EXPECT_EQ(hit.nGen, 7) << "17-TET: Ri at 352.94";
    ASSERT_TRUE(jims::nearestPitch(jimsState(G19), 360.0, false, 0, 0, hit));
    EXPECT_EQ(hit.nGen, 2) << "19-TET: Mi at 378.95";
}

// Partial staves (patent mechanism, J4.001): the God-Rest-Ye shape —
// La-mode, melody bounded by La instances A4..A5 — yields two partial
// staves cut 100 cents beyond each bounding La: [800,1200] and
// [1200,2200], in cents, Kernel-derived.
TEST(JiMStaffTests, tonicBoundedLaModeFrameGetsPartialStaves)
{
    muse::String state = jimsState(G12, 5); // mode_rotation 5 selects La
    muse::String melody =
        u"{\"notes\":[{\"nPer\":0,\"nGen\":1},{\"nPer\":-1,\"nGen\":3},"
        u"{\"nPer\":2,\"nGen\":-2},{\"nPer\":1,\"nGen\":0},{\"nPer\":1,\"nGen\":1}]}";
    std::vector<jims::StaveSegment> segments;
    ASSERT_TRUE(jims::frameForMelody(state, melody, u"tonic-bounded", segments));
    ASSERT_EQ(segments.size(), 2u);
    EXPECT_FALSE(segments[0].whole);
    EXPECT_NEAR(segments[0].lowerCents, 800.0, EPS);
    EXPECT_NEAR(segments[0].upperCents, 1200.0, EPS);
    EXPECT_FALSE(segments[1].whole);
    EXPECT_NEAR(segments[1].lowerCents, 1200.0, EPS);
    EXPECT_NEAR(segments[1].upperCents, 2200.0, EPS);
}

// The tuning metrics seam feeding the "M5= <cents>¢" label reports the
// state's own widths, never a fork-side constant.
TEST(JiMStaffTests, staffMetricsReportTheStateWidths)
{
    double generatorCents = 0.0, periodCents = 0.0;
    ASSERT_TRUE(jims::staffMetrics(jimsState(G19), generatorCents, periodCents));
    EXPECT_NEAR(generatorCents, G19, EPS);
    EXPECT_NEAR(periodCents, 1200.0, EPS);
}

// Negative control: invalid state fails VISIBLY (every wrapper returns
// false) — never a silent fall-back to 12-TET arithmetic. The scale
// below breaks the MOS embedding contract.
TEST(JiMStaffTests, invalidStateFailsVisiblyNotSilently)
{
    muse::String bad =
        u"{\"scale\":[\"M2\",\"M2\"],\"collection_rotation\":0,\"mode_rotation\":0,"
        u"\"generator_cents\":700.0,\"period_cents\":1200.0,"
        u"\"embedding\":{\"large_steps\":5,\"small_steps\":2},"
        u"\"extent\":{\"lower_do_register\":4,\"period_count\":2},"
        u"\"reference\":\"none\"}";
    double cents = 0.0;
    EXPECT_FALSE(jims::noteCentsAboveDo(bad, 1, -2, cents));
    double generatorCents = 0.0, periodCents = 0.0;
    EXPECT_FALSE(jims::staffMetrics(bad, generatorCents, periodCents));
    std::vector<jims::StaveSegment> segments;
    EXPECT_FALSE(jims::frameForMelody(bad, u"{\"notes\":[{\"nPer\":1,\"nGen\":-2}]}",
                                      u"tonic-bounded", segments));
    jims::PitchHit hit;
    EXPECT_FALSE(jims::nearestPitch(bad, 0.0, false, 0, 0, hit));
}

// M3 Phase 3: the Kernel-generated JiMSMusic font is registered, its
// class-notehead outlines are present and nonempty, and they genuinely
// differ from the fallback font's stock outlines (outline change only —
// position/size/selection logic is untouched from M1/M2).
TEST(JiMStaffTests, jimsMusicFontRegisteredWithKernelOutlines)
{
    auto provider = muse::modularity::globalIoc()->resolve<IEngravingFontsProvider>("jimstaff_tests");
    ASSERT_TRUE(provider);
    IEngravingFontPtr jimsFont = provider->fontByName("JiMSMusic");
    ASSERT_TRUE(jimsFont) << "JiMSMusic must be registered at engraving-module init";
    IEngravingFontPtr fallback = provider->fallbackFont();
    ASSERT_TRUE(fallback);
    for (SymId sym : { SymId::noteheadTriangleUpBlack, SymId::noteheadTriangleDownBlack,
                       SymId::noteheadDiamondBlack, SymId::noteheadSquareBlack }) {
        muse::RectF jb = jimsFont->bbox(sym, 1.0);
        EXPECT_FALSE(jb.isNull()) << "JiMSMusic outline empty";
        muse::RectF fb = fallback->bbox(sym, 1.0);
        EXPECT_TRUE(jb != fb) << "outline identical to fallback - registration had no effect";
    }
}
