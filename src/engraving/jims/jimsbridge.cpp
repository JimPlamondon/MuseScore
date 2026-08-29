/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 *
 * JiMStaff Milestone 1 — fork-side wrapper over the jims-musescore-bridge
 * C ABI. See jimsbridge.h.
 */
#include "jimsbridge.h"

#include <cmath>

#include "serialization/json.h"

#include "jims_musescore_bridge.h"

using namespace muse;

namespace mu::engraving::jims {
static String callBridge(const String& envelope)
{
    ByteArray utf8 = envelope.toUtf8();
    char* raw = jims_musescore_bridge_request(utf8.constChar());
    if (!raw) {
        return String();
    }
    String out = String::fromUtf8(raw);
    jims_musescore_bridge_free(raw);
    return out;
}

static bool okResult(const String& response, JsonValue& result)
{
    std::string err;
    JsonDocument doc = JsonDocument::fromJson(response.toUtf8(), &err);
    if (!err.empty()) {
        return false;
    }
    JsonObject root = doc.rootObject();
    if (!root.value("ok").toBool()) {
        return false;
    }
    result = root.value("result");
    return true;
}

bool available()
{
    // V2 contract (Milestone 2 Phase 4): this fork speaks bridge ABI 2.
    return jims_musescore_bridge_abi_version() == 2;
}

bool validateState(const String& stateJson, String& error)
{
    String envelope = String(u"{\"abi\":2,\"op\":\"validate\",\"state\":%1}").arg(stateJson);
    const String response = callBridge(envelope);
    std::string err;
    JsonDocument doc = JsonDocument::fromJson(response.toUtf8(), &err);
    if (!err.empty()) {
        error = String(u"bridge returned no JSON");
        return false;
    }
    JsonObject root = doc.rootObject();
    if (root.value("ok").toBool()) {
        return true;
    }
    error = root.value("error").toString();
    return false;
}

bool noteCentsAboveExtentLower(const String& stateJson, int nPer, int nGen, double& cents)
{
    // The staff-frame projection is Kernel-owned end to end: one op, no
    // fork-side ordinate/register/anchor arithmetic.
    String envelope = String(u"{\"abi\":2,\"op\":\"note_cents_above_extent_lower\",\"state\":%1,\"nPer\":%2,\"nGen\":%3}")
                      .arg(stateJson).arg(nPer).arg(nGen);
    JsonValue result;
    if (!okResult(callBridge(envelope), result)) {
        return false;
    }
    cents = result.toDouble();
    return true;
}

static bool updatedStateResult(const String& envelope, String& updatedState)
{
    JsonValue result;
    if (!okResult(callBridge(envelope), result) || !result.isString()) {
        return false;
    }
    updatedState = result.toString();
    return !updatedState.isEmpty();
}

bool widenExtent(const String& stateJson, int nPer, int nGen, String& updatedState)
{
    return updatedStateResult(
        String(u"{\"abi\":2,\"op\":\"widen_extent\",\"state\":%1,\"nPer\":%2,\"nGen\":%3}")
        .arg(stateJson).arg(nPer).arg(nGen), updatedState);
}

bool fitExtent(const String& stateJson, const String& melodyJson, String& updatedState)
{
    return updatedStateResult(
        String(u"{\"abi\":2,\"op\":\"fit_extent\",\"state\":%1,\"melody\":%2}")
        .arg(stateJson).arg(melodyJson), updatedState);
}

bool defaultVocalExtent(const String& stateJson, int lowKey, int highKey, const char* role, String& updatedState)
{
    return updatedStateResult(
        String(u"{\"abi\":2,\"op\":\"default_vocal_extent\",\"state\":%1,\"low_key\":%2,\"high_key\":%3,\"role\":\"%4\"}")
        .arg(stateJson).arg(lowKey).arg(highKey).arg(String::fromAscii(role)), updatedState);
}

bool noteheadToken(const String& stateJson, int nGen, String& token)
{
    String envelope = String(u"{\"abi\":2,\"op\":\"notehead_class\",\"state\":%1,\"nGen\":%2}")
                      .arg(stateJson).arg(nGen);
    JsonValue result;
    if (!okResult(callBridge(envelope), result)) {
        return false;
    }
    token = result.toString();
    return true;
}

bool tonicCentsAboveDo(const String& stateJson, double& cents)
{
    String envelope = String(u"{\"abi\":2,\"op\":\"tonic_cents_above_do\",\"state\":%1}").arg(stateJson);
    JsonValue result;
    if (!okResult(callBridge(envelope), result)) {
        return false;
    }
    cents = result.toDouble();
    return true;
}

bool periodicOrigins(const String& stateJson, PeriodicOrigins& out)
{
    String envelope = String(u"{\"abi\":2,\"op\":\"periodic_origins\",\"state\":%1}").arg(stateJson);
    JsonValue result;
    if (!okResult(callBridge(envelope), result)) {
        return false;
    }
    JsonObject object = result.toObject();
    out.doCentsAboveExtentLower = object.value("do_cents_above_extent_lower").toDouble();
    out.tonicCentsAboveExtentLower = object.value("tonic_cents_above_extent_lower").toDouble();
    return true;
}

bool staffMetrics(const String& stateJson, double& generatorCents, double& periodCents)
{
    String envelope = String(u"{\"abi\":2,\"op\":\"staff_metrics\",\"state\":%1}").arg(stateJson);
    JsonValue result;
    if (!okResult(callBridge(envelope), result)) {
        return false;
    }
    JsonObject o = result.toObject();
    generatorCents = o.value("generator_cents").toDouble();
    periodCents = o.value("period_cents").toDouble();
    return true;
}

bool generatorRange(double& minCents, double& maxCents)
{
    JsonValue result;
    if (!okResult(callBridge(String(u"{\"abi\":2,\"op\":\"generator_range\"}")), result)) {
        return false;
    }
    JsonObject o = result.toObject();
    minCents = o.value("min_cents").toDouble();
    maxCents = o.value("max_cents").toDouble();
    return true;
}

bool toneDiamondSettings(std::vector<ToneDiamondSetting>& settings, uint32_t& generatorParamId,
                         uint32_t& xParamId, uint32_t& yParamId)
{
    JsonValue result;
    if (!okResult(callBridge(String(u"{\"abi\":2,\"op\":\"tone_diamond_settings\"}")), result)
        || !result.isObject()) {
        return false;
    }
    const JsonObject root = result.toObject();
    generatorParamId = uint32_t(root.value("generator_param_id").toInt());
    xParamId = uint32_t(root.value("x_param_id").toInt());
    yParamId = uint32_t(root.value("y_param_id").toInt());
    settings.clear();
    const JsonArray settingsArray = root.value("settings").toArray();
    for (size_t i = 0; i < settingsArray.size(); ++i) {
        const JsonValue value = settingsArray.at(i);
        const JsonObject object = value.toObject();
        const JsonObject point = object.value("point").toObject();
        ToneDiamondSetting setting;
        setting.id = object.value("id").toString();
        setting.label = object.value("label").toString();
        setting.x = point.value("x").toDouble();
        setting.y = point.value("y").toDouble();
        if (setting.id.isEmpty() || setting.label.isEmpty()) {
            settings.clear();
            return false;
        }
        settings.push_back(std::move(setting));
    }
    return !settings.empty();
}

bool labelLegibilityRange(double& minCents, double& maxCents)
{
    JsonValue result;
    if (!okResult(callBridge(String(u"{\"abi\":2,\"op\":\"label_legibility_range\"}")), result)) {
        return false;
    }
    JsonObject o = result.toObject();
    minCents = o.value("min_cents").toDouble();
    maxCents = o.value("max_cents").toDouble();
    return true;
}

bool scaleDotLabels(const String& stateJson, std::vector<LabeledDotStack>& stacks)
{
    String envelope = String(u"{\"abi\":2,\"op\":\"scale_dot_labels\",\"state\":%1}").arg(stateJson);
    JsonValue result;
    if (!okResult(callBridge(envelope), result)) {
        return false;
    }
    double generatorCents = 0.0;
    double periodCents = 0.0;
    if (!staffMetrics(stateJson, generatorCents, periodCents)) {
        return false;
    }
    stacks.clear();
    JsonArray array = result.toArray();
    for (size_t i = 0; i < array.size(); ++i) {
        JsonObject stack = array.at(i).toObject();
        LabeledDotStack out;
        out.cents = stack.value("ordinate").toDouble() * periodCents;
        JsonArray members = stack.value("members").toArray();
        for (size_t j = 0; j < members.size(); ++j) {
            JsonObject member = members.at(j).toObject();
            out.members.push_back({ member.value("nGen").toInt(),
                                    member.value("label").toString() });
        }
        stacks.push_back(out);
    }
    return true;
}

bool jiLines(const String& stateJson, std::vector<JiLine>& lines)
{
    String envelope = String(u"{\"abi\":2,\"op\":\"ji_lines\",\"state\":%1}").arg(stateJson);
    JsonValue result;
    if (!okResult(callBridge(envelope), result)) {
        return false;
    }
    lines.clear();
    JsonArray array = result.toArray();
    for (size_t i = 0; i < array.size(); ++i) {
        JsonObject line = array.at(i).toObject();
        lines.push_back({ line.value("cents").toDouble(),
                          line.value("limit").toInt(),
                          line.value("visible").toBool() });
    }
    return true;
}

static ChangePoint readPoint(const JsonObject& o)
{
    ChangePoint p;
    p.nGen = o.value("nGen").toInt();
    p.label = o.value("label").toString();
    p.ordinate = o.value("ordinate").toDouble();
    p.periodOffset = o.value("period_offset").toInt();
    return p;
}

static bool stringResult(const String& response, String& out, String* error)
{
    std::string err;
    JsonDocument doc = JsonDocument::fromJson(response.toUtf8(), &err);
    if (!err.empty()) {
        if (error) {
            *error = u"bridge returned no JSON";
        }
        return false;
    }
    JsonObject root = doc.rootObject();
    if (!root.value("ok").toBool()) {
        if (error) {
            *error = root.value("error").toString();
        }
        return false;
    }
    if (!root.value("result").isString()) {
        if (error) {
            *error = u"bridge result is not a string";
        }
        return false;
    }
    out = root.value("result").toString();
    return true;
}

bool tonicAmbitForMelody(const String& stateJson, const String& melodyJson, String& token, String* error)
{
    String envelope = String(u"{\"abi\":2,\"op\":\"tonic_ambit\",\"state\":%1,\"melody\":%2}")
                      .arg(stateJson).arg(melodyJson);
    if (!stringResult(callBridge(envelope), token, error)) {
        return false;
    }
    if (token != u"tonic-bounded" && token != u"tonic-centered") {
        if (error) {
            *error = u"Kernel returned an unknown tonic-ambit token: " + token;
        }
        return false;
    }
    return true;
}

bool musicxmlStaffStateV3Xml(const String& stateJson, int staffNumber, String& out, String* error)
{
    String envelope = staffNumber > 0
                      ? String(u"{\"abi\":2,\"op\":\"musicxml_staff_state_v3_xml\",\"state\":%1,\"staff_number\":%2}")
                      .arg(stateJson).arg(staffNumber)
                      : String(u"{\"abi\":2,\"op\":\"musicxml_staff_state_v3_xml\",\"state\":%1}").arg(stateJson);
    return stringResult(callBridge(envelope), out, error);
}

bool musicxmlSharedStateV3Xml(const String& stateJson, String& out, String* error)
{
    String envelope = String(u"{\"abi\":2,\"op\":\"musicxml_shared_state_v3_xml\",\"state\":%1}").arg(stateJson);
    return stringResult(callBridge(envelope), out, error);
}

bool musicxmlChangeEventV3Xml(const String& oldStateJson, const String& newStateJson, String& out, String* error)
{
    String envelope = String(u"{\"abi\":2,\"op\":\"musicxml_change_event_v3_xml\",\"old_state\":%1,\"new_state\":%2}")
                      .arg(oldStateJson).arg(newStateJson);
    return stringResult(callBridge(envelope), out, error);
}

static bool readSoundingPitch(const String& response, SoundingPitch& out, String* error)
{
    JsonValue result;
    if (!okResult(response, result)) {
        if (error) {
            std::string err;
            JsonDocument doc = JsonDocument::fromJson(response.toUtf8(), &err);
            *error = err.empty() ? doc.rootObject().value("error").toString() : String(u"bridge returned no JSON");
        }
        return false;
    }
    JsonObject o = result.toObject();
    // Every field the transport relies on must be present and well-typed;
    // a malformed answer is a failure, never a default pitch.
    for (const char* key : { "nPer", "nGen", "frequency_hz", "midi_key", "cents_offset", "step", "alter", "octave",
                             "reference_key_number", "reference_frequency_hz", "anchor" }) {
        if (!o.contains(key)) {
            if (error) {
                *error = String(u"note_sounding_pitch answer lacks %1").arg(String::fromAscii(key));
            }
            return false;
        }
    }
    out.nPer = o.value("nPer").toInt();
    out.nGen = o.value("nGen").toInt();
    out.frequencyHz = o.value("frequency_hz").toDouble();
    out.midiKey = o.value("midi_key").toInt();
    out.centsOffset = o.value("cents_offset").toDouble();
    String step = o.value("step").toString();
    out.step = step.isEmpty() ? '\0' : step.at(0).toAscii();
    out.alter = o.value("alter").toInt();
    out.octave = o.value("octave").toInt();
    out.referenceKeyNumber = o.value("reference_key_number").toInt();
    out.referenceFrequencyHz = o.value("reference_frequency_hz").toDouble();
    out.anchor = o.value("anchor").toString();
    if (out.midiKey < 0 || out.midiKey > 127 || !(out.frequencyHz > 0.0)
        || std::abs(out.centsOffset) > 50.0 + 1e-9 || step.size() != 1
        || String(u"CDEFGAB").indexOf(step.at(0)) == muse::nidx) {
        if (error) {
            *error = u"note_sounding_pitch answer out of range";
        }
        return false;
    }
    return true;
}

bool noteSoundingPitch(const String& stateJson, int nPer, int nGen, SoundingPitch& out, String* error)
{
    String envelope = String(u"{\"abi\":2,\"op\":\"note_sounding_pitch\",\"state\":%1,\"nPer\":%2,\"nGen\":%3}")
                      .arg(stateJson).arg(nPer).arg(nGen);
    return readSoundingPitch(callBridge(envelope), out, error);
}

bool vst3ProfileTransaction(const String& stateJson, uint32_t slot, uint32_t generation, uint32_t sampleOffset,
                            mpe::DynamicTonalityProfileEvent& out, String* error)
{
    String envelope = String(u"{\"abi\":2,\"op\":\"vst3_profile_transaction\",\"state\":%1,"
                             "\"slot\":%2,\"generation\":%3,\"sample_offset\":%4}")
                      .arg(stateJson)
                      .arg(static_cast<int64_t>(slot))
                      .arg(static_cast<int64_t>(generation))
                      .arg(static_cast<int64_t>(sampleOffset));
    JsonValue result;
    if (!okResult(callBridge(envelope), result)) {
        if (error) {
            *error = u"Kernel rejected the VST3 tuning profile";
        }
        return false;
    }
    JsonArray points = result.toArray();
    if (points.size() != mpe::DynamicTonalityProfileEvent::POINT_COUNT) {
        if (error) {
            *error = u"Kernel returned an incomplete VST3 tuning profile";
        }
        return false;
    }
    for (size_t i = 0; i < points.size(); ++i) {
        JsonObject point = points.at(i).toObject();
        out.points[i].paramId = static_cast<uint32_t>(point.value("param_id").toInt());
        out.points[i].normalized = point.value("normalized").toDouble();
    }
    return true;
}

static void readTonicPitchLabel(const JsonObject& o, TonicPitchLabel& out);

bool tonicPitchLabel(const String& stateJson, TonicPitchLabel& out)
{
    String envelope = String(u"{\"abi\":2,\"op\":\"tonic_pitch_label\",\"state\":%1}").arg(stateJson);
    JsonValue result;
    if (!okResult(callBridge(envelope), result)) {
        return false;
    }
    readTonicPitchLabel(result.toObject(), out);
    return !out.label.isEmpty();
}

bool tonicPitchLabelInPeriod(const String& stateJson, int periodIndex, TonicPitchLabel& out)
{
    String envelope = String(u"{\"abi\":2,\"op\":\"tonic_pitch_label\",\"state\":%1,\"period_index\":%2}")
                      .arg(stateJson).arg(periodIndex);
    JsonValue result;
    if (!okResult(callBridge(envelope), result)) {
        return false;
    }
    readTonicPitchLabel(result.toObject(), out);
    return !out.label.isEmpty();
}

static void readTonicPitchLabel(const JsonObject& o, TonicPitchLabel& out)
{
    out.label = o.value("label").toString();
    out.keyNumber = o.value("key_number").toInt();
    out.nPer = o.value("nPer").toInt();
    out.nGen = o.value("nGen").toInt();
}

bool changeIndicator(const String& oldStateJson, const String& newStateJson, ChangeIndicator& out, String* error)
{
    String envelope = String(u"{\"abi\":2,\"op\":\"change_indicator\",\"old_state\":%1,\"new_state\":%2}")
                      .arg(oldStateJson).arg(newStateJson);
    const String response = callBridge(envelope);
    JsonValue result;
    if (!okResult(response, result)) {
        if (error) {
            std::string err;
            JsonDocument doc = JsonDocument::fromJson(response.toUtf8(), &err);
            *error = err.empty() ? doc.rootObject().value("error").toString() : String(u"bridge returned no JSON");
        }
        return false;
    }
    out = ChangeIndicator();
    JsonObject o = result.toObject();
    JsonArray kinds = o.value("kinds").toArray();
    for (size_t i = 0; i < kinds.size(); ++i) {
        out.kinds.push_back(kinds.at(i).toString());
    }
    JsonObject terrain = o.value("terrain").toObject();
    JsonArray stacks = terrain.value("dot_stacks").toArray();
    for (size_t i = 0; i < stacks.size(); ++i) {
        JsonObject s = stacks.at(i).toObject();
        ChangeStack stack;
        stack.ordinate = s.value("ordinate").toDouble();
        stack.periodOffset = s.value("period_offset").toInt();
        JsonArray members = s.value("members").toArray();
        for (size_t j = 0; j < members.size(); ++j) {
            stack.members.push_back(readPoint(members.at(j).toObject()));
        }
        out.dotStacks.push_back(stack);
    }
    JsonArray tonics = terrain.value("tonic_indicators").toArray();
    for (size_t i = 0; i < tonics.size(); ++i) {
        out.tonicIndicators.push_back(readPoint(tonics.at(i).toObject()));
    }
    JsonArray arrows = terrain.value("arrows").toArray();
    for (size_t i = 0; i < arrows.size(); ++i) {
        JsonObject a = arrows.at(i).toObject();
        ChangeArrow arrow;
        arrow.kind = a.value("kind").toString();
        arrow.from = readPoint(a.value("from").toObject());
        arrow.to = readPoint(a.value("to").toObject());
        arrow.up = a.value("direction").toString() == u"up";
        arrow.trumps = a.value("trumps").toString();
        out.arrows.push_back(arrow);
    }
    return true;
}

bool connectorGlyph(ConnectorGlyph& out)
{
    JsonValue result;
    if (!okResult(callBridge(String(u"{\"abi\":2,\"op\":\"connector_glyph\"}")), result)) {
        return false;
    }
    JsonObject o = result.toObject();
    out.penCents = o.value("pen_cents").toDouble();
    out.headHeightCents = o.value("head_height_cents").toDouble();
    out.headHalfWidthCents = o.value("head_half_width_cents").toDouble();
    return true;
}

static String jimsExtraCentsJson(const std::vector<double>& extraCents)
{
    String out = u"[";
    for (size_t i = 0; i < extraCents.size(); ++i) {
        if (i) {
            out += u",";
        }
        out += String::number(extraCents[i], 6);
    }
    return out + u"]";
}

bool frameForMelody(const String& stateJson, const String& melodyJson,
                    const String& extentToken, std::vector<StaveSegment>& segments,
                    const std::vector<double>& extraCents)
{
    // Owner rule 2026-08-19 (7b): extra cents the frame must cover ride in
    // the same op's options; without them the envelope is byte-identical
    // to the Milestone-4 request.
    String envelope = extraCents.empty()
                      ? String(u"{\"abi\":2,\"op\":\"frame_for_melody\",\"state\":%1,\"melody\":%2,\"declared_extent\":\"%3\"}")
                      .arg(stateJson).arg(melodyJson).arg(extentToken)
                      : String(u"{\"abi\":2,\"op\":\"frame_for_melody\",\"state\":%1,\"melody\":%2,\"declared_extent\":\"%3\","
                               u"\"options\":{\"extra_cents\":%4}}")
                      .arg(stateJson).arg(melodyJson).arg(extentToken).arg(jimsExtraCentsJson(extraCents));
    JsonValue result;
    if (!okResult(callBridge(envelope), result)) {
        return false;
    }
    segments.clear();
    JsonArray array = result.toObject().value("segments").toArray();
    for (size_t i = 0; i < array.size(); ++i) {
        JsonObject seg = array.at(i).toObject();
        segments.push_back({ seg.value("lower_cents").toDouble(),
                             seg.value("upper_cents").toDouble(),
                             seg.value("whole").toBool() });
    }
    return !segments.empty();
}

bool frameBandsForMelody(const String& stateJson, const String& melodyJson,
                         const String& extentToken, bool elideEmptyPeriods, int minBandPeriods,
                         FrameBands& out, const std::vector<double>& extraCents)
{
    String envelope = extraCents.empty()
                      ? String(
        u"{\"abi\":2,\"op\":\"frame_for_melody\",\"state\":%1,\"melody\":%2,\"declared_extent\":\"%3\","
        u"\"options\":{\"elide_empty_periods\":%4,\"min_band_periods\":%5}}")
                      .arg(stateJson).arg(melodyJson).arg(extentToken)
                      .arg(String(elideEmptyPeriods ? u"true" : u"false")).arg(minBandPeriods)
                      : String(
        u"{\"abi\":2,\"op\":\"frame_for_melody\",\"state\":%1,\"melody\":%2,\"declared_extent\":\"%3\","
        u"\"options\":{\"elide_empty_periods\":%4,\"min_band_periods\":%5,\"extra_cents\":%6}}")
                      .arg(stateJson).arg(melodyJson).arg(extentToken)
                      .arg(String(elideEmptyPeriods ? u"true" : u"false")).arg(minBandPeriods)
                      .arg(jimsExtraCentsJson(extraCents));
    JsonValue result;
    if (!okResult(callBridge(envelope), result)) {
        return false;
    }
    JsonObject o = result.toObject();
    if (o.value("schema").toString() != u"jims.frame-bands.v1") {
        return false;
    }
    out = FrameBands();
    out.omittedPeriodCount = o.value("omitted_period_count").toInt();
    JsonArray bands = o.value("bands").toArray();
    for (size_t i = 0; i < bands.size(); ++i) {
        JsonObject b = bands.at(i).toObject();
        FrameBand band;
        JsonArray segs = b.value("segments").toArray();
        for (size_t j = 0; j < segs.size(); ++j) {
            JsonObject seg = segs.at(j).toObject();
            band.segments.push_back({ seg.value("lower_cents").toDouble(),
                                      seg.value("upper_cents").toDouble(),
                                      seg.value("whole").toBool() });
        }
        band.lowerCents = b.value("lower_cents").toDouble();
        band.upperCents = b.value("upper_cents").toDouble();
        band.lowestPeriodIndex = b.value("lowest_period_index").toInt();
        band.highestPeriodIndex = b.value("highest_period_index").toInt();
        band.labelPeriodIndex = b.value("label_period_index").toInt();
        readTonicPitchLabel(b.value("tonic_label").toObject(), band.tonicLabel);
        if (band.segments.empty()) {
            return false;
        }
        out.bands.push_back(band);
    }
    return !out.bands.empty();
}

bool nearestPitch(const String& stateJson, double targetCents,
                  bool hasCurrent, int currentNPer, int currentNGen, PitchHit& hit)
{
    String current = hasCurrent
                     ? String(u"{\"nPer\":%1,\"nGen\":%2}").arg(currentNPer).arg(currentNGen)
                     : String(u"null");
    String envelope = String(u"{\"abi\":2,\"op\":\"nearest_pitch\",\"state\":%1,\"target_cents\":%2,\"current\":%3}")
                      .arg(stateJson).arg(String::number(targetCents, 6)).arg(current);
    JsonValue result;
    if (!okResult(callBridge(envelope), result)) {
        return false;
    }
    JsonObject o = result.toObject();
    hit.nPer = o.value("nPer").toInt();
    hit.nGen = o.value("nGen").toInt();
    hit.centsAboveExtentLower = o.value("cents_above_extent_lower").toDouble();
    JsonObject cp = o.value("compatibility_pitch").toObject();
    muse::String step = cp.value("step").toString();
    hit.step = step.isEmpty() ? 'C' : step.at(0).toAscii();
    hit.alter = cp.value("alter").toInt();
    hit.octave = cp.value("octave").toInt();
    return true;
}

static void readPitchHit(const JsonObject& o, PitchHit& hit)
{
    hit.nPer = o.value("nPer").toInt();
    hit.nGen = o.value("nGen").toInt();
    hit.centsAboveExtentLower = o.value("cents_above_extent_lower").toDouble();
    JsonObject cp = o.value("compatibility_pitch").toObject();
    muse::String step = cp.value("step").toString();
    hit.step = step.isEmpty() ? 'C' : step.at(0).toAscii();
    hit.alter = cp.value("alter").toInt();
    hit.octave = cp.value("octave").toInt();
}

bool stepPitch(const String& stateJson, int currentNPer, int currentNGen,
               bool up, const char* domain, PitchHit& hit)
{
    String envelope = String(
        u"{\"abi\":2,\"op\":\"step_pitch\",\"state\":%1,\"current\":{\"nPer\":%2,\"nGen\":%3},\"up\":%4,\"domain\":\"%5\"}")
                      .arg(stateJson).arg(currentNPer).arg(currentNGen)
                      .arg(String(up ? u"true" : u"false")).arg(String::fromAscii(domain));
    JsonValue result;
    if (!okResult(callBridge(envelope), result)) {
        return false;
    }
    readPitchHit(result.toObject(), hit);
    return true;
}

static StateChangeOption readOption(const JsonObject& o)
{
    StateChangeOption opt;
    opt.id = o.value("id").toString();
    opt.label = o.value("label").toString();
    opt.hasNGen = o.contains("nGen");
    opt.nGen = o.value("nGen").toInt();
    opt.nPer = o.value("nPer").toInt();
    opt.current = o.value("current").toBool();
    if (o.contains("members") && o.value("members").isArray()) {
        JsonArray members = o.value("members").toArray();
        for (size_t i = 0; i < members.size(); ++i) {
            opt.memberLabels.push_back(members.at(i).toObject().value("label").toString());
        }
    }
    return opt;
}

bool stateChangeOptions(const String& stateJson, StateChangeOptions& options)
{
    String envelope = String(u"{\"abi\":2,\"op\":\"state_change_options\",\"state\":%1}").arg(stateJson);
    JsonValue result;
    if (!okResult(callBridge(envelope), result)) {
        return false;
    }
    JsonObject o = result.toObject();
    options = StateChangeOptions();
    auto readList = [&](const char* key, std::vector<StateChangeOption>& out) {
        if (!o.contains(key) || !o.value(key).isArray()) {
            return;
        }
        JsonArray arr = o.value(key).toArray();
        for (size_t i = 0; i < arr.size(); ++i) {
            out.push_back(readOption(arr.at(i).toObject()));
        }
    };
    readList("tonics", options.tonics);
    readList("key_targets", options.keyTargets);
    readList("rotations", options.rotations);
    readList("cycles", options.cycles);
    options.referenceBound = o.value("reference_bound").toBool();
    if (o.contains("bind_forms") && o.value("bind_forms").isArray()) {
        JsonArray forms = o.value("bind_forms").toArray();
        for (size_t i = 0; i < forms.size(); ++i) {
            options.bindForms.push_back(forms.at(i).toString());
        }
    }
    return true;
}

bool applyStateChange(const String& stateJson, const String& choiceId, String& newStateJson, String& error)
{
    String envelope = String(u"{\"abi\":2,\"op\":\"apply_state_change\",\"state\":%1,\"choice\":\"%2\"}")
                      .arg(stateJson).arg(choiceId);
    const String response = callBridge(envelope);
    std::string err;
    JsonDocument doc = JsonDocument::fromJson(response.toUtf8(), &err);
    if (!err.empty()) {
        error = u"bridge returned no JSON";
        return false;
    }
    JsonObject root = doc.rootObject();
    if (!root.value("ok").toBool()) {
        error = root.value("error").toString();
        return false;
    }
    // The Kernel's own JSON text IS the state: slice it out of the envelope
    // verbatim ({"ok":true,"result":{...}}) rather than re-serializing
    // through a JSON library that would reorder keys.
    static const String marker(u"\"result\":");
    const size_t at = response.indexOf(marker);
    if (at == muse::nidx || !response.endsWith(u"}")) {
        error = u"bridge envelope has no result";
        return false;
    }
    newStateJson = response.mid(at + marker.size(), response.size() - (at + marker.size()) - 1);
    return true;
}

bool entryFromStandardPitch(const String& stateJson, char step, int alter, int octave,
                            SoundingPitch& out, String* error)
{
    String envelope = String(u"{\"abi\":2,\"op\":\"entry_from_standard_pitch\",\"state\":%1,\"step\":\"%2\",\"alter\":%3,\"octave\":%4}")
                      .arg(stateJson).arg(String(muse::Char(step))).arg(alter).arg(octave);
    return readSoundingPitch(callBridge(envelope), out, error);
}

bool noteContinuation(const String& stateJson, double frequencyHz, SoundingPitch& out, String* error)
{
    String envelope = String(u"{\"abi\":2,\"op\":\"note_continuation\",\"state\":%1,\"frequency_hz\":%2}")
                      .arg(stateJson).arg(String::number(frequencyHz, 17));
    return readSoundingPitch(callBridge(envelope), out, error);
}

bool scaleDots(const String& stateJson, std::vector<ScaleDotStack>& stacks)
{
    String envelope = String(u"{\"abi\":2,\"op\":\"scale_dots\",\"state\":%1}").arg(stateJson);
    JsonValue result;
    if (!okResult(callBridge(envelope), result)) {
        return false;
    }
    double generatorCents = 0.0;
    double periodCents = 0.0;
    if (!staffMetrics(stateJson, generatorCents, periodCents)) {
        return false;
    }

    stacks.clear();
    JsonArray array = result.toArray();
    for (size_t i = 0; i < array.size(); ++i) {
        JsonObject stack = array.at(i).toObject();
        ScaleDotStack out;
        out.cents = stack.value("ordinate").toDouble() * periodCents;
        JsonArray members = stack.value("front_to_back").toArray();
        for (size_t j = 0; j < members.size(); ++j) {
            out.frontToBack.push_back(members.at(j).toInt());
        }
        stacks.push_back(out);
    }
    return true;
}
}
