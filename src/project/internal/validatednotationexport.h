// SPDX-License-Identifier: GPL-3.0-only
// MuseScore-Studio-CLA-applies
#pragma once

#include <functional>
#include <QSaveFile>
#include "global/io/buffer.h"
#include "global/io/path.h"
#include "global/types/ret.h"

namespace mu::project {
// Notation writers can refuse incompatible musical state. Validate in memory,
// then atomically replace the destination; a refusal must preserve an old export.
inline muse::Ret writeValidatedNotationExport(const muse::io::path_t& path,
                                              const std::function<muse::Ret(muse::io::IODevice&)>& writer)
{
    muse::io::Buffer buffer;
    buffer.open(muse::io::IODevice::ReadWrite);
    buffer.setMeta("file_path", path.toStdString());
    muse::Ret ret = writer(buffer);
    if (!ret) {
        return ret;
    }
    if (buffer.hasError()) {
        return muse::Ret(int(muse::Ret::Code::UnknownError), buffer.errorString());
    }
    QSaveFile file(path.toQString());
    file.setDirectWriteFallback(false);
    if (!file.open(QIODevice::WriteOnly)
        || file.write(reinterpret_cast<const char*>(buffer.data().constData()), qint64(buffer.data().size())) != qint64(
            buffer.data().size())
        || !file.commit()) {
        return muse::Ret(int(muse::Ret::Code::UnknownError), file.errorString().toStdString());
    }
    return muse::make_ok();
}
}
