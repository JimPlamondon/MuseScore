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
#include "engraving/dom/chord.h"
#include "engraving/dom/note.h"
#include "engraving/dom/stem.h"
#include "engraving/dom/segment.h"
#include "engraving/dom/shadownote.h"
#include "engraving/dom/ledgerline.h"
#include "engraving/dom/staff.h"
#include "engraving/dom/system.h"
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

// Owner correction (2026-08-14, sweep review): stems must attach at the
// notehead's edge, not its center — the font's SMuFL stem anchors carry
// that fact, so JiMSMusic must publish nonzero anchors for every class
// notehead.
TEST(JiMStaffTests, jimsMusicNoteheadsPublishStemAnchors)
{
    auto provider = muse::modularity::globalIoc()->resolve<IEngravingFontsProvider>("jimstaff_tests");
    ASSERT_TRUE(provider);
    IEngravingFontPtr jimsFont = provider->fontByName("JiMSMusic");
    ASSERT_TRUE(jimsFont);
    for (SymId sym : { SymId::noteheadTriangleUpBlack, SymId::noteheadTriangleDownBlack,
                       SymId::noteheadDiamondBlack, SymId::noteheadSquareBlack }) {
        muse::PointF up = jimsFont->smuflAnchor(sym, SmuflAnchorId::stemUpSE, 1.0);
        muse::PointF down = jimsFont->smuflAnchor(sym, SmuflAnchorId::stemDownNW, 1.0);
        EXPECT_GT(up.x(), 0.0) << "stemUpSE missing — stem would pierce the head center";
        EXPECT_LT(down.x(), up.x());
    }
}

// Owner correction (2026-08-14, sweep review): a chord's stem must track
// the heads' Kernel-derived cents span at EVERY tuning — the line-number
// default leaves the stem short when the tuning moves the heads apart.
TEST(JiMStaffTests, chordStemSpansCentsHeightAcrossTunings)
{
    Score* score = ScoreRW::readScore(u"jimstaff_data/collision.mscx");
    ASSERT_TRUE(score);
    jims::TuningController controller(score, 0);
    for (double g : { 680.0, 700.0, 720.0 }) {
        ASSERT_TRUE(controller.beginPreview());
        ASSERT_TRUE(controller.preview(g));
        score->doLayout();
        const StaffType* st = score->staff(0)->staffType(Fraction(0, 1));
        for (Segment* seg = score->firstSegment(SegmentType::ChordRest); seg;
             seg = seg->next1(SegmentType::ChordRest)) {
            EngravingItem* el = seg->element(0);
            if (!el || !el->isChord()) {
                continue;
            }
            Chord* chord = toChord(el);
            if (chord->notes().size() < 2 || !chord->stem()) {
                continue;
            }
            const double headSpan
                = std::abs(chord->downNote()->jimsPosY(st) - chord->upNote()->jimsPosY(st));
            const double stemLen = chord->stem()->length();
            EXPECT_GE(stemLen, headSpan)
                << "stem shorter than the chord's cents span at generator " << g;
        }
        controller.cancel();
    }
    delete score;
}

// Owner correction (2026-08-14, second round): head clustering must be
// CENTS-true. At the 12-TET collision (0 cents apart) the dyad's heads
// sit on opposite sides of the stem (the accepted M2 look); at 720
// cents (240 apart — no visual overlap) both heads sit on the same
// side, in one column, attached to the stem. The diatonic-line test
// fired the second-cluster logic at every tuning.
TEST(JiMStaffTests, dyadHeadClusteringIsCentsTrueAcrossTunings)
{
    Score* score = ScoreRW::readScore(u"jimstaff_data/collision.mscx");
    ASSERT_TRUE(score);
    jims::TuningController controller(score, 0);

    auto dyad = [&]() -> Chord* {
        for (Segment* seg = score->firstSegment(SegmentType::ChordRest); seg;
             seg = seg->next1(SegmentType::ChordRest)) {
            EngravingItem* el = seg->element(0);
            if (el && el->isChord() && toChord(el)->notes().size() == 2) {
                return toChord(el);
            }
        }
        return nullptr;
    };

    // At exactly 700: a true collision — heads mirror to opposite sides.
    ASSERT_TRUE(controller.beginPreview());
    ASSERT_TRUE(controller.preview(700.0));
    score->doLayout();
    Chord* chord = dyad();
    ASSERT_TRUE(chord);
    EXPECT_NE(chord->notes()[0]->ldata()->mirror(), chord->notes()[1]->ldata()->mirror())
        << "colliding heads must sit on opposite stem sides at 700";
    controller.cancel();

    // At 720 (240 cents apart): no overlap — one column, no mirroring.
    ASSERT_TRUE(controller.beginPreview());
    ASSERT_TRUE(controller.preview(720.0));
    score->doLayout();
    chord = dyad();
    ASSERT_TRUE(chord);
    EXPECT_FALSE(chord->notes()[0]->ldata()->mirror());
    EXPECT_FALSE(chord->notes()[1]->ldata()->mirror());
    EXPECT_NEAR(chord->notes()[0]->ldata()->pos().x(), chord->notes()[1]->ldata()->pos().x(), 0.01)
        << "separated heads share one column at 720";
    controller.cancel();
    delete score;
}

// ---------------------------------------------------------------------
// Milestone 4 — stacked partial staves instead of ledger lines; header
// on every system (owner decisions 2026-08-15, J4.001 closure).
// ---------------------------------------------------------------------
namespace {
using Segs = std::vector<StaffType::JimsSegment>;

Segs frameOf(Score* score)
{
    return score->staff(0)->staffType(Fraction(0, 1))->jimsFrameSegments();
}

bool sameFrame(const Segs& a, const Segs& b)
{
    if (a.size() != b.size()) {
        return false;
    }
    for (size_t i = 0; i < a.size(); ++i) {
        if (std::abs(a[i].lowerCents - b[i].lowerCents) > EPS
            || std::abs(a[i].upperCents - b[i].upperCents) > EPS
            || a[i].whole != b[i].whole) {
            return false;
        }
    }
    return true;
}

// The Kernel's own answer for the melody currently on staff 0 — the
// oracle every fork frame must equal (ownership invariant).
Segs kernelFrameFor(Score* score)
{
    const StaffType* st = score->staff(0)->staffType(Fraction(0, 1));
    muse::String melody = u"{\"notes\":[";
    bool first = true;
    for (Segment* seg = score->firstSegment(SegmentType::ChordRest); seg;
         seg = seg->next1(SegmentType::ChordRest)) {
        for (track_idx_t track = 0; track < VOICES; ++track) {
            EngravingItem* el = seg->element(track);
            if (el && el->isChord()) {
                for (Note* note : toChord(el)->notes()) {
                    if (note->hasJimsPitch()) {
                        melody += (first ? u"" : u",");
                        melody += muse::String(u"{\"nPer\":%1,\"nGen\":%2}")
                                  .arg(note->jimsNPer()).arg(note->jimsNGen());
                        first = false;
                    }
                }
            }
        }
    }
    melody += u"]}";
    std::vector<jims::StaveSegment> segments;
    Segs out;
    if (jims::frameForMelody(st->jimsStateJson(), melody, st->jimsTonicExtent(), segments)) {
        for (const jims::StaveSegment& s : segments) {
            out.push_back({ s.lowerCents, s.upperCents, s.whole });
        }
    }
    return out;
}

Note* highestJimsNote(Score* score)
{
    const StaffType* st = score->staff(0)->staffType(Fraction(0, 1));
    Note* best = nullptr;
    double bestCents = -1e9;
    for (Segment* seg = score->firstSegment(SegmentType::ChordRest); seg;
         seg = seg->next1(SegmentType::ChordRest)) {
        EngravingItem* el = seg->element(0);
        if (!el || !el->isChord()) {
            continue;
        }
        for (Note* n : toChord(el)->notes()) {
            double c = 0.0;
            if (n->hasJimsPitch()
                && jims::noteCentsAboveDo(st->jimsStateJson(), n->jimsNPer(), n->jimsNGen(), c)
                && c > bestCents) {
                bestCents = c;
                best = n;
            }
        }
    }
    return best;
}

// Move a note by whole periods through the ordinary undoable property
// path (never touching the frame cache directly).
void shiftNotePeriods(Score* score, Note* note, int periods)
{
    // A real drag/entry moves BOTH the lattice identity and the stock
    // compatibility pitch (an octave per period), so ledger generation
    // sees the same line movement the GUI would produce.
    score->startCmd(TranslatableString::untranslatable("JiMS test edit"));
    note->undoChangeProperty(Pid::JIMS_NPER, note->jimsNPer() + periods);
    note->undoChangeProperty(Pid::PITCH, std::clamp(note->pitch() + 12 * periods, 0, 127));
    score->endCmd();
    score->doLayout();
}

size_t ledgerLineCountOnStaff0(Score* score)
{
    size_t n = 0;
    for (Segment* seg = score->firstSegment(SegmentType::ChordRest); seg;
         seg = seg->next1(SegmentType::ChordRest)) {
        for (track_idx_t track = 0; track < VOICES; ++track) {
            EngravingItem* el = seg->element(track);
            if (el && el->isChord()) {
                n += toChord(el)->ledgerLines().size();
            }
        }
    }
    return n;
}
}

// (a) An empty JiMS staff still has a Kernel-owned frame: exactly one
// whole period, sourced from frame_for_melody (owner decision 1a), never
// synthesized fork-side.
TEST(JiMStaffTests, emptyJimsStaffFrameIsKernelOwnedOneWholePeriod)
{
    Score* score = ScoreRW::readScore(u"jimstaff_data/collision.mscx");
    ASSERT_TRUE(score);
    score->doLayout();
    ASSERT_FALSE(frameOf(score).empty());

    // Delete every chord on staff 0 through the ordinary edit path.
    score->startCmd(TranslatableString::untranslatable("JiMS test edit"));
    std::vector<Chord*> chords;
    for (Segment* seg = score->firstSegment(SegmentType::ChordRest); seg;
         seg = seg->next1(SegmentType::ChordRest)) {
        for (track_idx_t track = 0; track < VOICES; ++track) {
            EngravingItem* el = seg->element(track);
            if (el && el->isChord()) {
                chords.push_back(toChord(el));
            }
        }
    }
    for (Chord* c : chords) {
        score->deleteItem(c);
    }
    score->endCmd();
    score->doLayout();

    Segs f = frameOf(score);
    ASSERT_EQ(f.size(), 1u) << "empty staff = one Kernel segment, not an empty cache";
    EXPECT_TRUE(f[0].whole);
    EXPECT_NEAR(f[0].lowerCents, 0.0, EPS);
    EXPECT_NEAR(f[0].upperCents, 1200.0, EPS);
    EXPECT_TRUE(sameFrame(f, kernelFrameFor(score))) << "fork frame != Kernel frame";
    delete score;
}

// (b) Growth above/below on an edit re-derives the stack in the same
// transaction; undo restores the exact prior list; redo the new one;
// and at every step the fork frame equals the Kernel's answer.
TEST(JiMStaffTests, liveFrameGrowsShrinksAndRoundTripsThroughUndo)
{
    Score* score = ScoreRW::readScore(u"jimstaff_data/collision.mscx");
    ASSERT_TRUE(score);
    score->doLayout();
    const Segs base = frameOf(score);
    ASSERT_FALSE(base.empty());
    ASSERT_TRUE(sameFrame(base, kernelFrameFor(score)));

    Note* top = highestJimsNote(score);
    ASSERT_TRUE(top);
    const double baseTop = base.back().upperCents;
    const double baseBottom = base.front().lowerCents;

    // Grow above by two periods: the stack must extend upward and gain
    // at least one segment; the top segment is partial unless promoted.
    shiftNotePeriods(score, top, +2);
    Segs grown = frameOf(score);
    EXPECT_GT(grown.back().upperCents, baseTop + 1200.0 - EPS);
    EXPECT_GT(grown.size(), base.size());
    EXPECT_TRUE(sameFrame(grown, kernelFrameFor(score))) << "fork frame != Kernel after growth";
    // No note may sit outside the returned frame.
    {
        const StaffType* st = score->staff(0)->staffType(Fraction(0, 1));
        double c = 0.0;
        ASSERT_TRUE(jims::noteCentsAboveDo(st->jimsStateJson(), top->jimsNPer(), top->jimsNGen(), c));
        EXPECT_LE(c, grown.back().upperCents + EPS);
        EXPECT_GE(c, grown.front().lowerCents - EPS);
    }

    // Undo: exact prior list. Redo: exact grown list.
    score->undoRedo(true, nullptr);
    score->doLayout();
    EXPECT_TRUE(sameFrame(frameOf(score), base)) << "undo must restore the exact prior stack";
    score->undoRedo(false, nullptr);
    score->doLayout();
    EXPECT_TRUE(sameFrame(frameOf(score), grown)) << "redo must restore the grown stack";
    score->undoRedo(true, nullptr);
    score->doLayout();

    // Grow below by two periods with the same note.
    shiftNotePeriods(score, top, -2);
    Segs low = frameOf(score);
    EXPECT_LT(low.front().lowerCents, baseBottom - 1200.0 + EPS);
    EXPECT_TRUE(sameFrame(low, kernelFrameFor(score))) << "fork frame != Kernel after growth below";
    score->undoRedo(true, nullptr);
    score->doLayout();
    EXPECT_TRUE(sameFrame(frameOf(score), base));

    // Shrink: delete the outermost chord; the stack must not keep the
    // old extent, and must equal the Kernel's answer.
    shiftNotePeriods(score, top, +2);
    score->startCmd(TranslatableString::untranslatable("JiMS test edit"));
    score->deleteItem(top->chord());
    score->endCmd();
    score->doLayout();
    Segs shrunk = frameOf(score);
    EXPECT_LT(shrunk.back().upperCents, grown.back().upperCents - EPS);
    EXPECT_TRUE(sameFrame(shrunk, kernelFrameFor(score)));
    delete score;
}

// (c) One score-wide stack per staff, identical on every system, and
// the header geometry is one shared calculation for all systems.
TEST(JiMStaffTests, frameAndHeaderGeometryAreIdenticalAcrossSystems)
{
    Score* score = ScoreRW::readScore(u"jimstaff_data/grym.mscx");
    ASSERT_TRUE(score);
    score->doLayout();
    ASSERT_GE(score->systems().size(), 2u) << "grym must span multiple systems for this test";
    const StaffType* first = nullptr;
    StaffType::JimsHeaderGeometry g0 {};
    for (System* sys : score->systems()) {
        Measure* m = sys->firstMeasure();
        if (!m) {
            continue;
        }
        const StaffType* st = score->staff(0)->staffType(m->tick());
        ASSERT_TRUE(st && st->isJiMS());
        StaffType::JimsHeaderGeometry g
            = st->jimsHeaderGeometry(score->style().spatium(), score->style().defaultSpatium());
        if (!first) {
            first = st;
            g0 = g;
            continue;
        }
        EXPECT_TRUE(sameFrame(st->jimsFrameSegments(), first->jimsFrameSegments()));
        EXPECT_NEAR(g.headerWidth, g0.headerWidth, EPS);
        EXPECT_NEAR(g.clefRx, g0.clefRx, EPS);
        EXPECT_NEAR(g.leftLabelBand, g0.leftLabelBand, EPS);
        EXPECT_NEAR(g.rightLabelBand, g0.rightLabelBand, EPS);
    }
    ASSERT_TRUE(first);
    delete score;
}

// (d) No ledger line is ever GENERATED for a JiMS chord — even when a
// note sits far outside the configured line count (owner decision 3a:
// suppression by non-generation, not by hiding at paint time).
TEST(JiMStaffTests, jimsChordsGenerateNoLedgerLinesEvenFarOutside)
{
    Score* score = ScoreRW::readScore(u"jimstaff_data/collision.mscx");
    ASSERT_TRUE(score);
    score->doLayout();
    Note* top = highestJimsNote(score);
    ASSERT_TRUE(top);
    shiftNotePeriods(score, top, +3);   // three periods up: far above the staff
    EXPECT_EQ(ledgerLineCountOnStaff0(score), 0u)
        << "a JiMS chord must never carry LedgerLine elements";
    shiftNotePeriods(score, top, -6);   // and far below
    EXPECT_EQ(ledgerLineCountOnStaff0(score), 0u);
    delete score;
}

// (e) The note-input preview (ShadowNote) exposes no ledger lines on a
// JiMS staff, at any line index — the preview path is a distinct
// suppression target (owner decision 3a).
TEST(JiMStaffTests, shadowNoteShowsNoLedgerLinesOnJimsStaff)
{
    Score* score = ScoreRW::readScore(u"jimstaff_data/collision.mscx");
    ASSERT_TRUE(score);
    score->doLayout();
    ShadowNote* sn = score->shadowNote();
    ASSERT_TRUE(sn);
    sn->setTrack(0);
    sn->setTick(Fraction(0, 1));
    ASSERT_TRUE(sn->staffType() && sn->staffType()->isJiMS());
    for (int line : { -30, -6, 0, 12, 20, 40 }) {
        sn->setLineIndex(line);
        EXPECT_FALSE(sn->ledgerLinesVisible()) << "JiMS preview ledger at line " << line;
    }
    delete score;
}

// Scale-dot labels (owner epiphany 2026-08-15), Phase 3: the per-staff
// display mode — fork-owned StaffType presentation state, default
// Auto, serialized as its own tag, never entering the Kernel state.
TEST(JiMStaffTests, scaleDotLabelModeDefaultsPersistsAndStaysOutOfKernelState)
{
    Score* score = ScoreRW::readScore(u"jimstaff_data/collision.mscx");
    ASSERT_TRUE(score);
    StaffType* st = score->staff(0)->staffType(Fraction(0, 1));
    ASSERT_TRUE(st && st->isJiMS());

    // Absent tag reads as Auto.
    EXPECT_EQ(st->jimsScaleDotLabelMode(), JimsScaleDotLabelMode::Auto);

    // All four values round-trip through mscx write/read.
    for (auto mode : { JimsScaleDotLabelMode::None, JimsScaleDotLabelMode::Left,
                       JimsScaleDotLabelMode::Split, JimsScaleDotLabelMode::Auto }) {
        st->setJimsScaleDotLabelMode(mode);
        EXPECT_EQ(st->jimsScaleDotLabelMode(), mode);
        const muse::String path = u"jims_label_mode_roundtrip.mscx";
        ASSERT_TRUE(ScoreRW::saveScore(score, path));
        std::ifstream in(path.toStdString());
        const std::string xml((std::istreambuf_iterator<char>(in)),
                              std::istreambuf_iterator<char>());
        if (mode == JimsScaleDotLabelMode::Auto) {
            // Default mode: tag may be omitted; absent reads Auto.
        } else {
            EXPECT_NE(xml.find("<jimsScaleDotLabels>"), std::string::npos);
        }
        // The mode NEVER enters the Kernel state JSON.
        EXPECT_EQ(st->jimsStateJson().toStdString().find("ScaleDotLabel"), std::string::npos);
        EXPECT_EQ(st->jimsStateJson().toStdString().find("label"), std::string::npos);
    }

    // Equality participates: two staff types differing only in mode differ.
    StaffType a(*st), b(*st);
    a.setJimsScaleDotLabelMode(JimsScaleDotLabelMode::Left);
    b.setJimsScaleDotLabelMode(JimsScaleDotLabelMode::Split);
    EXPECT_FALSE(a == b);
    b.setJimsScaleDotLabelMode(JimsScaleDotLabelMode::Left);
    EXPECT_TRUE(a == b);
    delete score;
}

// Phase 3: the mode round-trips through a full mscx save/reload, and
// differing StaffType spans keep independent modes.
TEST(JiMStaffTests, scaleDotLabelModeRoundTripsAndSpansAreIndependent)
{
    Score* score = ScoreRW::readScore(u"jimstaff_data/mode-change.mscx");
    ASSERT_TRUE(score);
    Staff* staff = score->staff(0);
    StaffType* base = staff->staffType(Fraction(0, 1));
    StaffType* changed = staff->staffType(Fraction(16, 4));
    ASSERT_TRUE(base && changed && base != changed);
    base->setJimsScaleDotLabelMode(JimsScaleDotLabelMode::Left);
    changed->setJimsScaleDotLabelMode(JimsScaleDotLabelMode::Split);
    EXPECT_EQ(base->jimsScaleDotLabelMode(), JimsScaleDotLabelMode::Left);
    EXPECT_EQ(changed->jimsScaleDotLabelMode(), JimsScaleDotLabelMode::Split);

    const muse::String path = u"jims_label_mode_spans.mscx";
    ASSERT_TRUE(ScoreRW::saveScore(score, path));
    std::ifstream in(path.toStdString());
    const std::string xml((std::istreambuf_iterator<char>(in)),
                          std::istreambuf_iterator<char>());
    EXPECT_NE(xml.find(">left<"), std::string::npos);
    EXPECT_NE(xml.find(">split<"), std::string::npos);
    delete score;
}

// Phase 4: Auto resolves via the Kernel's label-legibility range with
// STRICT comparisons — Left strictly inside (690.9, 709.1), Split at
// or outside either exact boundary. Test-only epsilon; production has
// none.
TEST(JiMStaffTests, scaleDotLabelAutoResolvesExactlyAtLegibilityBoundaries)
{
    Score* score = ScoreRW::readScore(u"jimstaff_data/collision.mscx");
    ASSERT_TRUE(score);
    StaffType* st = score->staff(0)->staffType(Fraction(0, 1));
    ASSERT_TRUE(st && st->isJiMS());
    st->setJimsScaleDotLabelMode(JimsScaleDotLabelMode::Auto);

    jims::TuningController controller(score, 0);
    const double eps = 0.001;
    const struct { double g; JimsScaleDotLabelMode want; } cases[] = {
        { 690.9 - eps, JimsScaleDotLabelMode::Split },
        { 690.9,       JimsScaleDotLabelMode::Split },
        { 690.9 + eps, JimsScaleDotLabelMode::Left },
        { 700.0,       JimsScaleDotLabelMode::Left },
        { 709.1 - eps, JimsScaleDotLabelMode::Left },
        { 709.1,       JimsScaleDotLabelMode::Split },
        { 709.1 + eps, JimsScaleDotLabelMode::Split },
    };
    for (const auto& c : cases) {
        ASSERT_TRUE(controller.beginPreview());
        ASSERT_TRUE(controller.preview(c.g));
        EXPECT_EQ(st->jimsResolvedScaleDotLabelMode(), c.want) << "at " << c.g;
        controller.cancel();
    }
    // Explicit modes pass through unresolved.
    st->setJimsScaleDotLabelMode(JimsScaleDotLabelMode::None);
    EXPECT_EQ(st->jimsResolvedScaleDotLabelMode(), JimsScaleDotLabelMode::None);
    st->setJimsScaleDotLabelMode(JimsScaleDotLabelMode::Split);
    EXPECT_EQ(st->jimsResolvedScaleDotLabelMode(), JimsScaleDotLabelMode::Split);
    delete score;
}

// Phase 4: ONE shared header-geometry calculation feeds layout, draw,
// and margin reservation. None reserves no label bands; Left reserves
// a left band only; Split reserves both; every band is positive when
// labels exist on that side.
TEST(JiMStaffTests, scaleDotLabelHeaderGeometryIsSharedAndModeAware)
{
    Score* score = ScoreRW::readScore(u"jimstaff_data/collision.mscx");
    ASSERT_TRUE(score);
    score->doLayout();
    StaffType* st = score->staff(0)->staffType(Fraction(0, 1));
    ASSERT_TRUE(st && st->isJiMS());
    const double sp = score->style().spatium();

    st->setJimsScaleDotLabelMode(JimsScaleDotLabelMode::None);
    auto none = st->jimsHeaderGeometry(sp, sp);
    EXPECT_EQ(none.leftLabelBand, 0.0);
    EXPECT_EQ(none.rightLabelBand, 0.0);
    EXPECT_GT(none.headerWidth, 0.0);

    st->setJimsScaleDotLabelMode(JimsScaleDotLabelMode::Left);
    auto left = st->jimsHeaderGeometry(sp, sp);
    EXPECT_GT(left.leftLabelBand, 0.0);
    EXPECT_EQ(left.rightLabelBand, 0.0);
    EXPECT_GT(left.headerWidth, none.headerWidth);

    st->setJimsScaleDotLabelMode(JimsScaleDotLabelMode::Split);
    auto split = st->jimsHeaderGeometry(sp, sp);
    EXPECT_GT(split.leftLabelBand, 0.0);
    EXPECT_GT(split.rightLabelBand, 0.0);
    delete score;
}

// Phase 4: the edge-collision opposite-side invariant, checked against
// REAL Kernel stack data (never assumed): at the 5-TET boundary every
// multi-member stack pairs a flat-side (nGen <= 0) member with a
// sharp-side (nGen > 0) member, so Split cannot label-collide where
// dots collide.
TEST(JiMStaffTests, edgeCollisionStacksAlwaysStraddleTheReSplit)
{
    Score* score = ScoreRW::readScore(u"jimstaff_data/collision.mscx");
    ASSERT_TRUE(score);
    jims::TuningController controller(score, 0);
    for (double g : { 720.0, 1200.0 * 4.0 / 7.0 + 0.0001 }) {
        ASSERT_TRUE(controller.beginPreview());
        ASSERT_TRUE(controller.preview(g));
        const StaffType* st = score->staff(0)->staffType(Fraction(0, 1));
        std::vector<jims::LabeledDotStack> stacks;
        ASSERT_TRUE(jims::scaleDotLabels(st->jimsStateJson(), stacks));
        ASSERT_FALSE(stacks.empty());
        for (const jims::LabeledDotStack& stack : stacks) {
            if (stack.members.size() < 2) {
                continue;
            }
            bool hasFlatSide = false, hasSharpSide = false;
            for (const jims::LabeledDotMember& member : stack.members) {
                (member.nGen <= 0 ? hasFlatSide : hasSharpSide) = true;
                EXPECT_FALSE(member.label.isEmpty());
            }
            EXPECT_TRUE(hasFlatSide && hasSharpSide)
                << "multi-member stack must straddle the Re split at g=" << g;
        }
        controller.cancel();
    }
    delete score;
}

// Phase 5 (labels FINAL §13): the labels-enabled controller sweep — the
// same shared controller, all four modes, per-tick semantic assertions
// and the latency floor. Gated by JIMS_SWEEP=1 with artifacts in
// JIMS_SWEEP_OUT (suffix -labels).
TEST(JiMStaffTests, labelsEnabledControllerSweepHasNoStaleLabels)
{
    const char* gate = std::getenv("JIMS_SWEEP");
    if (!gate || muse::String::fromUtf8(gate) != u"1") {
        GTEST_SKIP() << "set JIMS_SWEEP=1 to run the labels sweep";
    }
    const char* outDir = std::getenv("JIMS_SWEEP_OUT");
    ASSERT_TRUE(outDir);

    const std::vector<muse::String> pieces
        = { u"collision", u"ode-to-joy", u"acc-chromatic", u"mode-change", u"grym" };
    const JimsScaleDotLabelMode modes[]
        = { JimsScaleDotLabelMode::Auto, JimsScaleDotLabelMode::Left,
            JimsScaleDotLabelMode::Split, JimsScaleDotLabelMode::None };
    const char* modeNames[] = { "auto", "left", "split", "none" };

    for (const muse::String& piece : pieces) {
        Score* score = ScoreRW::readScore(u"jimstaff_data/" + piece + u".mscx");
        ASSERT_TRUE(score);
        score->doLayout();
        StaffType* st = score->staff(0)->staffType(Fraction(0, 1));
        ASSERT_TRUE(st && st->isJiMS());
        jims::TuningController controller(score, 0);
        double rMin = 0.0, rMax = 0.0;
        ASSERT_TRUE(jims::labelLegibilityRange(rMin, rMax));

        std::ofstream sem(std::string(outDir) + "/" + piece.toStdString() + "-labels-semantics.jsonl");
        std::ofstream lat(std::string(outDir) + "/" + piece.toStdString() + "-labels-latency.txt");

        for (int m = 0; m < 4; ++m) {
            st->setJimsScaleDotLabelMode(modes[m]);
            std::vector<double> applied;
            ASSERT_TRUE(controller.beginPreview());
            // Warm-up round trip, then one captured round trip plus the
            // exact legibility boundaries and their neighbors.
            for (int i = 0; i <= 80; ++i) {
                ASSERT_TRUE(controller.preview(680.0 + 0.5 * i));
            }
            std::vector<double> samples;
            for (int i = 0; i <= 160; ++i) {
                samples.push_back(i <= 80 ? 680.0 + 0.5 * i : 720.0 - 0.5 * (i - 80));
            }
            for (double b : { rMin - 0.001, rMin, rMin + 0.001, rMax - 0.001, rMax, rMax + 0.001 }) {
                samples.push_back(b);
            }
            for (double g : samples) {
                ASSERT_TRUE(controller.preview(g));
                applied.push_back(controller.lastApplyMs());
                // No stale state: the metrics read back the applied g.
                double mg = 0.0, mp = 0.0;
                ASSERT_TRUE(jims::staffMetrics(st->jimsStateJson(), mg, mp));
                ASSERT_NEAR(mg, g, 1e-9);
                // Resolved mode is correct for the applied tuning.
                const JimsScaleDotLabelMode resolved = st->jimsResolvedScaleDotLabelMode();
                if (modes[m] == JimsScaleDotLabelMode::Auto) {
                    ASSERT_EQ(resolved, (g > rMin && g < rMax)
                              ? JimsScaleDotLabelMode::Left : JimsScaleDotLabelMode::Split);
                } else {
                    ASSERT_EQ(resolved, modes[m]);
                }
                // Labels re-derive from the CURRENT state every tick.
                std::vector<jims::LabeledDotStack> stacks;
                ASSERT_TRUE(jims::scaleDotLabels(st->jimsStateJson(), stacks));
                ASSERT_FALSE(stacks.empty());
                for (const jims::LabeledDotStack& stack : stacks) {
                    for (const jims::LabeledDotMember& member : stack.members) {
                        ASSERT_FALSE(member.label.isEmpty());
                    }
                }
                // Geometry agrees with the resolved mode (shared calc).
                const auto geom = st->jimsHeaderGeometry(score->style().spatium(),
                                                         score->style().defaultSpatium());
                if (resolved == JimsScaleDotLabelMode::None) {
                    ASSERT_EQ(geom.leftLabelBand + geom.rightLabelBand, 0.0);
                } else if (resolved == JimsScaleDotLabelMode::Left) {
                    ASSERT_GT(geom.leftLabelBand, 0.0);
                    ASSERT_EQ(geom.rightLabelBand, 0.0);
                } else {
                    ASSERT_GT(geom.leftLabelBand + geom.rightLabelBand, 0.0);
                }
            }
            controller.cancel();
            std::sort(applied.begin(), applied.end());
            const size_t n = applied.size();
            lat << "piece=" << piece.toStdString() << " mode=" << modeNames[m]
                << " ticks=" << n
                << " median_ms=" << applied[n / 2]
                << " p95_ms=" << applied[n * 95 / 100]
                << " max_ms=" << applied[n - 1] << " dropped=0\n";
            EXPECT_LE(applied[n * 95 / 100], 33.3) << piece.toStdString() << " " << modeNames[m];
            // Deterministic semantic record at the boundaries per mode.
            for (double g : { rMin, 700.0, rMax }) {
                ASSERT_TRUE(controller.beginPreview());
                ASSERT_TRUE(controller.preview(g));
                std::vector<jims::LabeledDotStack> stacks;
                ASSERT_TRUE(jims::scaleDotLabels(st->jimsStateJson(), stacks));
                sem << "{\"mode\":\"" << modeNames[m] << "\",\"g\":"
                    << muse::String::number(g, 6).toStdString() << ",\"stacks\":[";
                bool firstStack = true;
                for (const jims::LabeledDotStack& stack : stacks) {
                    sem << (firstStack ? "" : ",") << "["
                        << muse::String::number(stack.cents, 4).toStdString();
                    for (const jims::LabeledDotMember& member : stack.members) {
                        sem << ",[" << member.nGen << ",\"" << member.label.toStdString() << "\"]";
                    }
                    sem << "]";
                    firstStack = false;
                }
                sem << "]}\n";
                controller.cancel();
            }
        }
        st->setJimsScaleDotLabelMode(JimsScaleDotLabelMode::Auto);
        delete score;
    }
}

// Milestone 4 Phase 5: a wide melody (several stacked segments) swept
// through the tuning controller with labels enabled. Every tick: the
// Kernel frame derivation succeeds, every note is contained by the
// returned frame, the fork frame equals the Kernel's answer for the
// current state (a changed state never reuses a stale frame), no
// ledger element exists, labels stay populated, every system reports
// the same frame, and p95 apply latency stays within the floor.
TEST(JiMStaffTests, wideMelodyControllerSweepKeepsFrameValidAndFast)
{
    const char* gate = std::getenv("JIMS_SWEEP");
    if (!gate || muse::String::fromUtf8(gate) != u"1") {
        GTEST_SKIP() << "set JIMS_SWEEP=1 to run the M4 wide-melody sweep";
    }
    const char* outDir = std::getenv("JIMS_SWEEP_OUT");
    ASSERT_TRUE(outDir);

    for (const muse::String& piece : { muse::String(u"collision"), muse::String(u"grym") }) {
        Score* score = ScoreRW::readScore(u"jimstaff_data/" + piece + u".mscx");
        ASSERT_TRUE(score);
        score->doLayout();
        StaffType* st = score->staff(0)->staffType(Fraction(0, 1));
        ASSERT_TRUE(st && st->isJiMS());
        st->setJimsScaleDotLabelMode(JimsScaleDotLabelMode::Auto);

        // Widen: push the highest note up two periods so the frame needs
        // a whole middle stave plus a partial edge (patent cut rule).
        Note* top = highestJimsNote(score);
        ASSERT_TRUE(top);
        shiftNotePeriods(score, top, +2);
        ASSERT_GE(frameOf(score).size(), 3u) << piece.toStdString() << ": expected a stacked frame";

        std::ofstream sem(std::string(outDir) + "/" + piece.toStdString() + "-m4-frame-semantics.jsonl");
        std::ofstream lat(std::string(outDir) + "/" + piece.toStdString() + "-m4-frame-latency.txt");

        jims::TuningController controller(score, 0);
        std::vector<double> applied;
        ASSERT_TRUE(controller.beginPreview());
        for (int i = 0; i <= 80; ++i) {                    // warm-up
            ASSERT_TRUE(controller.preview(680.0 + 0.5 * i));
        }
        std::vector<double> samples;
        for (int i = 0; i <= 160; ++i) {
            samples.push_back(i <= 80 ? 680.0 + 0.5 * i : 720.0 - 0.5 * (i - 80));
        }
        for (double g : samples) {
            ASSERT_TRUE(controller.preview(g));
            applied.push_back(controller.lastApplyMs());
            score->doLayout();
            const Segs frame = frameOf(score);
            ASSERT_FALSE(frame.empty()) << piece.toStdString() << " g=" << g << ": frame derivation failed";
            ASSERT_TRUE(sameFrame(frame, kernelFrameFor(score))) << piece.toStdString() << " g=" << g
                                                                  << ": fork frame != Kernel (stale?)";
            // Every note inside the returned frame.
            for (Segment* seg = score->firstSegment(SegmentType::ChordRest); seg;
                 seg = seg->next1(SegmentType::ChordRest)) {
                EngravingItem* el = seg->element(0);
                if (!el || !el->isChord()) {
                    continue;
                }
                for (Note* n : toChord(el)->notes()) {
                    double c = 0.0;
                    ASSERT_TRUE(jims::noteCentsAboveDo(st->jimsStateJson(), n->jimsNPer(), n->jimsNGen(), c));
                    ASSERT_GE(c, frame.front().lowerCents - EPS) << piece.toStdString() << " g=" << g;
                    ASSERT_LE(c, frame.back().upperCents + EPS) << piece.toStdString() << " g=" << g;
                }
            }
            ASSERT_EQ(ledgerLineCountOnStaff0(score), 0u) << piece.toStdString() << " g=" << g;
            std::vector<jims::LabeledDotStack> stacks;
            ASSERT_TRUE(jims::scaleDotLabels(st->jimsStateJson(), stacks));
            ASSERT_FALSE(stacks.empty());
            for (System* sys : score->systems()) {
                Measure* m = sys->firstMeasure();
                if (m) {
                    ASSERT_TRUE(sameFrame(score->staff(0)->staffType(m->tick())->jimsFrameSegments(), frame));
                }
            }
            sem << "{\"g\":" << g << ",\"segments\":[";
            for (size_t i = 0; i < frame.size(); ++i) {
                sem << (i ? "," : "") << "[" << frame[i].lowerCents << "," << frame[i].upperCents
                    << "," << (frame[i].whole ? "true" : "false") << "]";
            }
            sem << "],\"ledgers\":0}\n";
        }
        controller.cancel();
        std::sort(applied.begin(), applied.end());
        const double p95 = applied[size_t(applied.size() * 0.95)];
        const double median = applied[applied.size() / 2];
        lat << "piece=" << piece.toStdString() << " ticks=" << applied.size()
            << " median_ms=" << median << " p95_ms=" << p95 << " max_ms=" << applied.back() << "\n";
        EXPECT_LE(p95, 33.3) << piece.toStdString() << ": p95 apply latency floor breached with live frame derivation";
        delete score;
    }
}

// Milestone 4 Phase 5 evidence writer (env-gated): produces the render
// scenario scores through ordinary edits so the CLI can render each in
// clean directories — empty staff, growth above, growth below, boundary
// crossing (whole middle + partial edge), and shrink after growth.
TEST(JiMStaffTests, m4WriteRenderScenarios)
{
    const char* gate = std::getenv("JIMS_M4_SCENARIOS");
    if (!gate || muse::String::fromUtf8(gate) != u"1") {
        GTEST_SKIP() << "set JIMS_M4_SCENARIOS=1 (and JIMS_SWEEP_OUT) to write the M4 render scenarios";
    }
    const char* outDir = std::getenv("JIMS_SWEEP_OUT");
    ASSERT_TRUE(outDir);
    const muse::String out = muse::String::fromUtf8(outDir);

    auto fresh = []() {
        Score* s = ScoreRW::readScore(u"jimstaff_data/collision.mscx");
        s->doLayout();
        return s;
    };
    // 1. empty staff
    {
        Score* score = fresh();
        score->startCmd(TranslatableString::untranslatable("JiMS scenario"));
        std::vector<Chord*> chords;
        for (Segment* seg = score->firstSegment(SegmentType::ChordRest); seg;
             seg = seg->next1(SegmentType::ChordRest)) {
            for (track_idx_t track = 0; track < VOICES; ++track) {
                EngravingItem* el = seg->element(track);
                if (el && el->isChord()) {
                    chords.push_back(toChord(el));
                }
            }
        }
        for (Chord* c : chords) {
            score->deleteItem(c);
        }
        score->endCmd();
        score->doLayout();
        ASSERT_EQ(frameOf(score).size(), 1u);
        ASSERT_TRUE(ScoreRW::saveScore(score, out + u"/m4-empty.mscx"));
        delete score;
    }
    // 2. growth above (+2 periods on the highest note)
    {
        Score* score = fresh();
        shiftNotePeriods(score, highestJimsNote(score), +2);
        ASSERT_GE(frameOf(score).size(), 3u);
        ASSERT_TRUE(ScoreRW::saveScore(score, out + u"/m4-grow-above.mscx"));
        delete score;
    }
    // 3. growth below (-2 periods on the highest note)
    {
        Score* score = fresh();
        shiftNotePeriods(score, highestJimsNote(score), -2);
        ASSERT_GE(frameOf(score).size(), 2u);
        ASSERT_TRUE(ScoreRW::saveScore(score, out + u"/m4-grow-below.mscx"));
        delete score;
    }
    // 4. boundary crossing: +1 period (top note lands one octave up:
    // whole middle stave + partial edge per the cut rule)
    {
        Score* score = fresh();
        shiftNotePeriods(score, highestJimsNote(score), +1);
        const Segs f = frameOf(score);
        ASSERT_GE(f.size(), 2u);
        ASSERT_TRUE(ScoreRW::saveScore(score, out + u"/m4-cross-boundary.mscx"));
        delete score;
    }
    // 5. shrink: grow +2 then delete that chord
    {
        Score* score = fresh();
        Note* top = highestJimsNote(score);
        shiftNotePeriods(score, top, +2);
        score->startCmd(TranslatableString::untranslatable("JiMS scenario"));
        score->deleteItem(top->chord());
        score->endCmd();
        score->doLayout();
        ASSERT_TRUE(ScoreRW::saveScore(score, out + u"/m4-shrink.mscx"));
        delete score;
    }
}
