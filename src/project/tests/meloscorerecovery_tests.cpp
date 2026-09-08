// SPDX-License-Identifier: GPL-3.0-only
// MuseScore-Studio-CLA-applies
#include <gtest/gtest.h>
#include "project/internal/projectautosaver.h"
#include "project/internal/projectconfiguration.h"

using namespace muse;
using namespace mu::project;

TEST(MeloScoreRecovery, AutosaveRestoresOriginalPathWithoutCreatingAnUncompressedFolder)
{
    ProjectAutoSaver autosaver(modularity::globalCtx());
    for (const auto* name : { "/scores/My score.meloscore", "/scores/Étude.v2.meloscore", "/scores/old.mscz" }) {
        const io::path_t path(String::fromUtf8(name));
        const auto recovery = autosaver.projectAutoSavePath(path);
        EXPECT_EQ(recovery, path.appendingSuffix("autosave"));
        EXPECT_EQ(autosaver.projectOriginalPath(recovery), path);
    }
}

TEST(MeloScoreRecovery, BackupKeepsNativeSuffixAndHasAnIndependentDirectory)
{
    ProjectConfiguration config(modularity::globalCtx());
    EXPECT_EQ(config.projectBackupPath("/scores/Étude.v2.meloscore").toStdString(),
              "/scores/.melopresto-backup/.Étude.v2.meloscore~");
}
