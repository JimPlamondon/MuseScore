/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 */
#include <gtest/gtest.h>

#include <limits>
#include <utility>
#include <vector>

#include "vst/internal/synth/vstsynthesiser.h"

using namespace muse::vst;

TEST(VstPendingHostParameterQueueTests, ReplaysOnlyTheLatestFiniteValuesAfterReadiness)
{
    detail::PendingHostParameterQueue queue;

    EXPECT_TRUE(queue.enqueue(0x4A530020u, 700.0));
    EXPECT_TRUE(queue.enqueue(0x4A530020u, 690.0));
    EXPECT_TRUE(queue.enqueue(0x4A530022u, 0.25));
    EXPECT_FALSE(queue.enqueue(0x4A530023u, std::numeric_limits<double>::quiet_NaN()));
    EXPECT_EQ(queue.size(), 2u);

    std::vector<std::pair<uint32_t, double> > applied;
    queue.replay([&applied](uint32_t paramId, double plain) {
        applied.emplace_back(paramId, plain);
    });

    ASSERT_EQ(applied.size(), 2u);
    EXPECT_EQ(applied[0].first, 0x4A530020u);
    EXPECT_DOUBLE_EQ(applied[0].second, 690.0);
    EXPECT_EQ(applied[1].first, 0x4A530022u);
    EXPECT_DOUBLE_EQ(applied[1].second, 0.25);
    EXPECT_EQ(queue.size(), 0u);
}

TEST(VstPendingHostParameterQueueTests, FailureAndReplacementDiscardOneInstancesPendingValues)
{
    detail::PendingHostParameterQueue failedInstance;
    detail::PendingHostParameterQueue replacementInstance;

    ASSERT_TRUE(failedInstance.enqueue(0x4A530020u, 690.0));
    failedInstance.clear();
    ASSERT_TRUE(replacementInstance.enqueue(0x4A530020u, 700.0));

    std::vector<double> failedApplied;
    failedInstance.replay([&failedApplied](uint32_t, double plain) {
        failedApplied.push_back(plain);
    });
    std::vector<double> replacementApplied;
    replacementInstance.replay([&replacementApplied](uint32_t, double plain) {
        replacementApplied.push_back(plain);
    });

    EXPECT_TRUE(failedApplied.empty());
    ASSERT_EQ(replacementApplied.size(), 1u);
    EXPECT_DOUBLE_EQ(replacementApplied.front(), 700.0);
}
