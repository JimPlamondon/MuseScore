// SPDX-License-Identifier: GPL-3.0-only
// MuseScore-Studio-CLA-applies
#include <gtest/gtest.h>
#include <QFile>
#include <QTemporaryDir>
#include "project/internal/validatednotationexport.h"
using namespace muse;
using namespace mu::project;

TEST(ValidatedNotationExport, RefusalPreservesExistingDestinationAndDoesNotCreateANewOne)
{
    QTemporaryDir directory;
    ASSERT_TRUE(directory.isValid());
    const QString path = directory.filePath("score.mxl");
    auto refuse = [](io::IODevice& output) {
        output.write(QByteArray("partial archive"));
        return Ret(int(Ret::Code::UnknownError), "incompatible JiMS state");
    };
    EXPECT_EQ(writeValidatedNotationExport(path, refuse).text(), "incompatible JiMS state");
    EXPECT_FALSE(QFile::exists(path));
    QFile file(path);
    ASSERT_TRUE(file.open(QIODevice::WriteOnly));
    file.write("previous valid export");
    file.close();
    EXPECT_FALSE(writeValidatedNotationExport(path, refuse));
    ASSERT_TRUE(file.open(QIODevice::ReadOnly));
    EXPECT_EQ(file.readAll(), "previous valid export");
}

TEST(ValidatedNotationExport, SuccessReplacesDestinationWithCompleteOutput)
{
    QTemporaryDir directory;
    ASSERT_TRUE(directory.isValid());
    const QString path = directory.filePath("score.musicxml");
    QFile file(path);
    ASSERT_TRUE(file.open(QIODevice::WriteOnly));
    file.write("old export with more bytes");
    file.close();
    ASSERT_TRUE(writeValidatedNotationExport(path, [&path](io::IODevice& output) {
        EXPECT_EQ(output.meta("file_path"), path.toStdString());
        output.write(QByteArray("complete export"));
        return make_ok();
    }));
    ASSERT_TRUE(file.open(QIODevice::ReadOnly));
    EXPECT_EQ(file.readAll(), "complete export");
}
