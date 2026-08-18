/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 *
 * JiMStaff Milestone 1 — the fork-side wrapper over the jims-musescore-bridge
 * C ABI (Apache-2.0 staticlib built from the JiMS Kernel repository). Every
 * musical fact used by JiMStaff rendering crosses this seam as JSON derived
 * by the Kernel; nothing here computes note classes, memberships, cents, or
 * notehead classes itself.
 */
#ifndef MU_ENGRAVING_JIMSBRIDGE_H
#define MU_ENGRAVING_JIMSBRIDGE_H

#include <vector>

#include "types/string.h"

namespace mu::engraving::jims {
/// Bridge availability: the linked C ABI answers version 1.
bool available();

// Native MusicXML import (owner decision 1a, 2026-08-16): ask the Kernel
// whether a transcribed state JSON is a valid JiMStaffStateV2 (bridge op
// `validate`); on rejection `error` carries the Kernel's message. The
// importer computes no musical fact — the Kernel is the only gate.
bool validateState(const muse::String& stateJson, muse::String& error);

/// A note's cents above the staff's lower Do boundary, composed from the
/// Kernel's note_placement (Do-relative ordinate + register) and the state's
/// own period/extent values — projection only, no fact derivation.
bool noteCentsAboveDo(const muse::String& stateJson, int nPer, int nGen, double& cents);

/// The Kernel's semantic notehead-class token for a generator coordinate
/// (e.g. "conventional", "triangle-vertex-up").
bool noteheadToken(const muse::String& stateJson, int nGen, muse::String& token);

/// One scale-dot stack: cents above the lower Do, plus member generator
/// coordinates front-to-back (Kernel collision order).
struct ScaleDotStack {
    double cents = 0.0;
    std::vector<int> frontToBack;
};

/// The Kernel's scale-dot stacks for one staff, in ascending order.
bool scaleDots(const muse::String& stateJson, std::vector<ScaleDotStack>& stacks);

/// The Kernel-owned tonic-indicator position: cents above Do for the
/// state's mode-selected (movable) tonic.
bool tonicCentsAboveDo(const muse::String& stateJson, double& cents);

/// The staff's tuning metrics (generator and period widths in cents),
/// Kernel-validated. The tuning label and any width-derived drawing use
/// this; the fork never parses the state JSON for musical facts.
bool staffMetrics(const muse::String& stateJson, double& generatorCents, double& periodCents);

/// The Kernel's diatonic Valid Tuning Range in cents — every tuning
/// control's bounds derive from this, never a fork-side constant.
bool generatorRange(double& minCents, double& maxCents);

/// The Kernel's scale-dot label-legibility range (owner-defined,
/// distinct from the valid tuning range): Auto label mode reads Left
/// strictly inside, Split at or outside.
bool labelLegibilityRange(double& minCents, double& maxCents);

/// One labeled member of a scale-dot stack.
struct LabeledDotMember {
    int nGen = 0;
    muse::String label;
};

/// One scale-dot stack with its members' canonical-solfa labels,
/// ordered as the Kernel derives them (front order, ascending |nGen|).
struct LabeledDotStack {
    double cents = 0.0;
    std::vector<LabeledDotMember> members;
};

/// The staff's scale-dot stacks with per-member labels — ordered stack
/// topology preserved; the flat/sharp side split is the caller's
/// nGen-sign check, never transported.
bool scaleDotLabels(const muse::String& stateJson, std::vector<LabeledDotStack>& stacks);

/// One Kernel-owned Just Intonation staff line: exact just cents, prime
/// limit, and VTR-gated visibility for the state's current generator.
struct JiLine {
    double cents = 0.0;
    int limit = 5;
    bool visible = true;
};

/// The staff's current-key label (owner spec 2026-08-17): the tonic's
/// sounding pitch name + MIDI octave, e.g. "C4", drawn as "[PitchN]:" left
/// of the tonic indicator. Kernel-derived (pinned or default reference).
struct TonicPitchLabel {
    muse::String label;     // "C4", "Eb3", "F#4"
    int keyNumber = 0;
    int nPer = 0;           // the tonic instance the label names
    int nGen = 0;
};
bool tonicPitchLabel(const muse::String& stateJson, TonicPitchLabel& out);
/// Milestone 8 (owner finding 2, 2026-08-18): the current-key label for the
/// tonic instance in frame period `periodIndex` (period k = [k·P, (k+1)·P)
/// cents above the extent's lower Do) — the label of THAT row, so a
/// "[PitchN]:" always names the octave of the row it sits on. Period 0 is
/// exactly tonicPitchLabel. Same op with the additive period_index field.
bool tonicPitchLabelInPeriod(const muse::String& stateJson, int periodIndex, TonicPitchLabel& out);

/// Milestone 7 (playback): a note's SOUNDING pitch — the Kernel's answer
/// (identity + the section's tuning and reference) that playback feeds
/// through as MIDI key + cents; the fork computes none of it.
struct SoundingPitch {
    double frequencyHz = 0.0;
    int midiKey = 0;             // nearest 12-TET key, 0..127
    double centsOffset = 0.0;    // residual cents in [-50, 50)
    int referenceKeyNumber = 0;  // Re0 after resolution (62 when unpinned)
    double referenceFrequencyHz = 0.0;
    muse::String anchor;         // "explicit-reference" | "inferred-re0-d4"
};
bool noteSoundingPitch(const muse::String& stateJson, int nPer, int nGen, SoundingPitch& out, muse::String* error = nullptr);

/// Native MusicXML export (2026-08-17): the Kernel's COMPLETE self-tagged
/// jims:staff-state element (numbered with the extension's `number`
/// attribute when staffNumber > 0) and jims:change element (empty string
/// when nothing changed). The fork inserts them verbatim, never edits them.
bool musicxmlStaffStateV3Xml(const muse::String& stateJson, int staffNumber, muse::String& out, muse::String* error = nullptr);
bool musicxmlChangeEventV3Xml(const muse::String& oldStateJson, const muse::String& newStateJson, muse::String& out,
                              muse::String* error = nullptr);

/// The Kernel's JI staff-line scaffold (owner rulings 1a/2a, 2026-08-14).
bool jiLines(const muse::String& stateJson, std::vector<JiLine>& lines);

/// One stave segment of the Kernel-derived frame (partial-staves ruling
/// 2026-08-14): cents relative to the staff origin; partial segments get
/// a sliced, closed crescent at their cut edge.
struct StaveSegment {
    double lowerCents = 0.0;
    double upperCents = 0.0;
    bool whole = true;
};

/// The Kernel frame for a melody plus the DECLARED tonic-extent token
/// ("tonic-bounded" / "tonic-centered"), derived at authoring and saved.
// Milestone 5 — change indicators (owner notation rulings 2026-08-16).
// The Kernel returns the COMPLETE ready-to-paint model; the fork parses
// and paints, never diffs states or decides anything musical.
struct ChangePoint {
    int nGen = 0;
    muse::String label;
    double ordinate = 0.0;      // Do-relative, in periods [0,1)
    int periodOffset = 0;       // 0 = the enclosure's period, +1 = one period up
};
struct ChangeStack {
    double ordinate = 0.0;
    int periodOffset = 0;
    std::vector<ChangePoint> members;   // front first
};
struct ChangeArrow {
    muse::String kind;          // "key" | "mode"
    ChangePoint from;
    ChangePoint to;
    bool up = true;
    muse::String trumps;        // "" or the trumped kind ("mode")
};
struct ChangeIndicator {
    std::vector<muse::String> kinds;    // "key","mode","scale" in fixed order
    std::vector<ChangeStack> dotStacks;
    std::vector<ChangePoint> tonicIndicators;
    std::vector<ChangeArrow> arrows;
    bool empty() const { return kinds.empty(); }
};
/// `error` (optional) receives the Kernel's reason when no indicator can be
/// derived (e.g. an unrecoverable reference pair) — the panel shows it so a
/// change that draws nothing is never silent.
bool changeIndicator(const muse::String& oldStateJson, const muse::String& newStateJson, ChangeIndicator& out,
                     muse::String* error = nullptr);
struct ConnectorGlyph {
    double penCents = 0.0;
    double headHeightCents = 0.0;
    double headHalfWidthCents = 0.0;
};
bool connectorGlyph(ConnectorGlyph& out);

bool frameForMelody(const muse::String& stateJson, const muse::String& melodyJson, const muse::String& extentToken,
                    std::vector<StaveSegment>& segments);

/// Milestone 8 (octave-band elision): the Kernel's BANDED frame for a
/// melody through the same frame_for_melody op with the additive
/// `options` object (`elide_empty_periods`, `min_band_periods`). Bands
/// come bottom to top; each carries its segments, bounds, period indices,
/// and its own current-key label; `omittedPeriodCount` is the Kernel's
/// count. The fork never derives surviving periods, labels, or counts.
struct FrameBand {
    std::vector<StaveSegment> segments;
    double lowerCents = 0.0;
    double upperCents = 0.0;
    int lowestPeriodIndex = 0;
    int highestPeriodIndex = 0;
    int labelPeriodIndex = 0;
    TonicPitchLabel tonicLabel;
};
struct FrameBands {
    std::vector<FrameBand> bands;
    int omittedPeriodCount = 0;
};
bool frameBandsForMelody(const muse::String& stateJson, const muse::String& melodyJson, const muse::String& extentToken,
                         bool elideEmptyPeriods, int minBandPeriods, FrameBands& out);

/// A quantization hit: the nearest realizable lattice pitch to a target
/// cents height, with the Kernel compatibility pitch (step/alter/octave)
/// so the fork never derives spelling itself.
struct PitchHit {
    int nPer = 0;
    int nGen = 0;
    double centsAboveLowerDo = 0.0;
    char step = 'C';
    int alter = 0;
    int octave = 4;
};

/// Quantize a drag target (owner rulings 2026-08-14): nearest realizable
/// pitch; current identity retained at exact-midpoint ties when eligible.
bool nearestPitch(const muse::String& stateJson, double targetCents, bool hasCurrent, int currentNPer, int currentNGen, PitchHit& hit);

/// Milestone 6 (editing workflow): one keyboard step from a JiMS note —
/// `domain` is "lattice" (nearest realizable pitch strictly up/down),
/// "collection" (adjacent member of the placed collection), or "period"
/// (one whole period, same class). Kernel op `step_pitch`.
bool stepPitch(const muse::String& stateJson, int currentNPer, int currentNGen, bool up, const char* domain, PitchHit& hit);

/// Milestone 6: one Kernel-issued change-panel choice (opaque id, labels
/// from the canonical-solfa seam, `current` for the state's own value).
struct StateChangeOption {
    muse::String id;
    muse::String label;      // canonical solfa (tonics/key targets) or a catalogue name
    int nGen = 0;
    int nPer = 0;
    bool hasNGen = false;
    bool current = false;
    std::vector<muse::String> memberLabels;   // rotations/cycles: resulting members
};

/// Milestone 6: everything the change panel may offer for a state.
struct StateChangeOptions {
    std::vector<StateChangeOption> tonics;      // "mode:<nGen>"
    std::vector<StateChangeOption> keyTargets;  // "key:<nPer>:<nGen>" (need a bound reference)
    bool referenceBound = false;
    std::vector<muse::String> bindForms;        // "bind:<form>:<value>"
    std::vector<StateChangeOption> rotations;   // "scale:rotation:<r>"
    std::vector<StateChangeOption> cycles;      // "scale:cycle:<name>"
};

/// Kernel op `state_change_options`.
bool stateChangeOptions(const muse::String& stateJson, StateChangeOptions& options);

/// Kernel op `apply_state_change`: the complete new state JSON after one
/// choice, or false with `error` (unusable reference, foreign id, ...).
bool applyStateChange(const muse::String& stateJson, const muse::String& choiceId, muse::String& newStateJson, muse::String& error);

/// Kernel entry conversion: step/alter/octave to a validated identity.
bool entryFromStandardPitch(char step, int alter, int octave, int& nPer, int& nGen);
}

#endif
