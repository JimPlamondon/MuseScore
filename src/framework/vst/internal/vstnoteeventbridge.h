/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 *
 * MuseScore
 * Music Composition & Notation
 *
 * Copyright (C) 2026 MuseScore Limited and others
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

#pragma once

#include "audio/common/audiotypes.h"

#include "vsttypes.h"

namespace muse::vst {
//! Lossless note-on/off translation at the audio-source/VST3-effect seam.
//! A VST3 instrument source captures the same events it sends to the
//! instrument; downstream VST3 effects consume the shared AudioNoteEvent
//! representation. Per-note expressions deliberately remain instrument-only.
class VstNoteEventBridge
{
public:
    VstNoteEventBridge();

    void clear();
    void capture(const VstEvent& event, audio::samples_t sequenceSampleOffset);

    const audio::AudioNoteEvents& events() const;

    static VstEvent toVstEvent(const audio::AudioNoteEvent& event);

private:
    audio::AudioNoteEvents m_events;
};
}
