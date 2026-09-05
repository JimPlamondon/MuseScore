// SPDX-License-Identifier: GPL-3.0-only
// MuseScore-Studio-CLA-applies
#include <gtest/gtest.h>
#include "project/internal/jimsstocklosswarning.h"
using namespace muse;
using namespace mu::project;
TEST(JimsStockLossWarning, OnlyExplicitAcknowledgementCanDisableFutureWarnings)
{
    for (auto button : { IInteractive::Button::NoButton, IInteractive::Button::Cancel, IInteractive::Button::Close,
                         IInteractive::Button::Ok }) {
        for (bool showAgain : { false, true }) {
            IInteractive::Result result(int(button), showAgain);
            if (button == IInteractive::Button::Ok && !showAgain) {
                EXPECT_TRUE(explicitlyDisabledStockLossWarning(result));
            } else {
                EXPECT_FALSE(explicitlyDisabledStockLossWarning(result)) << int(button) << ":" << showAgain;
            }
        }
    }
    EXPECT_FALSE(explicitlyDisabledStockLossWarning(IInteractive::Result()));
}
