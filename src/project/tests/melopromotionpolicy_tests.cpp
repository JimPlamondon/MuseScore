// SPDX-License-Identifier: GPL-3.0-only
// MuseScore-Studio-CLA-applies
#include <gtest/gtest.h>
#include "global/productpolicy.h"
#include "global/settings.h"
#include "project/internal/projectconfiguration.h"

using namespace muse;

TEST(MeloPromotionPolicy, AudioComCrossPromotionCannotUseRetainedSharingPreferences)
{
    if (productPromotionsEnabled()) {
        GTEST_SKIP() << "Upstream promotions enabled for this build";
    }
    settings()->beginTransaction(false);
    settings()->setLocalValue(Settings::Key("project", "project/alsoShareAudioCom"), Val(true));
    settings()->setLocalValue(Settings::Key("project", "project/showAlsoShareAudioComDialog"), Val(true));
    mu::project::ProjectConfiguration config(modularity::globalCtx());
    EXPECT_FALSE(config.alsoShareAudioCom());
    EXPECT_FALSE(config.showAlsoShareAudioComDialog());
    settings()->rollbackTransaction(false);
}
