/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 */
#include <gtest/gtest.h>

#include <limits>

#include "internal/vstdynamictonalityprofile.h"

using namespace muse::vst;

namespace {
constexpr std::array<const char*, VstDynamicTonalityProfileHost::POINT_COUNT> TITLES = {
    "jims.profile.capability", "jims.profile.slot", "jims.profile.generation", "jims.profile.schemaVersion",
    "jims.profile.flags", "jims.profile.nPerMin", "jims.profile.nPerMax", "jims.profile.nGenMin",
    "jims.profile.nGenMax", "jims.profile.originNPer", "jims.profile.originNGen", "jims.profile.originFrequencyHi",
    "jims.profile.originFrequencyLo", "jims.profile.periodLog2Hi", "jims.profile.periodLog2Lo",
    "jims.profile.generatorLog2Hi", "jims.profile.generatorLog2Lo", "jims.profile.digestWord0",
    "jims.profile.digestWord1", "jims.profile.digestWord2", "jims.profile.digestWord3", "jims.profile.digestWord4",
    "jims.profile.digestWord5", "jims.profile.digestWord6", "jims.profile.digestWord7", "jims.profile.commit"
};
constexpr std::array<int32_t, VstDynamicTonalityProfileHost::POINT_COUNT> STEPS = {
    0, 1, 0, 0, 0, 127, 127, 34, 34, 127, 34, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void setTitle(Steinberg::Vst::String128 output, const char* input)
{
    size_t i = 0;
    for (; input[i] != '\0'; ++i) {
        output[i] = static_cast<Steinberg::Vst::TChar>(input[i]);
    }
    output[i] = 0;
}

std::vector<PluginParamInfo> validMetadata()
{
    std::vector<PluginParamInfo> result;
    for (size_t i = 0; i < TITLES.size(); ++i) {
        PluginParamInfo info {};
        info.id = VstDynamicTonalityProfileHost::PARAM_ID_START + static_cast<uint32_t>(i);
        info.stepCount = STEPS[i];
        info.flags = Steinberg::Vst::ParameterInfo::kNoFlags;
        setTitle(info.title, TITLES[i]);
        result.push_back(info);
    }
    return result;
}

double capabilityValue()
{
    return static_cast<double>(VstDynamicTonalityProfileHost::CAPABILITY_V1_0) / static_cast<double>(UINT32_MAX);
}

muse::mpe::DynamicTonalityProfileEvent validProfile(double marker = 0.25)
{
    muse::mpe::DynamicTonalityProfileEvent profile;
    for (size_t i = 0; i < profile.points.size(); ++i) {
        profile.points[i] = { VstDynamicTonalityProfileHost::PARAM_ID_START + static_cast<uint32_t>(i), marker };
    }
    profile.points[0].normalized = capabilityValue();
    profile.points[1].normalized = 0.0;
    profile.points[2].normalized = 0.0;
    profile.points.back().normalized = 0.0;
    return profile;
}
}

TEST(VstDynamicTonalityProfileTests, DiscoveryRequiresTheCompleteExactCapability)
{
    VstDynamicTonalityProfileHost host;
    EXPECT_FALSE(host.discover({}, capabilityValue()));

    auto metadata = validMetadata();
    EXPECT_TRUE(host.discover(metadata, capabilityValue()));
    EXPECT_TRUE(host.supported());

    metadata[5].stepCount += 1;
    EXPECT_FALSE(host.discover(metadata, capabilityValue()));
    metadata = validMetadata();
    metadata.push_back(metadata.back());
    EXPECT_FALSE(host.discover(metadata, capabilityValue()));
    metadata = validMetadata();
    metadata.back().id = VstDynamicTonalityProfileHost::PARAM_COMMIT + 1;
    EXPECT_FALSE(host.discover(metadata, capabilityValue()));
    metadata = validMetadata();
    EXPECT_FALSE(host.discover(metadata, capabilityValue() * 2.0));
}

TEST(VstDynamicTonalityProfileTests, TransactionsAdvanceGenerationAlternateSlotsAndSuppressDuplicates)
{
    VstDynamicTonalityProfileHost host;
    ASSERT_TRUE(host.discover(validMetadata(), capabilityValue()));
    std::array<ParamChangeEvent, VstDynamicTonalityProfileHost::POINT_COUNT> transaction;
    const auto profile = validProfile();

    ASSERT_TRUE(host.prepare(profile, false, transaction));
    EXPECT_EQ(host.generation(), 1u);
    EXPECT_EQ(host.slot(), 0u);
    EXPECT_DOUBLE_EQ(transaction[1].value, 0.0);
    EXPECT_DOUBLE_EQ(transaction[2].value, 1.0 / static_cast<double>(UINT32_MAX));
    EXPECT_FALSE(host.prepare(profile, false, transaction));

    ASSERT_TRUE(host.prepare(profile, true, transaction));
    EXPECT_EQ(host.generation(), 2u);
    EXPECT_EQ(host.slot(), 1u);
    EXPECT_DOUBLE_EQ(transaction[1].value, 1.0);

    auto changed = validProfile(0.5);
    ASSERT_TRUE(host.prepare(changed, false, transaction));
    EXPECT_EQ(host.generation(), 3u);
    EXPECT_EQ(host.slot(), 0u);
}

TEST(VstDynamicTonalityProfileTests, MalformedPayloadNeverBecomesCurrent)
{
    VstDynamicTonalityProfileHost host;
    ASSERT_TRUE(host.discover(validMetadata(), capabilityValue()));
    std::array<ParamChangeEvent, VstDynamicTonalityProfileHost::POINT_COUNT> transaction;
    auto profile = validProfile();
    profile.points[4].normalized = std::numeric_limits<double>::quiet_NaN();
    EXPECT_FALSE(host.prepare(profile, false, transaction));
    EXPECT_FALSE(host.hasCurrentProfile());

    profile = validProfile();
    profile.points[4].paramId = profile.points[3].paramId;
    EXPECT_FALSE(host.prepare(profile, false, transaction));
}

TEST(VstDynamicTonalityProfileTests, TransactionRestoresCanonicalIntegerLaneValuesAfterJsonTransport)
{
    VstDynamicTonalityProfileHost host;
    ASSERT_TRUE(host.discover(validMetadata(), capabilityValue()));
    std::array<ParamChangeEvent, VstDynamicTonalityProfileHost::POINT_COUNT> transaction;
    auto profile = validProfile();
    profile.points[13].normalized = 0.2497558594331508;

    ASSERT_TRUE(host.prepare(profile, false, transaction));
    constexpr uint32_t PERIOD_HIGH_WORD = 0x3FF00000u;
    EXPECT_DOUBLE_EQ(transaction[13].value,
                     static_cast<double>(PERIOD_HIGH_WORD) / static_cast<double>(UINT32_MAX));
}
