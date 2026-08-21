/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 *
 * MuseScore
 * Music Composition & Notation
 *
 * Copyright (C) 2026 MuseScore Limited and others
 */
#pragma once

#include <array>
#include <cstddef>

#include "vsttypes.h"

namespace muse::vst {
struct VstPerNotePitchConfig {
    bool tuningExpression = false;
    double mpePitchBendRangeSemitones = 0.0;
    std::array<PluginParamId, 16> pitchBendParams {};

    VstPerNotePitchConfig();
};

struct VstPerNotePitchOutput {
    std::array<VstEvent, 2> events {};
    std::array<ParamChangeEvent, 1> params {};
    size_t eventCount = 0;
    size_t paramCount = 0;
    bool paramsBeforeEvents = false;
};

//! Converts the canonical VST3 note sent by MuseScore into a pitch path the
//! selected unmodified instrument explicitly supports. The caller retains
//! the canonical event for downstream effects before applying this adapter.
class VstPerNotePitchAdapter
{
public:
    void configure(const VstPerNotePitchConfig& config);
    void reset();
    VstPerNotePitchOutput adapt(const VstEvent& event);

private:
    struct ActiveNote {
        int32_t noteId = -1;
        enum class Mode {
            TuningExpression, Mpe
        } mode = Mode::TuningExpression;
        int16_t channel = 0;
    };

    int16_t allocateMpeChannel() const;
    ActiveNote* findActiveNote(int32_t noteId);
    void rememberNote(int32_t noteId, ActiveNote::Mode mode, int16_t channel);

    VstPerNotePitchConfig m_config;
    std::array<ActiveNote, 128> m_activeNotes {};
};
}
