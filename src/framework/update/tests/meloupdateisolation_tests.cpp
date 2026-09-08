/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 *
 * MuseScore
 * Music Composition & Notation
 *
 * Copyright (C) 2024 MuseScore Limited and others
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <gmock/gmock.h>

#include "update/internal/appupdatescenario.h"
#include "update/internal/updateactioncontroller.h"
#include "update/internal/updateconfiguration.h"

using namespace muse;
using namespace muse::update;

class MeloUpdateServiceMock : public IAppUpdateService
{
public:
    using CheckPromise = async::Promise<RetVal<ReleaseInfo> >;
    MOCK_METHOD(CheckPromise, checkForUpdate, (), (override));
    MOCK_METHOD(const RetVal<ReleaseInfo>&, lastCheckResult, (), (const, override));
    MOCK_METHOD(RetVal<Progress>, downloadRelease, (), (override));
};

TEST(MeloUpdateIsolation, ManualAndAutomaticChecksNeverCallUpstreamService)
{
    const auto context = modularity::globalCtx();
    auto config = std::make_shared<UpdateConfiguration>(context);
    auto service = std::make_shared<testing::StrictMock<MeloUpdateServiceMock> >();
    AppUpdateScenario scenario(context);
    auto* global = modularity::globalIoc();
    auto previousConfig = global->resolve<IUpdateConfiguration>("meloupdate-test");
    global->unregister<IUpdateConfiguration>("meloupdate-test");
    global->registerExport<IUpdateConfiguration>("meloupdate-test", config);
    auto* local = modularity::ioc(context);
    auto previousService = local->resolve<IAppUpdateService>("meloupdate-test");
    local->unregister<IAppUpdateService>("meloupdate-test");
    local->registerExport<IAppUpdateService>("meloupdate-test", service);
    EXPECT_FALSE(config->isAppUpdatable());
    EXPECT_FALSE(scenario.needCheckForUpdate());
    EXPECT_CALL(*service, checkForUpdate()).Times(0);
    scenario.checkForUpdate(false);
    scenario.checkForUpdate(true);
    EXPECT_FALSE(scenario.checkInProgress());
    UpdateActionController controller(context);
    EXPECT_FALSE(controller.canReceiveAction("check-update"));
    global->unregister<IUpdateConfiguration>("meloupdate-test");
    if (previousConfig) {
        global->registerExport<IUpdateConfiguration>("meloupdate-test", previousConfig);
    }
    local->unregister<IAppUpdateService>("meloupdate-test");
    if (previousService) {
        local->registerExport<IAppUpdateService>("meloupdate-test", previousService);
    }
}
