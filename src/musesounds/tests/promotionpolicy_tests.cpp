// SPDX-License-Identifier: GPL-3.0-only
// MuseScore-CLA-applies
#include <gtest/gtest.h>
#include "global/productpolicy.h"
#include "appshell/internal/appshellconfiguration.h"
#include "musesounds/internal/musesoundsconfiguration.h"
#include "musesounds/internal/musesoundscheckupdatescenario.h"
#include "musesounds/internal/musesoundsrepository.h"
#include "musesounds/musesoundserrors.h"

using namespace muse;
using namespace mu::musesounds;

// No service or network dependencies are provided: suppressed entry points must
// return before consulting cached offers, creating requests or opening dialogs.
TEST(MeloPromotionPolicy, StartupAndSavedPromotionPreferencesCannotEnableOffers)
{
    if (productPromotionsEnabled()) {
        GTEST_SKIP() << "Upstream promotions enabled for this build";
    }
    mu::appshell::AppShellConfiguration shell(modularity::globalCtx());
    MuseSoundsConfiguration sounds(modularity::globalCtx());
    EXPECT_FALSE(shell.welcomeDialogShowOnStartup());
    EXPECT_FALSE(sounds.needCheckForMuseSoundsUpdate());
}

TEST(MeloPromotionPolicy, AutomaticManualAndCachedCampaignEntryPointsAreSuppressed)
{
    if (productPromotionsEnabled()) {
        GTEST_SKIP() << "Upstream promotions enabled for this build";
    }
    MuseSoundsCheckUpdateScenario scenario(modularity::globalCtx());
    EXPECT_FALSE(scenario.needCheckForUpdate());
    scenario.checkForUpdate(false);
    scenario.checkForUpdate(true);
    EXPECT_FALSE(scenario.checkInProgress());
    EXPECT_FALSE(scenario.hasUpdate());
    EXPECT_EQ(scenario.showUpdate().code(), static_cast<int>(Err::NoUpdate));
}

TEST(MeloPromotionPolicy, StorefrontDoesNotCreateNetworkRequests)
{
    if (productPromotionsEnabled()) {
        GTEST_SKIP() << "Upstream promotions enabled for this build";
    }
    MuseSoundsRepository repository(modularity::globalCtx());
    repository.init();
    EXPECT_TRUE(repository.soundsCatalogueList().empty());
}
