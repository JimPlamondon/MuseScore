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

#include "musicxmlwriter.h"

#include "engraving/dom/score.h"
#include "export/exportmusicxml.h"

#include "log.h"
#include "translation.h"

using namespace mu::iex::musicxml;
using namespace mu::project;
using namespace muse;
using namespace muse::io;

std::vector<INotationWriter::UnitType> MusicXmlWriter::supportedUnitTypes() const
{
    return { UnitType::PER_PART };
}

bool MusicXmlWriter::supportsUnitType(UnitType unitType) const
{
    std::vector<UnitType> unitTypes = supportedUnitTypes();
    return std::find(unitTypes.cbegin(), unitTypes.cend(), unitType) != unitTypes.cend();
}

Ret MusicXmlWriter::write(notation::INotationPtr notation, io::IODevice& destinationDevice, const Options&)
{
    IF_ASSERT_FAILED(notation) {
        return make_ret(Ret::Code::UnknownError);
    }
    mu::engraving::Score* score = notation->elements()->msScore();
    IF_ASSERT_FAILED(score) {
        return make_ret(Ret::Code::UnknownError);
    }

    String error;
    if (!saveXml(score, &destinationDevice, &error)) {
        return Ret(int(Ret::Code::UnknownError), error.empty()
                   ? muse::trc("iex_musicxml", "The score could not be exported. Save a native MuseScore copy to preserve your work.")
                   : error.toStdString());
    }
    return muse::make_ok();
}

Ret MusicXmlWriter::writeList(const notation::INotationPtrList&, io::IODevice&, const Options&)
{
    NOT_SUPPORTED;
    return Ret(Ret::Code::NotSupported);
}
