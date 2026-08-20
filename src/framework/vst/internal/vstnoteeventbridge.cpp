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

#include "vstnoteeventbridge.h"

#include <algorithm>
#include <limits>

using namespace muse;
using namespace muse::audio;
using namespace muse::vst;

namespace {
constexpr size_t NOTE_EVENT_CAPACITY = 4096;

samples_t absoluteSampleOffset(const VstEvent& event, samples_t sequenceSampleOffset)
{
    const samples_t eventOffset = static_cast<samples_t>(std::max(event.sampleOffset, Steinberg::int32(0)));
    if (sequenceSampleOffset > std::numeric_limits<samples_t>::max() - eventOffset) {
        return std::numeric_limits<samples_t>::max();
    }
    return sequenceSampleOffset + eventOffset;
}
}

VstNoteEventBridge::VstNoteEventBridge()
{
    m_events.reserve(NOTE_EVENT_CAPACITY);
}

void VstNoteEventBridge::clear()
{
    m_events.clear();
}

void VstNoteEventBridge::capture(const VstEvent& event, samples_t sequenceSampleOffset)
{
    if (m_events.size() >= m_events.capacity()) {
        return;
    }

    AudioNoteEvent translated;
    translated.sampleOffset = absoluteSampleOffset(event, sequenceSampleOffset);

    if (event.type == VstEvent::kNoteOnEvent) {
        translated.type = AudioNoteEvent::Type::NoteOn;
        translated.noteId = event.noteOn.noteId;
        translated.pitch = event.noteOn.pitch;
        translated.tuningCents = event.noteOn.tuning;
        translated.velocity = event.noteOn.velocity;
    } else if (event.type == VstEvent::kNoteOffEvent) {
        translated.type = AudioNoteEvent::Type::NoteOff;
        translated.noteId = event.noteOff.noteId;
        translated.pitch = event.noteOff.pitch;
        translated.tuningCents = event.noteOff.tuning;
        translated.velocity = event.noteOff.velocity;
    } else {
        return;
    }

    m_events.push_back(translated);
}

const AudioNoteEvents& VstNoteEventBridge::events() const
{
    return m_events;
}

VstEvent VstNoteEventBridge::toVstEvent(const AudioNoteEvent& source)
{
    VstEvent event {};
    event.busIndex = 0;
    event.sampleOffset = static_cast<Steinberg::int32>(std::min(
                                                           source.sampleOffset,
                                                           static_cast<samples_t>(std::numeric_limits<Steinberg::int32>::max())));
    event.ppqPosition = 0;
    event.flags = 0;

    if (source.type == AudioNoteEvent::Type::NoteOn) {
        event.type = VstEvent::kNoteOnEvent;
        event.noteOn.channel = 0;
        event.noteOn.pitch = source.pitch;
        event.noteOn.tuning = source.tuningCents;
        event.noteOn.velocity = source.velocity;
        event.noteOn.length = 0;
        event.noteOn.noteId = source.noteId;
    } else {
        event.type = VstEvent::kNoteOffEvent;
        event.noteOff.channel = 0;
        event.noteOff.pitch = source.pitch;
        event.noteOff.tuning = source.tuningCents;
        event.noteOff.velocity = source.velocity;
        event.noteOff.noteId = source.noteId;
    }

    return event;
}
