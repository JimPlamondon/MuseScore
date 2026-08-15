/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 *
 * JiMStaff Milestone 1 — fork-side wrapper over the jims-musescore-bridge
 * C ABI. See jimsbridge.h.
 */
#include "jimsbridge.h"

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

bool noteCentsAboveDo(const String& stateJson, int nPer, int nGen, double& cents)
{
    // The staff-frame projection is Kernel-owned end to end: one op, no
    // fork-side ordinate/register/anchor arithmetic.
    String envelope = String(u"{\"abi\":2,\"op\":\"note_cents_above_lower_do\",\"state\":%1,\"nPer\":%2,\"nGen\":%3}")
                      .arg(stateJson).arg(nPer).arg(nGen);
    JsonValue result;
    if (!okResult(callBridge(envelope), result)) {
        return false;
    }
    cents = result.toDouble();
    return true;
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

bool frameForMelody(const String& stateJson, const String& melodyJson,
                    const String& extentToken, std::vector<StaveSegment>& segments)
{
    String envelope = String(u"{\"abi\":2,\"op\":\"frame_for_melody\",\"state\":%1,\"melody\":%2,\"declared_extent\":\"%3\"}")
                      .arg(stateJson).arg(melodyJson).arg(extentToken);
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
    hit.centsAboveLowerDo = o.value("cents_above_lower_do").toDouble();
    JsonObject cp = o.value("compatibility_pitch").toObject();
    muse::String step = cp.value("step").toString();
    hit.step = step.isEmpty() ? 'C' : step.at(0).toAscii();
    hit.alter = cp.value("alter").toInt();
    hit.octave = cp.value("octave").toInt();
    return true;
}

bool entryFromStandardPitch(char step, int alter, int octave, int& nPer, int& nGen)
{
    String envelope = String(u"{\"abi\":2,\"op\":\"entry_from_standard_pitch\",\"step\":\"%1\",\"alter\":%2,\"octave\":%3}")
                      .arg(String(muse::Char(step))).arg(alter).arg(octave);
    JsonValue result;
    if (!okResult(callBridge(envelope), result)) {
        return false;
    }
    JsonObject o = result.toObject();
    nPer = o.value("nPer").toInt();
    nGen = o.value("nGen").toInt();
    return true;
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
