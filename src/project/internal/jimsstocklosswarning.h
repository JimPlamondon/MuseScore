// SPDX-License-Identifier: GPL-3.0-only
// MuseScore-Studio-CLA-applies
#pragma once
#include "iinteractive.h"
namespace mu::project {
inline bool explicitlyDisabledStockLossWarning(const muse::IInteractive::Result& result)
{
    return result.standardButton() == muse::IInteractive::Button::Ok && !result.showAgain();
}
}
