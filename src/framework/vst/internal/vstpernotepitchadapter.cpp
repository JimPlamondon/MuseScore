/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 *
 * MuseScore
 * Music Composition & Notation
 *
 * Copyright (C) 2026 MuseScore Limited and others
 */

#include "vstpernotepitchadapter.h"

#include <algorithm>
#include <cmath>

using namespace muse::vst;

VstPerNotePitchConfig::VstPerNotePitchConfig()
{
    pitchBendParams.fill(Steinberg::Vst::kNoParamId);
}

void VstPerNotePitchAdapter::configure(const VstPerNotePitchConfig& config)
{
    m_config = config;
    reset();
}

void VstPerNotePitchAdapter::reset()
{
    for (ActiveNote& note : m_activeNotes) {
        note.noteId = -1;
    }
}

int16_t VstPerNotePitchAdapter::allocateMpeChannel() const
{
    for (int16_t channel = 1; channel < 16; ++channel) {
        if (m_config.pitchBendParams[channel] == Steinberg::Vst::kNoParamId) {
            continue;
        }
        bool inUse = false;
        for (const ActiveNote& note : m_activeNotes) {
            if (note.noteId >= 0 && note.mode == ActiveNote::Mode::Mpe && note.channel == channel) {
                inUse = true;
                break;
            }
        }
        if (!inUse) {
            return channel;
        }
    }
    return -1;
}

VstPerNotePitchAdapter::ActiveNote* VstPerNotePitchAdapter::findActiveNote(const int32_t noteId)
{
    const auto note = std::find_if(m_activeNotes.begin(), m_activeNotes.end(), [noteId](const ActiveNote& active) {
        return active.noteId == noteId;
    });
    return note == m_activeNotes.end() ? nullptr : &*note;
}

void VstPerNotePitchAdapter::rememberNote(const int32_t noteId, const ActiveNote::Mode mode, const int16_t channel)
{
    ActiveNote* slot = findActiveNote(noteId);
    if (!slot) {
        const auto free = std::find_if(m_activeNotes.begin(), m_activeNotes.end(), [](const ActiveNote& active) {
            return active.noteId < 0;
        });
        if (free == m_activeNotes.end()) {
            return;
        }
        slot = &*free;
    }
    *slot = ActiveNote { noteId, mode, channel };
}

VstPerNotePitchOutput VstPerNotePitchAdapter::adapt(const VstEvent& event)
{
    VstPerNotePitchOutput output;
    output.events[0] = event;
    output.eventCount = 1;

    if (event.type == VstEvent::kNoteOnEvent && event.noteOn.noteId >= 0) {
        if (m_config.tuningExpression) {
            output.events[0].noteOn.tuning = 0.f;
            VstEvent expression {};
            expression.busIndex = event.busIndex;
            expression.sampleOffset = event.sampleOffset;
            expression.ppqPosition = event.ppqPosition;
            expression.flags = event.flags;
            expression.type = VstEvent::kNoteExpressionValueEvent;
            expression.noteExpressionValue.typeId = Steinberg::Vst::kTuningTypeID;
            expression.noteExpressionValue.noteId = event.noteOn.noteId;
            expression.noteExpressionValue.value = std::clamp(0.5 + event.noteOn.tuning / 24000.0, 0.0, 1.0);
            output.events[1] = expression;
            output.eventCount = 2;
            rememberNote(event.noteOn.noteId, ActiveNote::Mode::TuningExpression, event.noteOn.channel);
            return output;
        }

        const double rangeCents = m_config.mpePitchBendRangeSemitones * 100.0;
        if (rangeCents > 0.0 && std::abs(event.noteOn.tuning) <= rangeCents) {
            const int16_t channel = allocateMpeChannel();
            if (channel >= 0) {
                output.events[0].noteOn.channel = channel;
                output.events[0].noteOn.tuning = 0.f;
                output.params[0] = ParamChangeEvent {
                    m_config.pitchBendParams[channel],
                    std::clamp(0.5 + event.noteOn.tuning / (2.0 * rangeCents), 0.0, 1.0)
                };
                output.paramCount = 1;
                output.paramsBeforeEvents = true;
                rememberNote(event.noteOn.noteId, ActiveNote::Mode::Mpe, channel);
            }
        }
        return output;
    }

    if (event.type != VstEvent::kNoteOffEvent || event.noteOff.noteId < 0) {
        return output;
    }

    ActiveNote* active = findActiveNote(event.noteOff.noteId);
    if (!active) {
        return output;
    }
    if (active->mode == ActiveNote::Mode::TuningExpression) {
        output.events[0].noteOff.tuning = 0.f;
    } else {
        const int16_t channel = active->channel;
        output.events[0].noteOff.channel = channel;
        output.events[0].noteOff.tuning = 0.f;
    }
    active->noteId = -1;
    return output;
}
