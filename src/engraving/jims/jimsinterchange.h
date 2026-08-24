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

#pragma once

// JiMS MusicXML extension (urn:jims:musicxml:3) interchange carriers that the
// fork TRANSPORTS but does not interpret (owner decision 2026-08-19, item 5 of
// the Add-JiMS-to-MuseScore follow-ups): jims:provenance (document
// identification) and jims:tuning-trajectory (a printed continuous-tuning
// instruction inside a MusicXML direction). Both are read on import, saved
// in the score file, and written back on export exactly as carried — the
// fork computes no musical fact from them (no rendering, no playback; a
// printed trajectory's staff geometry is a later milestone with the Kernel).
// Values stay in the extension's own units: cents, normalized [0,1] control
// times, score time as MuseScore fractions (MusicXML divisions are a
// per-document encoding, rescaled on export).

#include <vector>

#include "global/types/string.h"

#include "../types/fraction.h"
#include "../types/types.h"

namespace mu::engraving::jims {
/// Explicit score-wide SATB melody designation. Soprano is the native and
/// MusicXML default; no value is inferred from part order or note content.
enum class MelodyPart : unsigned char {
    Soprano,
    Alto,
    Tenor,
    Bass,
};

inline muse::String melodyPartToken(MelodyPart part)
{
    switch (part) {
    case MelodyPart::Soprano: return u"soprano";
    case MelodyPart::Alto: return u"alto";
    case MelodyPart::Tenor: return u"tenor";
    case MelodyPart::Bass: return u"bass";
    }
    return u"soprano";
}

inline bool melodyPartFromToken(const muse::String& token, MelodyPart& part)
{
    if (token == u"soprano") {
        part = MelodyPart::Soprano;
        return true;
    }
    if (token == u"alto") {
        part = MelodyPart::Alto;
        return true;
    }
    if (token == u"tenor") {
        part = MelodyPart::Tenor;
        return true;
    }
    if (token == u"bass") {
        part = MelodyPart::Bass;
        return true;
    }
    return false;
}

/// One `jims:resource` of `jims:provenance`.
struct ProvenanceResource {
    muse::String role;        // source | master | arrangement
    muse::String uri;
    muse::String mediaType;   // media-type
    muse::String sha256;      // 64 hex chars or empty
    muse::String text;        // element text or empty

    bool operator==(const ProvenanceResource& o) const
    {
        return role == o.role && uri == o.uri && mediaType == o.mediaType && sha256 == o.sha256 && text == o.text;
    }
};

/// `jims:provenance` (in `identification`, before `miscellaneous`).
struct Provenance {
    bool strictFallback = false;               // fallback-profile="strict"
    std::vector<ProvenanceResource> resources;

    bool empty() const { return resources.empty() && !strictFallback; }
    bool operator==(const Provenance& o) const { return strictFallback == o.strictFallback && resources == o.resources; }
    bool operator!=(const Provenance& o) const { return !(*this == o); }
};

/// One `jims:control` of a cubic-bezier segment. Numeric values are carried
/// as the exact text read (the fork never computes with them, and a
/// re-formatted double would not be verbatim transport).
struct TrajectoryControl {
    muse::String time;         // normalized [0,1], as written
    muse::String valueCents;   // as written

    bool operator==(const TrajectoryControl& o) const { return time == o.time && valueCents == o.valueCents; }
};

/// One `jims:segment` of a tuning trajectory.
struct TrajectorySegment {
    Fraction duration;                       // duration-divisions, as score time (rescaled to the export's divisions)
    muse::String startCents;                 // as written
    muse::String endCents;                   // as written
    muse::String interpolation;              // linear | cubic-bezier
    std::vector<TrajectoryControl> controls; // exactly two for cubic-bezier

    bool operator==(const TrajectorySegment& o) const
    {
        return duration == o.duration && startCents == o.startCents && endCents == o.endCents
               && interpolation == o.interpolation && controls == o.controls;
    }
};

/// `jims:tuning-trajectory`: the direction it rode in gives its score time
/// (measure position + offset), staff, and placement.
struct TuningTrajectory {
    Fraction tick;                           // absolute score tick of the trajectory start
    muse::String placement;                  // direction placement attribute or empty
    std::vector<TrajectorySegment> segments;

    bool operator==(const TuningTrajectory& o) const
    {
        return tick == o.tick && placement == o.placement && segments == o.segments;
    }
};
}
