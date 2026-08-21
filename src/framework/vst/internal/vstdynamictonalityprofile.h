/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 */
#pragma once

#include <array>
#include <vector>

#include "mpe/events.h"

#include "../vsttypes.h"

namespace muse::vst {
class VstDynamicTonalityProfileHost
{
public:
    static constexpr PluginParamId PARAM_ID_START = 0x4A500100u;
    static constexpr PluginParamId PARAM_ID_END = 0x4A50017Fu;
    static constexpr PluginParamId PARAM_CAPABILITY = 0x4A500100u;
    static constexpr PluginParamId PARAM_SLOT = 0x4A500101u;
    static constexpr PluginParamId PARAM_GENERATION = 0x4A500102u;
    static constexpr PluginParamId PARAM_COMMIT = 0x4A500119u;
    static constexpr uint32_t CAPABILITY_V1_0 = 0x00010000u;
    static constexpr size_t POINT_COUNT = mpe::DynamicTonalityProfileEvent::POINT_COUNT;

    bool discover(const std::vector<PluginParamInfo>& metadata, PluginParamValue capabilityValue);
    bool supported() const { return m_supported; }

    bool prepare(const mpe::DynamicTonalityProfileEvent& profile, bool force, std::array<ParamChangeEvent, POINT_COUNT>& transaction);
    bool hasCurrentProfile() const { return m_hasCurrentProfile; }
    const mpe::DynamicTonalityProfileEvent& currentProfile() const { return m_currentProfile; }
    uint32_t generation() const { return m_generation; }
    uint32_t slot() const { return m_slot; }

    static bool isReserved(PluginParamId id) { return id >= PARAM_ID_START && id <= PARAM_ID_END; }
    static bool validPayload(const mpe::DynamicTonalityProfileEvent& profile);

private:
    static PluginParamValue encodeU32(uint32_t value);
    static PluginParamValue encodeCommit(uint32_t slot, uint32_t generation);

    bool m_supported = false;
    bool m_hasCurrentProfile = false;
    uint32_t m_generation = 0;
    uint32_t m_slot = 0;
    mpe::DynamicTonalityProfileEvent m_currentProfile;
};
}
