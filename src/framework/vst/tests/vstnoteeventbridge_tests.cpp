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

#include <gtest/gtest.h>

#include "vst/internal/vstnoteeventbridge.h"

using namespace muse;
using namespace muse::audio;
using namespace muse::vst;

namespace {
VstEvent noteOn(int32_t noteId, int16_t pitch, float tuning, float velocity)
{
    VstEvent event {};
    event.busIndex = 0;
    event.type = VstEvent::kNoteOnEvent;
    event.noteOn.noteId = noteId;
    event.noteOn.channel = 0;
    event.noteOn.pitch = pitch;
    event.noteOn.tuning = tuning;
    event.noteOn.velocity = velocity;
    return event;
}

VstEvent noteOff(int32_t noteId, int16_t pitch, float tuning)
{
    VstEvent event {};
    event.busIndex = 0;
    event.type = VstEvent::kNoteOffEvent;
    event.noteOff.noteId = noteId;
    event.noteOff.channel = 0;
    event.noteOff.pitch = pitch;
    event.noteOff.tuning = tuning;
    return event;
}
}

TEST(VstNoteEventBridgeTests, PreservesChordAndOverlappingUnisonIdentities)
{
    VstNoteEventBridge bridge;

    bridge.capture(noteOn(100, 60, 12.5f, 0.5f), 0);
    bridge.capture(noteOn(101, 64, -7.25f, 0.6f), 0);
    bridge.capture(noteOn(102, 60, 12.5f, 0.7f), 37);
    bridge.capture(noteOff(100, 60, 12.5f), 128);
    bridge.capture(noteOff(102, 60, 12.5f), 191);
    bridge.capture(noteOff(101, 64, -7.25f), 255);

    const AudioNoteEvents& events = bridge.events();
    ASSERT_EQ(events.size(), 6u);

    EXPECT_EQ(events[0].type, AudioNoteEvent::Type::NoteOn);
    EXPECT_EQ(events[0].noteId, 100);
    EXPECT_EQ(events[0].pitch, 60);
    EXPECT_FLOAT_EQ(events[0].tuningCents, 12.5f);
    EXPECT_FLOAT_EQ(events[0].velocity, 0.5f);
    EXPECT_EQ(events[0].sampleOffset, 0u);

    EXPECT_EQ(events[1].noteId, 101);
    EXPECT_EQ(events[1].pitch, 64);
    EXPECT_EQ(events[2].noteId, 102);
    EXPECT_EQ(events[2].pitch, 60);
    EXPECT_EQ(events[2].sampleOffset, 37u);

    EXPECT_EQ(events[3].type, AudioNoteEvent::Type::NoteOff);
    EXPECT_EQ(events[3].noteId, 100);
    EXPECT_EQ(events[4].noteId, 102);
    EXPECT_EQ(events[5].noteId, 101);
}

TEST(VstNoteEventBridgeTests, UsesOneMappingInBothDirections)
{
    AudioNoteEvent source {
        AudioNoteEvent::Type::NoteOn,
        73,
        404,
        69,
        -31.75f,
        0.875f,
    };

    const VstEvent vstEvent = VstNoteEventBridge::toVstEvent(source);
    EXPECT_EQ(vstEvent.type, VstEvent::kNoteOnEvent);
    EXPECT_EQ(vstEvent.sampleOffset, 73);
    EXPECT_EQ(vstEvent.noteOn.noteId, 404);
    EXPECT_EQ(vstEvent.noteOn.pitch, 69);
    EXPECT_FLOAT_EQ(vstEvent.noteOn.tuning, -31.75f);
    EXPECT_FLOAT_EQ(vstEvent.noteOn.velocity, 0.875f);

    VstNoteEventBridge bridge;
    bridge.capture(vstEvent, 0);
    ASSERT_EQ(bridge.events().size(), 1u);
    const AudioNoteEvent& roundTrip = bridge.events().front();
    EXPECT_EQ(roundTrip.type, source.type);
    EXPECT_EQ(roundTrip.sampleOffset, source.sampleOffset);
    EXPECT_EQ(roundTrip.noteId, source.noteId);
    EXPECT_EQ(roundTrip.pitch, source.pitch);
    EXPECT_FLOAT_EQ(roundTrip.tuningCents, source.tuningCents);
    EXPECT_FLOAT_EQ(roundTrip.velocity, source.velocity);
}

TEST(VstNoteEventBridgeTests, IgnoresPerNoteExpressionEvents)
{
    VstEvent expression {};
    expression.type = VstEvent::kNoteExpressionValueEvent;
    expression.noteExpressionValue.noteId = 42;
    expression.noteExpressionValue.typeId = Steinberg::Vst::kTuningTypeID;
    expression.noteExpressionValue.value = 0.75;

    VstNoteEventBridge bridge;
    bridge.capture(expression, 12);

    EXPECT_TRUE(bridge.events().empty());
}
