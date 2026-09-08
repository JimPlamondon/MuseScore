/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 *
 * MuseScore Studio
 * Music Composition & Notation
 *
 * Copyright (C) 2021 MuseScore Limited and others
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
#include <gtest/gtest.h>

#include "io/buffer.h"
#include "engraving/infrastructure/mscreader.h"
#include "engraving/infrastructure/mscwriter.h"

using namespace muse;
using namespace muse::io;
using namespace mu::engraving;

TEST(Engraving_MeloScoreIoTests, NativeFormatDispatch)
{
    for (const auto* suffix : { "meloscore", "meloscore~", "mscz", "mscz~" }) {
        EXPECT_TRUE(isMuseScoreFile(suffix));
        EXPECT_EQ(mscIoModeBySuffix(suffix), MscIoMode::Zip);
    }
    EXPECT_EQ(mscIoModeBySuffix("mscx"), MscIoMode::Dir);
    EXPECT_EQ(mscIoModeBySuffix("mscs"), MscIoMode::XmlFile);
    for (const auto* suffix : { "melosong", "zip", "unknown" }) {
        EXPECT_FALSE(isMuseScoreFile(suffix));
        EXPECT_EQ(mscIoModeBySuffix(suffix), MscIoMode::Unknown);
    }
}

TEST(Engraving_MeloScoreIoTests, NativeArchivePreservesAllMembers)
{
    for (const auto* name : { "My score.meloscore", "Étude.v2.meloscore", "Étude.v2.meloscore~" }) {
        SCOPED_TRACE(name);
        const path_t path(String::fromUtf8(name));
        const ByteArray score("<museScore><Score><meloStaff>1</meloStaff></Score></museScore>");
        const ByteArray style("<museScore><Style/></museScore>");
        const ByteArray settings("{\"melo\":true}");
        const ByteArray binary("image-or-audio-payload");
        ByteArray archive;
        {
            Buffer buffer(&archive);
            MscWriter::Params params;
            params.device = &buffer;
            params.filePath = path;
            params.mode = mscIoModeBySuffix(suffix(path));
            MscWriter writer(params);
            ASSERT_TRUE(writer.open());
            writer.writeScoreFile(score);
            writer.writeStyleFile(style);
            writer.writeAudioSettingsJsonFile(settings);
            writer.writeViewSettingsJsonFile(settings);
            writer.writeAutomationJsonFile(settings);
            writer.writeThumbnailFile(binary);
            writer.addImageFile(u"picture.png", binary);
            writer.writeAudioFile(binary);
            writer.addExcerptFile(u"Part 1", score);
            writer.addExcerptStyleFile(u"Part 1", style);
            writer.close();
            ASSERT_FALSE(writer.hasError());
        }
        Buffer buffer(&archive);
        MscReader::Params params;
        params.device = &buffer;
        params.filePath = path;
        params.mode = mscIoModeBySuffix(suffix(path));
        MscReader reader(params);
        ASSERT_TRUE(reader.open());
        EXPECT_TRUE(reader.isContainer());
        EXPECT_EQ(reader.readScoreFile(), score);
        EXPECT_EQ(reader.readStyleFile(), style);
        EXPECT_EQ(reader.readAudioSettingsJsonFile(), settings);
        EXPECT_EQ(reader.readViewSettingsJsonFile(), settings);
        EXPECT_EQ(reader.readAutomationJsonFile(), settings);
        EXPECT_EQ(reader.readThumbnailFile(), binary);
        EXPECT_EQ(reader.readImageFile(u"picture.png"), binary);
        EXPECT_EQ(reader.readAudioFile(), binary);
        EXPECT_EQ(reader.readExcerptFile(u"Part 1"), score);
        EXPECT_EQ(reader.readExcerptStyleFile(u"Part 1"), style);
        EXPECT_EQ(containerPath(path), path);
        EXPECT_EQ(mainFilePath(path), path);
        EXPECT_EQ(suffix(mainFileName(path)), "mscx");
    }
}
