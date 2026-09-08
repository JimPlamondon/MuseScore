// SPDX-License-Identifier: GPL-3.0-only
// MuseScore-CLA-applies
#pragma once

#include "muse_framework_config.h"

namespace muse {
// Upstream behavior is the fallback when the downstream suppression option is absent.
inline constexpr bool productPromotionsEnabled()
{
#ifdef MELO_SCORE_SUPPRESS_UPSTREAM_PROMOTIONS
    return false;
#else
    return true;
#endif
}
}
