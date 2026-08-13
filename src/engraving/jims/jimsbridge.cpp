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
    return jims_musescore_bridge_abi_version() == 1;
}

bool noteCentsAboveDo(const String& stateJson, int nPer, int nGen, double& cents)
{
    // The staff-frame projection is Kernel-owned end to end: one op, no
    // fork-side ordinate/register/anchor arithmetic.
    String envelope = String(u"{\"abi\":1,\"op\":\"note_cents_above_lower_do\",\"state\":%1,\"nPer\":%2,\"nGen\":%3}")
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
    String envelope = String(u"{\"abi\":1,\"op\":\"notehead_class\",\"state\":%1,\"nGen\":%2}")
                      .arg(stateJson).arg(nGen);
    JsonValue result;
    if (!okResult(callBridge(envelope), result)) {
        return false;
    }
    token = result.toString();
    return true;
}

bool scaleDots(const String& stateJson, std::vector<ScaleDotStack>& stacks)
{
    String envelope = String(u"{\"abi\":1,\"op\":\"scale_dots\",\"state\":%1}").arg(stateJson);
    JsonValue result;
    if (!okResult(callBridge(envelope), result)) {
        return false;
    }
    std::string err;
    JsonDocument stateDoc = JsonDocument::fromJson(stateJson.toUtf8(), &err);
    if (!err.empty()) {
        return false;
    }
    double periodCents = stateDoc.rootObject().value("period_cents").toDouble();

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
