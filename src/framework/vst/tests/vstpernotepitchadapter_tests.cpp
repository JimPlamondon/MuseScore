/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 *
 * MuseScore
 * Music Composition & Notation
 *
 * Copyright (C) 2026 MuseScore Limited and others
 */

#include <gtest/gtest.h>

#include "internal/vstpernotepitchadapter.h"

using namespace muse::vst;

namespace {
VstEvent note(VstEvent::EventTypes type, int32_t noteId, int32_t pitch, float tuning)
{
    VstEvent event {};
    event.type = type;
    if (type == VstEvent::kNoteOnEvent) {
        event.noteOn.noteId = noteId;
        event.noteOn.pitch = pitch;
        event.noteOn.tuning = tuning;
        event.noteOn.channel = 0;
    } else {
        event.noteOff.noteId = noteId;
        event.noteOff.pitch = pitch;
        event.noteOff.tuning = tuning;
        event.noteOff.channel = 0;
    }
    return event;
}
}

TEST(VstPerNotePitchAdapterTests, StandardTuningExpressionReplacesRatherThanDoublesNoteOnTuning)
{
    VstPerNotePitchConfig config;
    config.tuningExpression = true;
    VstPerNotePitchAdapter adapter;
    adapter.configure(config);

    const VstPerNotePitchOutput on = adapter.adapt(note(VstEvent::kNoteOnEvent, 42, 60, 28.f));
    ASSERT_EQ(on.eventCount, 2u);
    EXPECT_FLOAT_EQ(on.events[0].noteOn.tuning, 0.f);
    EXPECT_EQ(on.events[1].type, VstEvent::kNoteExpressionValueEvent);
    EXPECT_EQ(on.events[1].noteExpressionValue.noteId, 42);
    EXPECT_EQ(on.events[1].noteExpressionValue.typeId, Steinberg::Vst::kTuningTypeID);
    EXPECT_NEAR(on.events[1].noteExpressionValue.value, 0.5 + 28.0 / 24000.0, 1e-12);

    const VstPerNotePitchOutput off = adapter.adapt(note(VstEvent::kNoteOffEvent, 42, 60, 28.f));
    EXPECT_FLOAT_EQ(off.events[0].noteOff.tuning, 0.f);
}

TEST(VstPerNotePitchAdapterTests, ExplicitMpeModeAllocatesIndependentChannelsWithoutAnAmbiguousSameSampleReset)
{
    VstPerNotePitchConfig config;
    config.mpePitchBendRangeSemitones = 12.0;
    config.pitchBendParams[1] = 1001;
    config.pitchBendParams[2] = 1002;
    VstPerNotePitchAdapter adapter;
    adapter.configure(config);

    const VstPerNotePitchOutput first = adapter.adapt(note(VstEvent::kNoteOnEvent, 10, 60, 28.f));
    const VstPerNotePitchOutput second = adapter.adapt(note(VstEvent::kNoteOnEvent, 11, 64, -28.f));
    EXPECT_EQ(first.events[0].noteOn.channel, 1);
    EXPECT_EQ(second.events[0].noteOn.channel, 2);
    EXPECT_EQ(first.params[0].paramId, 1001u);
    EXPECT_EQ(second.params[0].paramId, 1002u);
    EXPECT_NEAR(first.params[0].value, 0.5 + 28.0 / 2400.0, 1e-12);
    EXPECT_NEAR(second.params[0].value, 0.5 - 28.0 / 2400.0, 1e-12);
    EXPECT_TRUE(first.paramsBeforeEvents);

    const VstPerNotePitchOutput firstOff = adapter.adapt(note(VstEvent::kNoteOffEvent, 10, 60, 28.f));
    EXPECT_EQ(firstOff.events[0].noteOff.channel, 1);
    EXPECT_EQ(firstOff.paramCount, 0u);

    const VstPerNotePitchOutput replacement = adapter.adapt(note(VstEvent::kNoteOnEvent, 12, 67, 14.f));
    EXPECT_EQ(replacement.events[0].noteOn.channel, 1);
    EXPECT_EQ(replacement.params[0].paramId, 1001u);
    EXPECT_NEAR(replacement.params[0].value, 0.5 + 14.0 / 2400.0, 1e-12);
}

TEST(VstPerNotePitchAdapterTests, StandardTuningTakesPriorityOverConfiguredMpeFallback)
{
    VstPerNotePitchConfig config;
    config.tuningExpression = true;
    config.mpePitchBendRangeSemitones = 48.0;
    config.pitchBendParams[1] = 1001;
    VstPerNotePitchAdapter adapter;
    adapter.configure(config);

    const VstPerNotePitchOutput on = adapter.adapt(note(VstEvent::kNoteOnEvent, 7, 67, 14.f));
    EXPECT_EQ(on.eventCount, 2u);
    EXPECT_EQ(on.paramCount, 0u);
    EXPECT_EQ(on.events[0].noteOn.channel, 0);
}
