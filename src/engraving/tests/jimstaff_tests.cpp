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

#include <cstdlib>
#include <fstream>

#include "engraving/dom/masterscore.h"
#include "engraving/editing/undo.h"
#include "engraving/dom/measure.h"
#include "engraving/dom/note.h"
#include "engraving/dom/segment.h"
#include "engraving/dom/staff.h"
#include "engraving/dom/stafftype.h"
#include "engraving/jims/jimsbridge.h"
#include "engraving/jims/jimstuningcontroller.h"

#include "utils/scorerw.h"

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

namespace {
constexpr double G19T = 1200.0 * 11.0 / 19.0;

double stateGeneratorCents(const StaffType* st)
{
    double g = 0.0, p = 0.0;
    return jims::staffMetrics(st->jimsStateJson(), g, p) ? g : -1.0;
}
}

// M3 Phase 4: the shared tuning controller. Preview mutates every JiMS
// state span transiently (no undo entries); commit lands exactly one
// undoable edit; cancel restores the pre-drag state. Every consumer of
// the seam re-derives — no stale notes, frames, or labels.
TEST(JiMStaffTests, tuningControllerPreviewCommitCancelUndo)
{
    Score* score = ScoreRW::readScore(u"jimstaff_data/mode-change.mscx");
    ASSERT_TRUE(score);
    Staff* staff = score->staff(0);
    ASSERT_TRUE(staff);
    const StaffType* base = staff->staffType(Fraction(0, 1));
    ASSERT_TRUE(base && base->isJiMS());
    EXPECT_NEAR(stateGeneratorCents(base), 700.0, 1e-9);

    jims::TuningController controller(score, 0);
    EXPECT_NEAR(controller.currentGeneratorCents(), 700.0, 1e-9);

    // Preview: every span updated, zero undo entries.
    const size_t undoBefore = score->undoStack()->size();
    ASSERT_TRUE(controller.beginPreview());
    ASSERT_TRUE(controller.preview(G19T));
    EXPECT_NEAR(stateGeneratorCents(staff->staffType(Fraction(0, 1))), G19T, 1e-9);
    EXPECT_EQ(score->undoStack()->size(), undoBefore) << "preview must not create undo entries";

    // Cancel restores the pre-drag value.
    controller.cancel();
    EXPECT_NEAR(stateGeneratorCents(staff->staffType(Fraction(0, 1))), 700.0, 1e-9);

    // Commit: exactly ONE undoable edit; undo restores; redo re-applies.
    ASSERT_TRUE(controller.beginPreview());
    ASSERT_TRUE(controller.preview(690.0));
    ASSERT_TRUE(controller.commit(690.0));
    EXPECT_EQ(score->undoStack()->size(), undoBefore + 1) << "commit is one undoable edit";
    EXPECT_NEAR(stateGeneratorCents(staff->staffType(Fraction(0, 1))), 690.0, 1e-9);
    score->undoRedo(true, nullptr);
    EXPECT_NEAR(stateGeneratorCents(staff->staffType(Fraction(0, 1))), 700.0, 1e-9);
    score->undoRedo(false, nullptr);
    EXPECT_NEAR(stateGeneratorCents(staff->staffType(Fraction(0, 1))), 690.0, 1e-9);
    delete score;
}

// The update reaches every span (base StaffType AND the measure-boundary
// StaffTypeChange), replacing only generator_cents — mode rotation and
// the per-span tonic extents survive untouched.
TEST(JiMStaffTests, tuningControllerUpdatesEverySpanPreservingIdentity)
{
    Score* score = ScoreRW::readScore(u"jimstaff_data/mode-change.mscx");
    ASSERT_TRUE(score);
    Staff* staff = score->staff(0);
    // The fixture's bar-5 StaffTypeChange carries mode_rotation 5.
    const StaffType* changed = staff->staffType(Fraction(16, 4));
    ASSERT_TRUE(changed && changed->isJiMS());
    ASSERT_TRUE(changed->jimsStateJson().contains(u"\"mode_rotation\":5"));

    jims::TuningController controller(score, 0);
    ASSERT_TRUE(controller.beginPreview());
    ASSERT_TRUE(controller.preview(G19T));
    const StaffType* baseAfter = staff->staffType(Fraction(0, 1));
    const StaffType* changedAfter = staff->staffType(Fraction(16, 4));
    EXPECT_NEAR(stateGeneratorCents(baseAfter), G19T, 1e-9);
    EXPECT_NEAR(stateGeneratorCents(changedAfter), G19T, 1e-9);
    EXPECT_TRUE(changedAfter->jimsStateJson().contains(u"\"mode_rotation\":5"));
    EXPECT_TRUE(baseAfter->jimsStateJson().contains(u"\"mode_rotation\":0"));
    EXPECT_TRUE(baseAfter->jimsStateJson().contains(u"tonic-bounded"));
    controller.cancel();
    delete score;
}

// Note heights are cents through the single seam: after a preview to a
// new tuning, a pinned note's cached cents re-derive to the new value —
// the cache invalidation the controller performs is what keeps notes and
// staff on one map (VTR boundary crossing exercised in both directions).
TEST(JiMStaffTests, tuningControllerRederivesNoteCentsAcrossVtrBoundary)
{
    Score* score = ScoreRW::readScore(u"jimstaff_data/mode-change.mscx");
    ASSERT_TRUE(score);
    Staff* staff = score->staff(0);
    // First note of the piece: C4 = (1,-2), 0 cents at any tuning.
    // Find a G (1,-1): one generator up — tuning-variant.
    Note* gNote = nullptr;
    for (Segment* seg = score->firstSegment(SegmentType::ChordRest); seg && !gNote;
         seg = seg->next1(SegmentType::ChordRest)) {
        EngravingItem* el = seg->element(0);
        if (el && el->isChord()) {
            for (Note* note : toChord(el)->notes()) {
                if (note->jimsNPer() == 1 && note->jimsNGen() == -1) {
                    gNote = note;
                    break;
                }
            }
        }
    }
    ASSERT_TRUE(gNote) << "fixture must contain G4 = (1,-1)";
    score->doLayout();
    EXPECT_NEAR(gNote->jimsCentsAboveDo(), 700.0, 1e-9);

    jims::TuningController controller(score, 0);
    for (double g : { G19T - 0.01, G19T, G19T + 0.01, 700.0 }) {
        ASSERT_TRUE(controller.beginPreview());
        ASSERT_TRUE(controller.preview(g));
        score->doLayout();
        EXPECT_NEAR(gNote->jimsCentsAboveDo(), g, 1e-9)
            << "note cents must re-derive at generator " << g;
        controller.cancel();
    }
    score->doLayout();
    EXPECT_NEAR(gNote->jimsCentsAboveDo(), 700.0, 1e-9);
    delete score;
}

// M3 Phase 6: the scripted evidence driver — the same shared controller
// the panel uses, swept 680->720 cents and back, three round trips per
// piece after warm-up, with exact samples at every semantic boundary.
// Gated by JIMS_SWEEP=1 (artifact dir in JIMS_SWEEP_OUT) so the normal
// fence stays fast. Semantic artifacts are deterministic; timing is
// reported separately and statistically.
TEST(JiMStaffTests, evidenceSweepAcceptancePieces)
{
    const char* gate = std::getenv("JIMS_SWEEP");
    if (!gate || muse::String::fromUtf8(gate) != u"1") {
        GTEST_SKIP() << "set JIMS_SWEEP=1 to run the evidence sweep";
    }
    const char* outDir = std::getenv("JIMS_SWEEP_OUT");
    ASSERT_TRUE(outDir) << "JIMS_SWEEP_OUT must name the artifact directory";

    const double G17T = 1200.0 * 10.0 / 17.0;
    const std::vector<double> exactSamples
        = { 680.0, G19T - 0.01, G19T, G19T + 0.01, 700.0, G17T, 720.0 };
    const std::vector<muse::String> pieces
        = { u"collision", u"ode-to-joy", u"acc-chromatic", u"mode-change", u"grym" };

    for (const muse::String& piece : pieces) {
        Score* score = ScoreRW::readScore(u"jimstaff_data/" + piece + u".mscx");
        ASSERT_TRUE(score) << piece.toStdString();
        score->doLayout();
        jims::TuningController controller(score, 0);
        ASSERT_NEAR(controller.currentGeneratorCents(), 700.0, 1e-9);

        auto captureSemantics = [&](std::ofstream& sem, double g) {
            const StaffType* st = score->staff(0)->staffType(Fraction(0, 1));
            std::vector<jims::JiLine> lines;
            jims::jiLines(st->jimsStateJson(), lines);
            int visible = 0;
            for (const jims::JiLine& line : lines) {
                visible += line.visible ? 1 : 0;
            }
            double mg = 0.0, mp = 0.0;
            jims::staffMetrics(st->jimsStateJson(), mg, mp);
            sem << "{\"g\":" << muse::String::number(g, 6).toStdString()
                << ",\"metrics_g\":" << muse::String::number(mg, 6).toStdString()
                << ",\"visible_ji_lines\":" << visible
                << ",\"frame\":[";
            bool firstSeg = true;
            for (const StaffType::JimsSegment& seg : st->jimsFrameSegments()) {
                sem << (firstSeg ? "" : ",") << "["
                    << muse::String::number(seg.lowerCents, 4).toStdString() << ","
                    << muse::String::number(seg.upperCents, 4).toStdString() << ","
                    << (seg.whole ? 1 : 0) << "]";
                firstSeg = false;
            }
            sem << "],\"notes\":[";
            bool firstNote = true;
            for (Segment* seg = score->firstSegment(SegmentType::ChordRest); seg;
                 seg = seg->next1(SegmentType::ChordRest)) {
                EngravingItem* el = seg->element(0);
                if (el && el->isChord()) {
                    for (Note* note : toChord(el)->notes()) {
                        if (note->hasJimsPitch()) {
                            sem << (firstNote ? "" : ",") << "["
                                << note->jimsNPer() << "," << note->jimsNGen() << ","
                                << muse::String::number(note->jimsCentsAboveDo(), 4).toStdString() << "]";
                            firstNote = false;
                        }
                    }
                }
            }
            sem << "]}\n";
        };

        std::ofstream sem(std::string(outDir) + "/" + piece.toStdString() + "-semantics.jsonl");
        std::ofstream lat(std::string(outDir) + "/" + piece.toStdString() + "-latency.txt");
        std::vector<double> applied;

        // Warm-up: one full round trip, uncaptured.
        ASSERT_TRUE(controller.beginPreview());
        for (int i = 0; i <= 80; ++i) {
            ASSERT_TRUE(controller.preview(680.0 + 0.5 * i));
        }
        for (int i = 80; i >= 0; --i) {
            ASSERT_TRUE(controller.preview(680.0 + 0.5 * i));
        }
        // Three captured forward-and-back sweeps.
        for (int sweep = 0; sweep < 3; ++sweep) {
            for (int i = 0; i <= 160; ++i) {
                const double g = (i <= 80) ? 680.0 + 0.5 * i : 720.0 - 0.5 * (i - 80);
                ASSERT_TRUE(controller.preview(g));
                applied.push_back(controller.lastApplyMs());
            }
        }
        // Exact boundary samples, semantics captured (sweep-invariant).
        for (double g : exactSamples) {
            ASSERT_TRUE(controller.preview(g));
            applied.push_back(controller.lastApplyMs());
            captureSemantics(sem, g);
            const StaffType* st = score->staff(0)->staffType(Fraction(0, 1));
            double mg = 0.0, mp = 0.0;
            ASSERT_TRUE(jims::staffMetrics(st->jimsStateJson(), mg, mp));
            ASSERT_NEAR(mg, g, 1e-9) << "no dropped/stale applied state";
        }
        controller.cancel();
        score->doLayout();
        ASSERT_NEAR(controller.currentGeneratorCents(), 700.0, 1e-9);

        std::sort(applied.begin(), applied.end());
        const size_t n = applied.size();
        lat << "piece=" << piece.toStdString()
            << " ticks=" << n
            << " median_ms=" << applied[n / 2]
            << " p95_ms=" << applied[n * 95 / 100]
            << " max_ms=" << applied[n - 1]
            << " dropped=0\n";
        // The acceptance floor: p95 <= 33.3ms, max <= 50ms per piece.
        EXPECT_LE(applied[n * 95 / 100], 33.3) << piece.toStdString();
        EXPECT_LE(applied[n - 1], 50.0) << piece.toStdString();
        delete score;
    }
}
