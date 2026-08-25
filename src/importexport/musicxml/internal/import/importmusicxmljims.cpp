/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 *
 * MuseScore Studio
 * Music Composition & Notation
 *
 * Copyright (C) 2026 Jim Plamondon
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
#include "importmusicxmljims.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "engraving/dom/factory.h"
#include "engraving/dom/measure.h"
#include "engraving/dom/part.h"
#include "engraving/dom/score.h"
#include "engraving/dom/segment.h"
#include "engraving/dom/staff.h"
#include "engraving/dom/stafftype.h"
#include "engraving/dom/stafftypechange.h"
#include "engraving/jims/jimsbridge.h"
#include "engraving/style/style.h"

#include "importmusicxmllogger.h"

#include "log.h"

using namespace muse;
using namespace mu::engraving;

namespace mu::iex::musicxml {
static const char* JIMS_URI_STEM = "urn:jims:musicxml:";

// Fatal JiMS import conditions go to the MusicXML logger (the import
// dialog) AND the console log, so a refused import is never silent.
static void jimsFatal(MusicXmlLogger* logger, const String& text, const XmlStreamReader* e = nullptr)
{
    LOGE() << "JiMS MusicXML import: " << text;
    if (logger) {
        logger->logError(text, e);
    }
}

//---------------------------------------------------------
//   resolveFromRoot
//---------------------------------------------------------

Err JimsImportContext::resolveFromRoot(const std::vector<XmlStreamReader::Attribute>& attributes,
                                       MusicXmlLogger* logger, const XmlStreamReader* e)
{
    for (const XmlStreamReader::Attribute& a : attributes) {
        const String name = String::fromAscii(a.name.ascii());
        if (!a.value.startsWith(String::fromAscii(JIMS_URI_STEM))) {
            continue;
        }
        const String versionText = a.value.mid(String::fromAscii(JIMS_URI_STEM).size());
        bool ok = false;
        const int version = versionText.toInt(&ok);
        if (!ok || version < MIN_VERSION || version > MAX_VERSION) {
            jimsFatal(logger,
                      String(
                          u"unsupported JiMS MusicXML namespace '%1' (this MuseScore understands urn:jims:musicxml:%2..%3); import refused so the document is not silently shown as a plain staff")
                      .arg(a.value).arg(MIN_VERSION).arg(MAX_VERSION), e);
            return Err::FileBadFormat;
        }
        if (name == u"xmlns") {
            jimsFatal(logger, String(u"the JiMS namespace '%1' must be bound to a prefix, not used as the default namespace").arg(
                          a.value), e);
            return Err::FileBadFormat;
        }
        if (!name.startsWith(u"xmlns:")) {
            continue;             // some other attribute happens to hold a JiMS URI
        }
        const String prefix = name.mid(6);
        if (prefix.empty()) {
            continue;
        }
        if (hasJims()) {
            if (version != m_version) {
                jimsFatal(logger, String(u"two distinct JiMS profiles declared in one document (urn:jims:musicxml:%1 and %2)")
                          .arg(m_version).arg(version), e);
                return Err::FileBadFormat;
            }
            logger->logDebugInfo(String(u"JiMS namespace bound twice; keeping prefix '%1'").arg(m_prefix), e);
            continue;
        }
        m_prefix = prefix;
        m_version = version;
    }
    return Err::NoError;
}

//---------------------------------------------------------
//   isJimsElement
//---------------------------------------------------------

bool JimsImportContext::isJimsElement(const AsciiStringView& qualifiedName, const char* local) const
{
    if (!hasJims()) {
        return false;
    }
    const String name = String::fromAscii(qualifiedName.ascii());
    return name == m_prefix + u":" + String::fromAscii(local);
}

//---------------------------------------------------------
//   jsonNumber
//---------------------------------------------------------

String JimsImportContext::jsonNumber(const String& text, bool& ok)
{
    const std::string s = text.trimmed().toStdString();
    char* end = nullptr;
    const double v = std::strtod(s.c_str(), &end);
    ok = !s.empty() && end && *end == '\0' && std::isfinite(v);
    if (!ok) {
        return String();
    }
    char buf[64];
    if (v == std::floor(v) && std::fabs(v) < 1e15) {
        std::snprintf(buf, sizeof buf, "%.0f.0", v);
        return String::fromAscii(buf);
    }
    for (int precision = 1; precision <= 17; ++precision) {
        std::snprintf(buf, sizeof buf, "%.*g", precision, v);
        if (std::strtod(buf, nullptr) == v) {
            break;
        }
    }
    String out = String::fromAscii(buf);
    if (!out.contains(u'.') && !out.contains(u'e')) {
        out += u".0";
    }
    return out;
}

//---------------------------------------------------------
//   parseStaffState
//---------------------------------------------------------

bool JimsImportContext::parseStaffState(XmlStreamReader& e, String& json, int& staffNumber, String& error) const
{
    // The whole element is always consumed, whatever goes wrong inside it,
    // so the caller's reader stays aligned; the first problem is reported.
    staffNumber = e.hasAttribute("number") ? e.intAttribute("number") : 0;
    const String p = m_prefix + u":";
    auto local = [&](const AsciiStringView& qname) -> String {
        const String name = String::fromAscii(qname.ascii());
        return name.startsWith(p) ? name.mid(p.size()) : String();
    };
    auto fail = [&](const String& text) {
        if (error.empty()) {
            error = text;
        }
    };
    auto integer = [&](const String& text, const char* field, int& out) -> bool {
        bool ok = false;
        out = text.trimmed().toInt(&ok);
        if (!ok) {
            fail(String(u"jims:staff-state %1 is not an integer: '%2'").arg(String::fromAscii(field), text));
        }
        return ok;
    };

    std::vector<String> steps;
    bool haveScale = false, haveColl = false, haveMode = false, haveGen = false, havePer = false, haveEmb = false, haveExt = false;
    int collectionRotation = 0, modeRotation = 0, largeSteps = 0, smallSteps = 0;
    int lowerNPer = 0, lowerNGen = 0, upperNPer = 0, upperNGen = 0;
    String generatorCents, periodCents, tonicAmbit, reference;

    while (e.readNextStartElement()) {
        const String tag = local(e.name());
        if (tag == u"scale") {
            haveScale = true;
            while (e.readNextStartElement()) {
                if (local(e.name()) == u"step") {
                    steps.push_back(e.readText().trimmed());
                } else {
                    e.skipCurrentElement();
                }
            }
        } else if (tag == u"collection-rotation") {
            haveColl = integer(e.readText(), "collection-rotation", collectionRotation);
        } else if (tag == u"mode-rotation") {
            haveMode = integer(e.readText(), "mode-rotation", modeRotation);
        } else if (tag == u"generator-cents") {
            generatorCents = jsonNumber(e.readText(), haveGen);
            if (!haveGen) {
                fail(u"jims:staff-state generator-cents is not a number");
            }
        } else if (tag == u"period-cents") {
            periodCents = jsonNumber(e.readText(), havePer);
            if (!havePer) {
                fail(u"jims:staff-state period-cents is not a number");
            }
        } else if (tag == u"embedding") {
            haveEmb = integer(e.attribute("large-steps"), "embedding/large-steps", largeSteps)
                      && integer(e.attribute("small-steps"), "embedding/small-steps", smallSteps);
            e.skipCurrentElement();
        } else if (tag == u"extent") {
            haveExt = integer(e.attribute("lower-n-per"), "extent/lower-n-per", lowerNPer)
                      && integer(e.attribute("lower-n-gen"), "extent/lower-n-gen", lowerNGen)
                      && integer(e.attribute("upper-n-per"), "extent/upper-n-per", upperNPer)
                      && integer(e.attribute("upper-n-gen"), "extent/upper-n-gen", upperNGen);
            e.skipCurrentElement();
        } else if (tag == u"tonic-ambit" || tag == u"tonic-extent") {   // owner rename 2026-08-19; the legacy spelling is still read
            tonicAmbit = e.readText().trimmed();
        } else if (tag == u"reference") {
            int forms = 0;
            while (e.readNextStartElement()) {
                const String form = local(e.name());
                ++forms;
                if (form == u"none") {
                    reference = u"\"none\"";
                    e.skipCurrentElement();
                } else if (form == u"reference-pitch") {
                    int keyNumber = 0;
                    if (integer(e.attribute("key-number"), "reference-pitch/key-number", keyNumber)) {
                        reference = String(u"{\"reference-pitch\":{\"key_number\":%1}}").arg(keyNumber);
                    }
                    e.skipCurrentElement();
                } else if (form == u"pitch-class") {
                    int pitchClass = 0;
                    if (integer(e.readText(), "pitch-class", pitchClass)) {
                        reference = String(u"{\"pitch-class\":{\"pitch_class\":%1}}").arg(pitchClass);
                    }
                } else if (form == u"frequency-hz") {
                    bool ok = false;
                    const String hertz = jsonNumber(e.readText(), ok);
                    if (ok) {
                        reference = String(u"{\"frequency-hz\":{\"hertz\":%1}}").arg(hertz);
                    } else {
                        fail(u"jims:reference frequency-hz is not a number");
                    }
                } else {
                    fail(String(u"unknown jims:reference form '%1'").arg(form));
                    e.skipCurrentElement();
                }
            }
            if (forms != 1) {
                fail(String(u"jims:reference must carry exactly one form, found %1").arg(forms));
            }
        } else {
            // Unknown JiMS-namespaced child: skip, never abort (Binding Requirement 3).
            e.skipCurrentElement();
        }
    }

    if (!(haveScale && haveColl && haveMode && haveGen && havePer && haveEmb && haveExt)) {
        fail(
            u"jims:staff-state is missing a required child (scale, collection-rotation, mode-rotation, generator-cents, period-cents, embedding, extent)");
    }
    if (steps.empty()) {
        fail(u"jims:staff-state scale carries no steps");
    }
    if (!error.empty()) {
        return false;
    }
    if (reference.empty()) {
        reference = u"\"none\"";        // V1/V2 profiles: an absent reference means none
    }

    String scale = u"[";
    for (size_t i = 0; i < steps.size(); ++i) {
        if (i) {
            scale += u",";
        }
        scale += u"\"" + steps[i] + u"\"";
    }
    scale += u"]";

    // Converter byte-shape (tools/jims/enriched_to_jims_mscx.py): fixed key
    // order, no spaces, tonic_ambit last. Built by concatenation — a "%10"
    // placeholder would be read as "%1" + "0".
    json = u"{\"scale\":" + scale
           + u",\"collection_rotation\":" + String::number(collectionRotation)
           + u",\"mode_rotation\":" + String::number(modeRotation)
           + u",\"generator_cents\":" + generatorCents
           + u",\"period_cents\":" + periodCents
           + u",\"embedding\":{\"large_steps\":" + String::number(largeSteps)
           + u",\"small_steps\":" + String::number(smallSteps) + u"}"
           + u",\"extent\":{\"lower\":{\"nPer\":" + String::number(lowerNPer)
           + u",\"nGen\":" + String::number(lowerNGen) + u"},\"upper\":{\"nPer\":" + String::number(upperNPer)
           + u",\"nGen\":" + String::number(upperNGen) + u"}}"
           + u",\"reference\":" + reference;
    if (!tonicAmbit.empty()) {
        json += String(u",\"tonic_ambit\":\"%1\"").arg(tonicAmbit);
    }
    json += u"}";
    return true;
}

//---------------------------------------------------------
//   buffer / statesFor
//---------------------------------------------------------

void JimsImportContext::buffer(const String& partId, const Fraction& tick, int staffNumber, const String& json)
{
    m_states[partId].push_back(BufferedState { tick, staffNumber, json });
}

const std::vector<JimsImportContext::BufferedState>* JimsImportContext::statesFor(const String& partId) const
{
    auto it = m_states.find(partId);
    return it == m_states.end() ? nullptr : &it->second;
}

//---------------------------------------------------------
//   applyToPart
//---------------------------------------------------------

static StaffType jimsStaffTypeFor(const String& json)
{
    // The fork's JiMS preset (jims12tet: clef/key signature/ledger lines
    // suppressed, JI lines on) with THIS state and its presentation line count.
    StaffType st = *StaffType::preset(StaffTypes::JIMS_12TET);
    st.setJiMS(true);
    st.setJimsJiLines(true);
    st.setJimsStateJson(json);
    return st;
}

bool JimsImportContext::applyToPart(Score* score, Part* part, const String& partId,
                                    const std::function<int(int)>& staffIndexForNumber, MusicXmlLogger* logger)
{
    const std::vector<BufferedState>* states = statesFor(partId);
    if (!states || states->empty()) {
        return true;
    }
    if (!jims::available()) {
        jimsFatal(logger, u"JiMS Kernel bridge unavailable; cannot import a JiMS staff");
        return false;
    }

    // Group by part-relative staff index, preserving document order.
    std::map<int, std::vector<const BufferedState*> > perStaff;
    for (const BufferedState& s : *states) {
        int idx = 0;
        if (s.staffNumber > 0) {
            idx = staffIndexForNumber(s.staffNumber);
            if (idx < 0 || idx >= int(part->nstaves())) {
                jimsFatal(logger, String(u"jims:staff-state number %1 names no staff of this part").arg(s.staffNumber));
                return false;
            }
        }
        perStaff[idx].push_back(&s);
    }

    for (const auto& entry : perStaff) {
        const int partStaff = entry.first;
        Staff* staff = part->staff(partStaff);
        const staff_idx_t staffIdx = score->staffIdx(part) + partStaff;
        bool first = true;
        Fraction lastTick(-1, 1);
        for (const BufferedState* s : entry.second) {
            String kernelError;
            if (!jims::validateState(s->json, kernelError)) {
                jimsFatal(logger, String(u"the JiMS Kernel rejected a jims:staff-state: %1").arg(kernelError));
                return false;
            }
            if (first) {
                if (!s->tick.isZero()) {
                    jimsFatal(logger, u"the first jims:staff-state of a staff must be declared in the first measure");
                    return false;
                }
                staff->setStaffType(Fraction(0, 1), jimsStaffTypeFor(s->json));
                first = false;
            } else {
                if (s->tick <= lastTick) {
                    jimsFatal(logger, u"jims:staff-state declarations must be in strictly increasing score-time order");
                    return false;
                }
                Measure* measure = score->tick2measure(s->tick);
                if (!measure || s->tick < measure->tick() || s->tick >= measure->endTick()) {
                    jimsFatal(logger, u"jims:staff-state does not sit inside a score measure");
                    return false;
                }
                const Fraction rtick = s->tick - measure->tick();
                if (!measure->canAddStaffTypeChange(staffIdx, rtick)) {
                    jimsFatal(logger, u"cannot place a staff type change for this jims:staff-state at its exact tick");
                    return false;
                }
                // File-read style construction (TRead::read for StaffTypeChange):
                // Measure::add() replaces the owned type with the staff's copy.
                StaffTypeChange* stc = Factory::createStaffTypeChange(measure);
                stc->setTrack(staffIdx * VOICES);
                stc->setParent(measure);
                stc->setRtick(rtick);
                stc->setStaffType(new StaffType(jimsStaffTypeFor(s->json)), true);
                if (rtick.isNotZero()
                    && !measure->findSegmentR(Segment::CHORD_REST_OR_TIME_TICK_TYPE, rtick)) {
                    measure->getSegmentR(SegmentType::TimeTick, rtick);
                }
                measure->add(stc);
            }
            lastTick = s->tick;
        }
    }

    // Milestone 3 engraving-font seam: a JiMS score selects the
    // Kernel-generated JiMSMusic outlines (stock glyphs fall back).
    score->style().set(Sid::musicalSymbolFont, String(u"JiMSMusic"));
    score->style().set(Sid::hideInstrumentNameIfOneInstrument, false);
    return true;
}

//---------------------------------------------------------
//   parseProvenance
//---------------------------------------------------------

bool JimsImportContext::parseProvenance(XmlStreamReader& e, engraving::jims::Provenance& out, String& error) const
{
    out = engraving::jims::Provenance();
    out.strictFallback = e.attribute("fallback-profile") == u"strict";
    while (e.readNextStartElement()) {
        if (isJimsElement(e.name(), "resource")) {
            engraving::jims::ProvenanceResource r;
            r.role = e.attribute("role");
            r.uri = e.attribute("uri");
            r.mediaType = e.attribute("media-type");
            r.sha256 = e.attribute("sha-256");
            if (r.role.isEmpty() || r.uri.isEmpty() || r.mediaType.isEmpty()) {
                error = u"jims:provenance resource is missing role, uri or media-type";
                e.skipCurrentElement();
                return false;
            }
            r.text = e.readText();
            out.resources.push_back(r);
        } else {
            error = String(u"unexpected element in jims:provenance: %1").arg(String::fromAscii(e.name().ascii()));
            e.skipCurrentElement();
            return false;
        }
    }
    return true;
}

//---------------------------------------------------------
//   parseTuningTrajectory
//---------------------------------------------------------

bool JimsImportContext::parseTuningTrajectory(XmlStreamReader& e, const std::function<engraving::Fraction(int)>& ticksOf,
                                              engraving::jims::TuningTrajectory& out, String& error) const
{
    out = engraving::jims::TuningTrajectory();
    while (e.readNextStartElement()) {
        if (!isJimsElement(e.name(), "segment")) {
            error = String(u"unexpected element in jims:tuning-trajectory: %1").arg(String::fromAscii(e.name().ascii()));
            e.skipCurrentElement();
            return false;
        }
        engraving::jims::TrajectorySegment seg;
        bool ok = false;
        const int divisions = e.attribute("duration-divisions").toInt(&ok);
        if (!ok || divisions <= 0) {
            error = u"jims:segment duration-divisions must be a positive integer";
            e.skipCurrentElement();
            return false;
        }
        seg.duration = ticksOf(divisions);
        seg.startCents = e.attribute("start-cents");
        seg.endCents = e.attribute("end-cents");
        seg.interpolation = e.attribute("interpolation");
        if (seg.startCents.isEmpty() || seg.endCents.isEmpty()
            || (seg.interpolation != u"linear" && seg.interpolation != u"cubic-bezier")) {
            error = u"jims:segment needs start-cents, end-cents and interpolation linear|cubic-bezier";
            e.skipCurrentElement();
            return false;
        }
        while (e.readNextStartElement()) {
            if (!isJimsElement(e.name(), "control")) {
                error = String(u"unexpected element in jims:segment: %1").arg(String::fromAscii(e.name().ascii()));
                e.skipCurrentElement();
                return false;
            }
            engraving::jims::TrajectoryControl c;
            c.time = e.attribute("time");
            c.valueCents = e.attribute("value-cents");
            if (c.time.isEmpty() || c.valueCents.isEmpty()) {
                error = u"jims:control needs time and value-cents";
                e.skipCurrentElement();
                return false;
            }
            seg.controls.push_back(c);
            e.skipCurrentElement();
        }
        if (seg.interpolation == u"cubic-bezier" && seg.controls.size() != 2) {
            error = u"a cubic-bezier jims:segment carries exactly two controls";
            return false;
        }
        if (seg.interpolation == u"linear" && !seg.controls.empty()) {
            error = u"a linear jims:segment carries no controls";
            return false;
        }
        out.segments.push_back(seg);
    }
    if (out.segments.empty()) {
        error = u"jims:tuning-trajectory carries no segment";
        return false;
    }
    return true;
}

//---------------------------------------------------------
//   checkSharedStatesAcrossParts
//---------------------------------------------------------

bool JimsImportContext::checkSharedStatesAcrossParts(MusicXmlLogger* logger) const
{
    // Timeline signature per part: the ordered (tick, staff number, state) list.
    //
    // Narrowed by owner ruling 2026-08-22 (mirrors the export side): parts are
    // compared on the Kernel's shared projection of each state, not the raw
    // state JSON. The projection omits only the per-staff extent; tonic-ambit
    // remains compared because it is one song-wide value repeated through
    // transport carriers. The Kernel owns the field classification; the fork
    // compares only the projection it is handed.
    auto sharedForm = [&logger](const BufferedState& s, String& out) {
        String err;
        if (!jims::musicxmlSharedStateV3Xml(s.json, out, &err)) {
            jimsFatal(logger, String(u"JiMS import: the Kernel could not derive the shared state form: %1").arg(err));
            return false;
        }
        return true;
    };

    const std::vector<BufferedState>* reference = nullptr;
    String referenceId;
    for (const auto& entry : m_states) {
        const String& partId = entry.first;
        const std::vector<BufferedState>& states = entry.second;
        if (!reference) {
            reference = &states;
            referenceId = partId;
            continue;
        }
        bool same = states.size() == reference->size();
        for (size_t i = 0; same && i < states.size(); ++i) {
            const BufferedState& a = (*reference)[i];
            const BufferedState& b = states[i];
            if (a.tick != b.tick || a.staffNumber != b.staffNumber) {
                same = false;
                break;
            }
            String sharedA, sharedB;
            if (!sharedForm(a, sharedA) || !sharedForm(b, sharedB)) {
                return false;
            }
            same = sharedA == sharedB;
        }
        if (!same) {
            jimsFatal(logger, String(u"JiMS parts %1 and %2 carry different jims:staff-state timelines; "
                                     u"every JiMS part of a document must share one state timeline")
                      .arg(referenceId, partId));
            return false;
        }
    }
    return true;
}
}
