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
#include "meijims.h"

#include <algorithm>
#include <sstream>

#include "engraving/dom/factory.h"
#include "engraving/dom/harmony.h"
#include "engraving/dom/measure.h"
#include "engraving/dom/note.h"
#include "engraving/dom/part.h"
#include "engraving/dom/score.h"
#include "engraving/dom/segment.h"
#include "engraving/dom/staff.h"
#include "engraving/dom/stafftype.h"
#include "engraving/dom/keysig.h"
#include "engraving/dom/stafftypechange.h"
#include "engraving/jims/jimsbridge.h"
#include "meiconverter.h"
#include "libmei.h"
#include "engraving/jims/jimschange.h"
#include "engraving/style/style.h"
#include "global/serialization/json.h"

#include "log.h"

using namespace mu::engraving;
using muse::String;

namespace mu::iex::mei {
//---------------------------------------------------------
// small helpers
//---------------------------------------------------------

static std::string fracStr(const Fraction& quarters)
{
    Fraction f = quarters.reduced();
    return std::to_string(f.numerator()) + "/" + std::to_string(f.denominator());
}

static Fraction quartersOf(const Fraction& f)
{
    return (f * Fraction(4, 1)).reduced();
}

/// Minimal decimal formatting for a tstamp double (no trailing zeros).
static std::string tstampStr(double tstamp)
{
    std::ostringstream oss;
    oss << tstamp;
    return oss.str();
}

static double tstampFrom(const Fraction& rtick, const Fraction& timesig)
{
    // 1 + offset expressed in meter units (same as Convert::tstampFromFraction).
    return 1.0 + (rtick / timesig * Fraction(timesig.denominator(), 1)).toDouble();
}

static const char* accidOf(int alter)
{
    switch (alter) {
    case 1: return "s";
    case -1: return "f";
    case 2: return "xs";
    case -2: return "ff";
    default: return nullptr;
    }
}

/// Lattice bound from the Kernel state JSON (transport read, no arithmetic).
static bool extentBounds(const String& stateJson, int out[4])
{
    std::string err;
    muse::JsonDocument doc = muse::JsonDocument::fromJson(stateJson.toUtf8(), &err);
    if (!err.empty() || !doc.isObject()) {
        return false;
    }
    muse::JsonObject extent = doc.rootObject().value("extent").toObject();
    muse::JsonObject lower = extent.value("lower").toObject();
    muse::JsonObject upper = extent.value("upper").toObject();
    out[0] = lower.value("nPer").toInt();
    out[1] = lower.value("nGen").toInt();
    out[2] = upper.value("nPer").toInt();
    out[3] = upper.value("nGen").toInt();
    return true;
}

//---------------------------------------------------------
// typed review value tree <-> jm value elements
//---------------------------------------------------------

static void writeReviewValue(pugi::xml_node parent, const jims::ReviewValue& v)
{
    using Kind = jims::ReviewValue::Kind;
    const char* tag = "jm:z";
    switch (v.kind) {
    case Kind::Object: tag = "jm:o";
        break;
    case Kind::Array: tag = "jm:a";
        break;
    case Kind::String: tag = "jm:s";
        break;
    case Kind::Number: tag = "jm:num";
        break;
    case Kind::Bool: tag = "jm:b";
        break;
    case Kind::Null: tag = "jm:z";
        break;
    }
    pugi::xml_node node = parent.append_child(tag);
    if (!v.name.isEmpty()) {
        node.append_attribute("n") = v.name.toStdString().c_str();
    }
    if (v.kind == Kind::Object || v.kind == Kind::Array) {
        for (const jims::ReviewValue& c : v.children) {
            writeReviewValue(node, c);
        }
    } else if (v.kind != Kind::Null) {
        node.text().set(v.text.toStdString().c_str());
    }
}

static std::string localNameOf(pugi::xml_node node);

static jims::ReviewValue readReviewValue(pugi::xml_node node)
{
    using Kind = jims::ReviewValue::Kind;
    jims::ReviewValue v;
    v.name = String(node.attribute("n").value());
    const std::string tag = localNameOf(node);
    if (tag == "o" || tag == "a") {
        v.kind = (tag == "o") ? Kind::Object : Kind::Array;
        for (pugi::xml_node child : node.children()) {
            v.children.push_back(readReviewValue(child));
        }
    } else if (tag == "s") {
        v.kind = Kind::String;
        v.text = String(node.text().as_string());
    } else if (tag == "num") {
        v.kind = Kind::Number;
        v.text = String(node.text().as_string());
    } else if (tag == "b") {
        v.kind = Kind::Bool;
        v.text = String(node.text().as_string());
    } else {
        v.kind = Kind::Null;
    }
    return v;
}

//---------------------------------------------------------
// JimsMeiExporter
//---------------------------------------------------------

std::string JimsMeiExporter::respIdFor(const String& reviewer)
{
    for (size_t i = 0; i < m_reviewers.size(); ++i) {
        if (m_reviewers.at(i) == reviewer) {
            return "jims-resp-" + std::to_string(i + 1);
        }
    }
    m_reviewers.push_back(reviewer);
    return "jims-resp-" + std::to_string(m_reviewers.size());
}

bool JimsMeiExporter::buildPlan(const Score* score)
{
    m_score = score;
    m_present = false;
    m_error.clear();
    m_staves.clear();
    m_tonicAmbit.clear();
    m_measures.clear();
    m_measureIndex.clear();
    m_harms.clear();
    m_notes.clear();
    m_reviewers.clear();
    m_adjAnnotIds.clear();
    for (const jims::ReviewAdjudication& adj : score->jimsReview().adjudications) {
        if (!adj.reviewer.isEmpty()) {
            respIdFor(adj.reviewer);
        }
    }
    if (!score->jimsReview().empty()) {
        m_present = true;
    }

    // JiMS chord names must be exportable (same contract as MusicXML export).
    for (const Segment* segment = score->firstSegment(SegmentType::ChordRest); segment;
         segment = segment->next1(SegmentType::ChordRest)) {
        for (const EngravingItem* item : segment->annotations()) {
            if (!item || !item->isHarmony()) {
                continue;
            }
            const Harmony* harmony = toHarmony(item);
            if (harmony->harmonyType() != HarmonyType::JIMS) {
                continue;
            }
            const String name = harmony->harmonyName();
            bool whitespace = false;
            for (size_t i = 0; i < name.size(); ++i) {
                if (name.at(i).isSpace()) {
                    whitespace = true;
                }
            }
            if (name.isEmpty() || whitespace || name.contains(u'~')) {
                m_error = u"JiMS MEI export: every JiMS chord name must be nonempty, whitespace-free, and tilde-free";
                return false;
            }
            m_present = true;
        }
    }

    int staffN = 0;
    for (const Staff* staff : score->staves()) {
        ++staffN;
        const StaffType* base = staff->staffType(Fraction(0, 1));
        if (!base || !base->isJiMS()) {
            continue;
        }
        if (!jims::available()) {
            m_error = u"JiMS MEI export: the JiMS Kernel bridge is unavailable";
            return false;
        }
        m_present = true;
        StaffPlan plan;
        plan.staff = staff;
        plan.staffN = staffN;
        plan.staffDefId = "jims-sd-" + std::to_string(staffN);
        plan.states.push_back({ Fraction(0, 1), base->jimsStateJson() });
        for (const Measure* m = score->firstMeasure(); m; m = m->nextMeasure()) {
            for (const StaffTypeChange* carrier : jims::changeCarriers(m, staff->idx())) {
                if (!carrier->staffType() || !carrier->staffType()->isJiMS()) {
                    continue;
                }
                const Fraction tick = m->tick() + carrier->rtick();
                plan.states.push_back({ tick, carrier->staffType()->jimsStateJson() });
            }
        }
        size_t si = 0;
        for (const auto& st : plan.states) {
            UNUSED(st);
            ++si;
            plan.stateAnnotIds.push_back("jims-stannot-" + std::to_string(staffN) + "-" + std::to_string(si));
            plan.jmStateIds.push_back("jims-state-" + std::to_string(staffN) + "-" + std::to_string(si));
        }
        if (m_tonicAmbit.empty()) {
            m_tonicAmbit = base->jimsTonicAmbit();
        }
        m_staves.push_back(plan);
    }
    return true;
}

bool JimsMeiExporter::projectPitch(const String& stateJson, int nPer, int nGen, std::string& pname, int& alter, int& octave)
{
    jims::SoundingPitch projection;
    String error;
    if (!jims::noteSoundingPitch(stateJson, nPer, nGen, projection, &error)) {
        m_error = String(u"JiMS MEI export: extent projection failed: %1").arg(error);
        return false;
    }
    pname = std::string(1, char(std::tolower(projection.step)));
    alter = projection.alter;
    octave = projection.octave;
    return true;
}

bool JimsMeiExporter::onStaffDef(pugi::xml_node staffDefNode, const Staff* staff)
{
    for (StaffPlan& plan : m_staves) {
        if (plan.staff != staff) {
            continue;
        }
        staffDefNode.append_attribute("xml:id") = plan.staffDefId.c_str();
        // Key signature as a child keySig with @mode (mei-basic's @keysig
        // attribute cannot carry the mode; the reader accepts either).
        {
            std::string sig = "0";
            pugi::xml_attribute keysigAttr = staffDefNode.attribute("keysig");
            if (keysigAttr) {
                sig = keysigAttr.value();
                staffDefNode.remove_attribute(keysigAttr);
            }
            pugi::xml_node keySig = staffDefNode.append_child("keySig");
            keySig.append_attribute("sig") = sig.c_str();
            KeyMode mode = plan.staff->keySigEvent(Fraction(0, 1)).mode();
            if (mode == KeyMode::UNKNOWN || mode == KeyMode::NONE) {
                const Measure* first = m_score->firstMeasure();
                const Segment* keySeg = first ? first->findSegment(SegmentType::KeySig, first->tick()) : nullptr;
                if (keySeg) {
                    const EngravingItem* item = keySeg->element(plan.staff->idx() * VOICES);
                    if (item && item->isKeySig()) {
                        mode = toKeySig(item)->keySigEvent().mode();
                    }
                }
            }
            static const std::map<KeyMode, const char*> modeNames = {
                { KeyMode::MAJOR, "major" }, { KeyMode::MINOR, "minor" },
                { KeyMode::DORIAN, "dorian" }, { KeyMode::PHRYGIAN, "phrygian" },
                { KeyMode::LYDIAN, "lydian" }, { KeyMode::MIXOLYDIAN, "mixolydian" },
                { KeyMode::AEOLIAN, "aeolian" }, { KeyMode::IONIAN, "ionian" },
                { KeyMode::LOCRIAN, "locrian" },
            };
            auto it = modeNames.find(mode);
            if (it != modeNames.end()) {
                keySig.append_attribute("mode") = it->second;
            }
        }
        // Extent hull across the state timeline, projected by the Kernel.
        std::string loP, hiP;
        int loAlter = 0, hiAlter = 0, loOct = 0, hiOct = 0;
        bool first = true;
        auto rank = [](const std::string& pname, int oct) {
            static const std::string steps = "cdefgab";
            return oct * 7 + int(steps.find(pname.at(0)));
        };
        for (const auto& st : plan.states) {
            int b[4];
            if (!extentBounds(st.second, b)) {
                m_error = u"JiMS MEI export: cannot read the extent of a staff state";
                return false;
            }
            std::string p;
            int alter = 0, oct = 0;
            if (!projectPitch(st.second, b[0], b[1], p, alter, oct)) {
                return false;
            }
            if (first || rank(p, oct) < rank(loP, loOct)) {
                loP = p;
                loAlter = alter;
                loOct = oct;
            }
            if (!projectPitch(st.second, b[2], b[3], p, alter, oct)) {
                return false;
            }
            if (first || rank(p, oct) > rank(hiP, hiOct)) {
                hiP = p;
                hiAlter = alter;
                hiOct = oct;
            }
            first = false;
        }
        if (!staffDefNode.attribute("clef.shape") && !staffDefNode.child("clef")) {
            const ClefTypeList clefs = plan.staff->clefType(Fraction(0, 1));
            libmei::Clef meiClef = Convert::clefToMEI(clefs.concertClef);
            if (meiClef.HasShape() && meiClef.HasLine()) {
                libmei::AttConverter attConverter;
                pugi::xml_node clefNode = staffDefNode.append_child("clef");
                clefNode.append_attribute("shape") = attConverter.ClefshapeToStr(meiClef.GetShape()).c_str();
                clefNode.append_attribute("line") = meiClef.GetLine();
                if (meiClef.HasDis()) {
                    int dis = 0;
                    switch (meiClef.GetDis()) {
                    case libmei::OCTAVE_DIS_8: dis = 8;
                        break;
                    case libmei::OCTAVE_DIS_15: dis = 15;
                        break;
                    case libmei::OCTAVE_DIS_22: dis = 22;
                        break;
                    default: break;
                    }
                    if (dis) {
                        clefNode.append_attribute("dis") = dis;
                        clefNode.append_attribute("dis.place") = attConverter.StaffrelBasicToStr(meiClef.GetDisPlace()).c_str();
                    }
                }
            }
        }
        pugi::xml_node ambitus = staffDefNode.append_child("ambitus");
        pugi::xml_node lo = ambitus.append_child("ambNote");
        lo.append_attribute("type") = "lowest";
        lo.append_attribute("pname") = loP.c_str();
        lo.append_attribute("oct") = loOct;
        if (const char* a = accidOf(loAlter)) {
            lo.append_attribute("accid") = a;
        }
        pugi::xml_node hi = ambitus.append_child("ambNote");
        hi.append_attribute("type") = "highest";
        hi.append_attribute("pname") = hiP.c_str();
        hi.append_attribute("oct") = hiOct;
        if (const char* a = accidOf(hiAlter)) {
            hi.append_attribute("accid") = a;
        }
    }
    return true;
}

void JimsMeiExporter::onMeasure(const Measure* measure, const std::string& xmlId)
{
    m_measureIndex[measure] = m_measures.size();
    m_measures.push_back({ measure, xmlId });
}

void JimsMeiExporter::writeScoreAnnots(pugi::xml_node scoreNode)
{
    if (!m_present) {
        return;
    }
    if (!m_tonicAmbit.empty()) {
        pugi::xml_node annot = scoreNode.append_child("annot");
        annot.append_attribute("xml:id") = "jims-ambit";
        annot.append_attribute("type") = "jims-tonic-ambit";
        annot.append_attribute("class") = ("#jims.ambit." + m_tonicAmbit.toStdString()).c_str();
        annot.text().set(m_tonicAmbit.toStdString().c_str());
    }
    const jims::ReviewRecord& review = m_score->jimsReview();
    if (!review.focusedReviewReasons.empty()) {
        pugi::xml_node fr = scoreNode.append_child("annot");
        fr.append_attribute("xml:id") = "jims-focused-review";
        fr.append_attribute("type") = "jims-focused-review";
        for (const String& reason : review.focusedReviewReasons) {
            fr.append_child("p").text().set(reason.toStdString().c_str());
        }
    }
    if (!m_staves.empty()) {
        const String token = jims::melodyPartToken(m_score->jimsMelodyPart());
        pugi::xml_node annot = scoreNode.append_child("annot");
        annot.append_attribute("xml:id") = "jims-melody";
        annot.append_attribute("type") = "jims-melody-part";
        annot.append_attribute("class") = ("#jims.melody." + token.toStdString()).c_str();
        for (const StaffPlan& plan : m_staves) {
            const Part* part = plan.staff->part();
            if (part && part->partName().toLower() == token) {
                annot.append_attribute("plist") = ("#" + plan.staffDefId).c_str();
                break;
            }
        }
        annot.text().set(token.toStdString().c_str());
    }
}

void JimsMeiExporter::writeMeasureAnnots(pugi::xml_node measureNode, const Measure* measure)
{
    // Evidentiary adjudications anchored inside this measure. An anchor that
    // no longer lands in the score is STALE: it is marked, never silently
    // re-timed (spec/MAPPING.md fact 12-14).
    const jims::ReviewRecord& review = m_score->jimsReview();
    for (size_t i = 0; i < review.adjudications.size(); ++i) {
        const jims::ReviewAdjudication& adj = review.adjudications.at(i);
        if (adj.tick < measure->tick() || adj.tick >= measure->endTick()) {
            continue;
        }
        pugi::xml_node annot = measureNode.append_child("annot");
        const std::string id = adj.annotId.isEmpty()
                               ? ("jims-adj-" + std::to_string(i + 1)) : adj.annotId.toStdString();
        annot.append_attribute("xml:id") = id.c_str();
        annot.append_attribute("type") = "jims-adjudication";
        annot.append_attribute("class") = ("#jims.outcome." + adj.outcome.toStdString()).c_str();
        annot.append_attribute("tstamp")
            = tstampStr(tstampFrom(adj.tick - measure->tick(), measure->timesig())).c_str();
        if (!adj.reviewer.isEmpty()) {
            annot.append_attribute("resp") = ("#" + respIdFor(adj.reviewer)).c_str();
        }
        for (const String& note : adj.notes) {
            annot.append_child("p").text().set(note.toStdString().c_str());
        }
        pugi::xml_node ptrs = annot.append_child("p");
        for (const String& ev : adj.evidence) {
            pugi::xml_node ptr = ptrs.append_child("ptr");
            ptr.append_attribute("type") = "jims-evidence";
            ptr.append_attribute("target") = ev.toStdString().c_str();
        }
        if (!adj.sourceAnalysis.isEmpty()) {
            pugi::xml_node ptr = ptrs.append_child("ptr");
            ptr.append_attribute("type") = "jims-source-analysis";
            ptr.append_attribute("target") = adj.sourceAnalysis.toStdString().c_str();
        }
        m_adjAnnotIds.push_back(id);
    }
    for (StaffPlan& plan : m_staves) {
        for (size_t si = 0; si < plan.states.size(); ++si) {
            const Fraction tick = plan.states.at(si).first;
            if (tick < measure->tick() || tick >= measure->endTick()) {
                continue;
            }
            pugi::xml_node annot = measureNode.append_child("annot");
            annot.append_attribute("xml:id") = plan.stateAnnotIds.at(si).c_str();
            annot.append_attribute("type") = "jims-tonal-state";
            annot.append_attribute("staff") = plan.staffN;
            annot.append_attribute("tstamp") = tstampStr(tstampFrom(tick - measure->tick(), measure->timesig())).c_str();
            annot.append_attribute("corresp") = ("#" + plan.jmStateIds.at(si)).c_str();
        }
    }
}

void JimsMeiExporter::onHarm(pugi::xml_node harmNode, const Harmony* harmony, const std::string& xmlId)
{
    if (!harmony || harmony->harmonyType() != HarmonyType::JIMS) {
        return;
    }
    // @type is a space-separated token list; ensure the profile token once.
    pugi::xml_attribute type = harmNode.attribute("type");
    const std::string current = type ? type.value() : "";
    if ((" " + current + " ").find(" jims-chord-name ") == std::string::npos) {
        const std::string merged = current.empty() ? "jims-chord-name" : current + " jims-chord-name";
        if (type) {
            type.set_value(merged.c_str());
        } else {
            harmNode.append_attribute("type") = merged.c_str();
        }
    }
    m_harms.push_back({ xmlId, harmony });
}

void JimsMeiExporter::onNote(const Note* note, const std::string& xmlId)
{
    if (!note || !note->hasJimsPitch()) {
        return;
    }
    const StaffType* st = note->staff() ? note->staff()->staffTypeForElement(note) : nullptr;
    if (!st || !st->isJiMS()) {
        return;
    }
    m_notes.push_back({ xmlId, note });
}

bool JimsMeiExporter::writeExtMeta(pugi::xml_node meiHead)
{
    if (!m_present) {
        return true;
    }
    // Header fixups for the mei-jims profile: the fileDesc title carries
    // the movement title, workList the work title, and the composer rides
    // in a native composer element (the reader's authoritative carriers).
    {
        const String movementTitle = m_score->metaTag(u"movementTitle");
        const String workTitle = m_score->metaTag(u"workTitle");
        const String composer = m_score->metaTag(u"composer");
        pugi::xml_node fileDesc = meiHead.child("fileDesc");
        pugi::xml_node titleStmt = fileDesc ? fileDesc.child("titleStmt") : pugi::xml_node();
        if (titleStmt) {
            pugi::xml_node title = titleStmt.child("title");
            if (title && !movementTitle.isEmpty()) {
                title.text().set(movementTitle.toStdString().c_str());
            }
            if (!titleStmt.child("composer") && !composer.isEmpty()) {
                pugi::xml_node comp = titleStmt.insert_child_after("composer", titleStmt.child("title"));
                pugi::xml_node pn = comp.append_child("persName");
                pn.text().set(composer.toStdString().c_str());
            }
        }
        if (!meiHead.child("workList") && !workTitle.isEmpty()) {
            pugi::xml_node revisionForOrder = meiHead.child("revisionDesc");
            pugi::xml_node extForOrder = meiHead.child("extMeta");
            pugi::xml_node anchor = extForOrder ? extForOrder : revisionForOrder;
            pugi::xml_node workList = anchor ? meiHead.insert_child_before("workList", anchor)
                                      : meiHead.append_child("workList");
            pugi::xml_node work = workList.append_child("work");
            pugi::xml_node wt = work.append_child("title");
            wt.text().set(workTitle.toStdString().c_str());
        }
    }

    // Any stored extMeta record would carry stale links; regenerate.
    for (pugi::xml_node ext = meiHead.child("extMeta"); ext;) {
        pugi::xml_node next = ext.next_sibling("extMeta");
        meiHead.remove_child(ext);
        ext = next;
    }
    // extMeta must precede revisionDesc in meiHead.
    pugi::xml_node revision = meiHead.child("revisionDesc");
    pugi::xml_node ext = revision ? meiHead.insert_child_before("extMeta", revision) : meiHead.append_child("extMeta");
    pugi::xml_node rec = ext.append_child("jm:record");
    rec.append_attribute("xmlns:jm") = JIMS_MEI_NS;
    rec.append_attribute("xmlns:jims") = JIMS_MUSICXML_NS;
    rec.append_attribute("version") = "1";
    pugi::xml_node mx = rec.append_child("jm:musicxml");

    for (StaffPlan& plan : m_staves) {
        pugi::xml_node pe = mx.append_child("jm:part");
        pe.append_attribute("ref") = ("#" + plan.staffDefId).c_str();
        for (size_t si = 0; si < plan.states.size(); ++si) {
            const Fraction tick = plan.states.at(si).first;
            const Measure* measure = nullptr;
            size_t midx = 0;
            for (size_t mi = 0; mi < m_measures.size(); ++mi) {
                const Measure* candidate = m_measures.at(mi).first;
                if (tick >= candidate->tick() && tick < candidate->endTick()) {
                    measure = candidate;
                    midx = mi;
                    break;
                }
            }
            if (!measure) {
                m_error = u"JiMS MEI export: a staff state does not sit in an exported measure";
                return false;
            }
            auto mit = m_measureIndex.find(measure);
            UNUSED(mit);
            pugi::xml_node se = pe.append_child("jm:state");
            se.append_attribute("xml:id") = plan.jmStateIds.at(si).c_str();
            se.append_attribute("measure") = int(midx) + 1;
            se.append_attribute("off") = fracStr(quartersOf(tick - measure->tick())).c_str();
            se.append_attribute("annot") = ("#" + plan.stateAnnotIds.at(si)).c_str();
            String fragment;
            String err;
            if (!jims::musicxmlStaffStateV3Xml(plan.states.at(si).second, 0, fragment, &err)) {
                m_error = String(u"JiMS MEI export: the Kernel refused to serialize a staff state: %1").arg(err);
                return false;
            }
            pugi::xml_document fragDoc;
            if (!fragDoc.load_string(fragment.toStdString().c_str())) {
                m_error = u"JiMS MEI export: unparsable Kernel staff-state fragment";
                return false;
            }
            se.append_copy(fragDoc.first_child());
        }
        pugi::xml_node notes = pe.append_child("jm:notes");
        for (const auto& entry : m_notes) {
            if (entry.second->staff() != plan.staff) {
                continue;
            }
            pugi::xml_node ne = notes.append_child("jm:note");
            ne.append_attribute("ref") = ("#" + entry.first).c_str();
            pugi::xml_node px = ne.append_child("jims:pitch");
            px.append_attribute("n-per") = entry.second->jimsNPer();
            px.append_attribute("n-gen") = entry.second->jimsNGen();
        }
        // Tuning trajectories (verbatim carriers; duration-divisions in the
        // JiMS MEI canonical quarter-note basis).
        for (const jims::TuningTrajectory& t : plan.staff->jimsTuningTrajectories()) {
            const Measure* measure = nullptr;
            size_t midx = 0;
            for (size_t mi = 0; mi < m_measures.size(); ++mi) {
                const Measure* candidate = m_measures.at(mi).first;
                if (t.tick >= candidate->tick() && t.tick < candidate->endTick()) {
                    measure = candidate;
                    midx = mi;
                    break;
                }
            }
            if (!measure) {
                continue;
            }
            pugi::xml_node te = pe.append_child("jm:trajectory");
            te.append_attribute("measure") = int(midx) + 1;
            te.append_attribute("off") = fracStr(quartersOf(t.tick - measure->tick())).c_str();
            te.append_attribute("staff") = plan.staffN;
            if (!t.placement.isEmpty()) {
                te.append_attribute("placement") = t.placement.toStdString().c_str();
            }
            pugi::xml_node tt = te.append_child("jims:tuning-trajectory");
            for (const jims::TrajectorySegment& seg : t.segments) {
                pugi::xml_node sege = tt.append_child("jims:segment");
                // JiMS MEI canonical basis: 960 divisions per quarter note.
                const Fraction div = (quartersOf(seg.duration) * Fraction(960, 1)).reduced();
                if (div.denominator() != 1) {
                    m_error = u"JiMS MEI export: a trajectory duration does not fit the canonical 960-division basis";
                    return false;
                }
                sege.append_attribute("duration-divisions") = div.numerator();
                sege.append_attribute("start-cents") = seg.startCents.toStdString().c_str();
                sege.append_attribute("end-cents") = seg.endCents.toStdString().c_str();
                sege.append_attribute("interpolation") = seg.interpolation.toStdString().c_str();
                for (const jims::TrajectoryControl& c : seg.controls) {
                    pugi::xml_node ce = sege.append_child("jims:control");
                    ce.append_attribute("time") = c.time.toStdString().c_str();
                    ce.append_attribute("value-cents") = c.valueCents.toStdString().c_str();
                }
            }
        }
    }

    pugi::xml_node me = mx.append_child("jm:measures");
    for (const auto& entry : m_measures) {
        pugi::xml_node ev = me.append_child("jm:measure");
        ev.append_attribute("ref") = ("#" + entry.second).c_str();
        ev.append_attribute("len") = fracStr(quartersOf(entry.first->ticks())).c_str();
    }

    pugi::xml_node he = mx.append_child("jm:harmony");
    for (const auto& entry : m_harms) {
        const Harmony* harmony = entry.second;
        const EngravingObject* parent = harmony->explicitParent();
        if (!parent || !parent->isSegment()) {
            continue;
        }
        const Segment* segment = toSegment(parent);
        const Measure* measure = segment->measure();
        auto mit = m_measureIndex.find(measure);
        if (mit == m_measureIndex.end()) {
            continue;
        }
        pugi::xml_node ev = he.append_child("jm:event");
        ev.append_attribute("harm") = ("#" + entry.first).c_str();
        ev.append_attribute("measure") = int(mit->second) + 1;
        ev.append_attribute("off") = fracStr(quartersOf(segment->tick() - measure->tick())).c_str();
    }

    // Responsible agents for the evidentiary adjudications.
    if (!m_reviewers.empty()) {
        pugi::xml_node fileDesc = meiHead.child("fileDesc");
        if (!fileDesc) {
            fileDesc = meiHead.prepend_child("fileDesc");
        }
        pugi::xml_node titleStmt = fileDesc.child("titleStmt");
        if (!titleStmt) {
            titleStmt = fileDesc.prepend_child("titleStmt");
        }
        pugi::xml_node respStmt = titleStmt.child("respStmt");
        if (!respStmt) {
            respStmt = titleStmt.append_child("respStmt");
        }
        for (size_t i = 0; i < m_reviewers.size(); ++i) {
            const std::string id = "jims-resp-" + std::to_string(i + 1);
            bool exists = false;
            for (pugi::xml_node pn : respStmt.children("persName")) {
                if (id == pn.attribute("xml:id").value()) {
                    exists = true;
                }
            }
            if (exists) {
                continue;
            }
            pugi::xml_node pn = respStmt.append_child("persName");
            pn.append_attribute("xml:id") = id.c_str();
            pn.append_attribute("role") = "jims-reviewer";
            pn.text().set(m_reviewers.at(i).toStdString().c_str());
        }
    }

    // Native provenance sources (uri, media type, hash) in fileDesc/sourceDesc.
    const jims::Provenance& prov = m_score->jimsProvenance();
    if (!prov.resources.empty()) {
        pugi::xml_node fileDesc = meiHead.child("fileDesc");
        if (!fileDesc) {
            fileDesc = meiHead.prepend_child("fileDesc");
        }
        pugi::xml_node sourceDesc = fileDesc.child("sourceDesc");
        if (!sourceDesc) {
            sourceDesc = fileDesc.append_child("sourceDesc");
        }
        int i = 0;
        for (const jims::ProvenanceResource& r : prov.resources) {
            ++i;
            const std::string id = "jims-src-prov-" + std::to_string(i);
            bool exists = false;
            for (pugi::xml_node src : sourceDesc.children("source")) {
                if (id == src.attribute("xml:id").value()) {
                    exists = true;
                }
            }
            if (exists) {
                continue;
            }
            pugi::xml_node src = sourceDesc.append_child("source");
            src.append_attribute("type") = "jims-provenance";
            src.append_attribute("xml:id") = id.c_str();
            pugi::xml_node bibl = src.append_child("bibl");
            auto ident = [&bibl](const char* type, const String& value) {
                if (value.isEmpty()) {
                    return;
                }
                pugi::xml_node n = bibl.append_child("identifier");
                n.append_attribute("type") = type;
                n.text().set(value.toStdString().c_str());
            };
            ident("uri", r.uri);
            ident("sha-256", r.sha256);
            ident("media-type", r.mediaType);
        }
    }

    // Provenance supplement (the JiMS-constrained remainder).
    if (!prov.empty()) {
        pugi::xml_node ss = mx.append_child("jm:source-supplement");
        ss.append_attribute("strict") = prov.strictFallback ? "true" : "false";
        int i = 0;
        for (const jims::ProvenanceResource& r : prov.resources) {
            ++i;
            pugi::xml_node se = ss.append_child("jm:source");
            se.append_attribute("ref") = ("#jims-src-prov-" + std::to_string(i)).c_str();
            if (!r.role.isEmpty()) {
                se.append_attribute("role") = r.role.toStdString().c_str();
            }
            if (!r.text.isEmpty()) {
                se.text().set(r.text.toStdString().c_str());
            }
        }
    }
    // The evidentiary review record. Every adjudication must still resolve
    // to a live score position; a stale anchor is reported, never silently
    // emitted as valid analysis.
    const jims::ReviewRecord& review = m_score->jimsReview();
    if (!review.empty()) {
        pugi::xml_node rv = rec.append_child("jm:review");
        rv.append_attribute("schema") = review.schema.toStdString().c_str();
        if (!review.work.children.empty()) {
            pugi::xml_node w = rv.append_child("jm:work");
            writeReviewValue(w, review.work);
        }
        // Revision history: date, agent and prose ride the NATIVE
        // revisionDesc (spec/MAPPING.md fact 20); jm:audit carries only the
        // exact field/prior/new payload, linked by @change.
        pugi::xml_node revisionDesc = meiHead.child("revisionDesc");
        if (!revisionDesc && !review.audits.empty()) {
            revisionDesc = meiHead.append_child("revisionDesc");
        }
        for (size_t i = 0; i < review.audits.size(); ++i) {
            const jims::ReviewAudit& a = review.audits.at(i);
            // reuse the imported change identity so a round trip never
            // duplicates the native revision entry
            const std::string id = a.changeId.isEmpty()
                                   ? ("jims-change-" + std::to_string(i + 1))
                                   : a.changeId.toStdString();
            bool exists = false;
            for (pugi::xml_node ch : revisionDesc.children("change")) {
                if (id == ch.attribute("xml:id").value()) {
                    exists = true;
                }
            }
            if (!exists) {
                pugi::xml_node ch = revisionDesc.append_child("change");
                ch.append_attribute("xml:id") = id.c_str();
                if (!a.date.isEmpty()) {
                    ch.append_attribute("isodate") = a.date.toStdString().c_str();
                }
                if (!a.phase.isEmpty()) {
                    ch.append_attribute("label") = a.phase.toStdString().c_str();
                }
                pugi::xml_node cd = ch.append_child("changeDesc");
                cd.append_child("p").text().set(a.reason.toStdString().c_str());
            }
            pugi::xml_node ae = rv.append_child("jm:audit");
            ae.append_attribute("change") = ("#" + id).c_str();
            writeReviewValue(ae, a.record);
        }
        size_t emitted = 0;
        for (size_t i = 0; i < review.adjudications.size(); ++i) {
            const jims::ReviewAdjudication& adj = review.adjudications.at(i);
            const std::string id = adj.annotId.isEmpty()
                                   ? ("jims-adj-" + std::to_string(i + 1)) : adj.annotId.toStdString();
            const bool placed = std::find(m_adjAnnotIds.begin(), m_adjAnnotIds.end(), id) != m_adjAnnotIds.end();
            pugi::xml_node te = rv.append_child("jm:adjudication");
            if (placed) {
                te.append_attribute("annot") = ("#" + id).c_str();
                ++emitted;
            } else {
                // the anchored position no longer exists in this score
                te.append_attribute("stale") = "true";
                te.append_attribute("id") = id.c_str();
                te.append_attribute("tick") = adj.tick.toString().toStdString().c_str();
                LOGW() << "JiMS MEI export: adjudication " << id
                       << " is stale (its score-time anchor no longer resolves); marked stale";
            }
            writeReviewValue(te, adj.record);
        }
        UNUSED(emitted);
    }
    return true;
}

//---------------------------------------------------------
// JimsMeiImporter
//---------------------------------------------------------

void JimsMeiImporter::capture(pugi::xml_node root)
{
    m_error.clear();
    m_staffDefN.clear();
    pugi::xml_node record = root.select_node("//extMeta/*").node();
    if (!record || String(record.name()) != u"jm:record") {
        // Also accept an arbitrary prefix for urn:jims:mei:1 by local name.
        record = pugi::xml_node();
        for (pugi::xpath_node candidate : root.select_nodes("//extMeta/*")) {
            String name = String(candidate.node().name());
            if (name == u"jm:record" || name.endsWith(u":record") || name == u"record") {
                record = candidate.node();
                break;
            }
        }
    }
    if (record) {
        m_recordDoc.reset();
        m_recordDoc.append_copy(record);
        m_record = m_recordDoc.first_child();
    }
    for (pugi::xpath_node sd : root.select_nodes("//staffDef[@xml:id]")) {
        m_staffDefN[sd.node().attribute("xml:id").value()] = sd.node().attribute("n").as_int();
    }
    m_provResources.clear();
    for (pugi::xpath_node src : root.select_nodes("//sourceDesc/source[@type='jims-provenance']")) {
        engraving::jims::ProvenanceResource resource;
        for (pugi::xpath_node ident : src.node().select_nodes(".//identifier")) {
            const std::string type = ident.node().attribute("type").value();
            const String value = String(ident.node().text().as_string());
            if (type == "uri") {
                resource.uri = value;
            } else if (type == "sha-256") {
                resource.sha256 = value;
            } else if (type == "media-type") {
                resource.mediaType = value;
            }
        }
        m_provResources.push_back(resource);
    }
    m_melodyToken.clear();
    pugi::xml_node melody = root.select_node("//score/annot[@type='jims-melody-part']").node();
    if (melody) {
        m_melodyToken = String(melody.text().as_string());
    }

    // Evidentiary review carriers: responsible agents, focused-review
    // reasons, and each adjudication's native annotation (its class,
    // prose, pointers, and exact timing).
    m_reviewerById.clear();
    for (pugi::xpath_node pn : root.select_nodes("//respStmt/persName[@role='jims-reviewer']")) {
        m_reviewerById[pn.node().attribute("xml:id").value()] = String(pn.node().text().as_string());
    }
    m_focusedReviewReasons.clear();
    pugi::xml_node fr = root.select_node("//score/annot[@type='jims-focused-review']").node();
    if (fr) {
        for (pugi::xml_node p : fr.children("p")) {
            m_focusedReviewReasons.push_back(String(p.text().as_string()));
        }
    }
    m_adjAnnots.clear();
    for (pugi::xpath_node a : root.select_nodes("//measure/annot[@type='jims-adjudication']")) {
        m_adjAnnots[a.node().attribute("xml:id").value()] = a.node();
    }
    m_changeById.clear();
    for (pugi::xpath_node ch : root.select_nodes("//revisionDesc/change[@xml:id]")) {
        ChangeEntry entry;
        entry.date = String(ch.node().attribute("isodate").value());
        entry.phase = String(ch.node().attribute("label").value());
        entry.reason = String(ch.node().select_node(".//changeDesc/p").node().text().as_string());
        m_changeById[ch.node().attribute("xml:id").value()] = entry;
    }
    m_adjMeasureIndex.clear();
    int mi = 0;
    for (pugi::xpath_node m : root.select_nodes("//section/measure")) {
        for (pugi::xpath_node a : m.node().select_nodes("./annot[@type='jims-adjudication']")) {
            m_adjMeasureIndex[a.node().attribute("xml:id").value()] = mi;
        }
        ++mi;
    }
}

/// jx local name of a node whose prefix is unknown ("jx:staff-state",
/// "jims:staff-state", or unprefixed).
static std::string localName(pugi::xml_node node)
{
    std::string name = node.name();
    size_t colon = name.find(':');
    return colon == std::string::npos ? name : name.substr(colon + 1);
}

static std::string localNameOf(pugi::xml_node node)
{
    return localName(node);
}

static pugi::xml_node childByLocal(pugi::xml_node parent, const char* local)
{
    for (pugi::xml_node child : parent.children()) {
        if (localName(child) == local) {
            return child;
        }
    }
    return pugi::xml_node();
}

bool JimsMeiImporter::stateJsonFromXml(pugi::xml_node staffStateNode, String& json)
{
    // Mirrors JimsImportContext::parseStaffState's converter byte-shape:
    // fixed key order, no spaces, tonic_ambit last (the musicxml importer
    // remains the owning transcription; re-sync on change).
    auto jsonNumber = [](const std::string& text, bool& ok) -> std::string {
        ok = false;
        if (text.empty()) {
            return text;
        }
        char* end = nullptr;
        std::strtod(text.c_str(), &end);
        ok = end && *end == '\0';
        return text;
    };

    std::vector<std::string> steps;
    pugi::xml_node scale = childByLocal(staffStateNode, "scale");
    for (pugi::xml_node step : scale.children()) {
        if (localName(step) == "step") {
            steps.push_back(step.text().as_string());
        }
    }
    pugi::xml_node embedding = childByLocal(staffStateNode, "embedding");
    pugi::xml_node extent = childByLocal(staffStateNode, "extent");
    pugi::xml_node reference = childByLocal(staffStateNode, "reference");
    pugi::xml_node ambit = childByLocal(staffStateNode, "tonic-ambit");
    if (!ambit) {
        ambit = childByLocal(staffStateNode, "tonic-extent");   // legacy spelling, read only
    }
    if (steps.empty() || !embedding || !extent
        || !childByLocal(staffStateNode, "collection-rotation")
        || !childByLocal(staffStateNode, "mode-rotation")
        || !childByLocal(staffStateNode, "generator-cents")
        || !childByLocal(staffStateNode, "period-cents")) {
        m_error = u"jims:staff-state in extMeta is missing a required child";
        return false;
    }
    bool okG = false, okP = false;
    const std::string gen = jsonNumber(childByLocal(staffStateNode, "generator-cents").text().as_string(), okG);
    const std::string per = jsonNumber(childByLocal(staffStateNode, "period-cents").text().as_string(), okP);
    if (!okG || !okP) {
        m_error = u"jims:staff-state cents fields are not numbers";
        return false;
    }
    std::string referenceJson = "\"none\"";
    if (reference) {
        pugi::xml_node form = reference.first_child();
        const std::string kind = localName(form);
        if (kind == "none") {
            referenceJson = "\"none\"";
        } else if (kind == "reference-pitch") {
            referenceJson = "{\"reference-pitch\":{\"key_number\":" + std::string(form.attribute("key-number").value()) + "}}";
        } else if (kind == "pitch-class") {
            referenceJson = "{\"pitch-class\":{\"pitch_class\":" + std::string(form.text().as_string()) + "}}";
        } else if (kind == "frequency-hz") {
            referenceJson = "{\"frequency-hz\":{\"hertz\":" + std::string(form.text().as_string()) + "}}";
        } else {
            m_error = String(u"unknown jims:reference form '%1'").arg(String::fromStdString(kind));
            return false;
        }
    }
    std::string scaleJson = "[";
    for (size_t i = 0; i < steps.size(); ++i) {
        if (i) {
            scaleJson += ",";
        }
        scaleJson += "\"" + steps.at(i) + "\"";
    }
    scaleJson += "]";
    std::string out = "{\"scale\":" + scaleJson
                      + ",\"collection_rotation\":" + std::string(childByLocal(staffStateNode, "collection-rotation").text().as_string())
                      + ",\"mode_rotation\":" + std::string(childByLocal(staffStateNode, "mode-rotation").text().as_string())
                      + ",\"generator_cents\":" + gen
                      + ",\"period_cents\":" + per
                      + ",\"embedding\":{\"large_steps\":" + std::string(embedding.attribute("large-steps").value())
                      + ",\"small_steps\":" + std::string(embedding.attribute("small-steps").value()) + "}"
                      + ",\"extent\":{\"lower\":{\"nPer\":" + std::string(extent.attribute("lower-n-per").value())
                      + ",\"nGen\":" + std::string(extent.attribute("lower-n-gen").value())
                      + "},\"upper\":{\"nPer\":" + std::string(extent.attribute("upper-n-per").value())
                      + ",\"nGen\":" + std::string(extent.attribute("upper-n-gen").value()) + "}}"
                      + ",\"reference\":" + referenceJson;
    if (ambit) {
        out += ",\"tonic_ambit\":\"" + std::string(ambit.text().as_string()) + "\"";
    }
    out += "}";
    json = String::fromStdString(out);
    return true;
}

bool JimsMeiImporter::apply(Score* score,
                            const std::function<Note* (const std::string&)>& noteForId,
                            const std::function<int(int)>& staffIndexForN)
{
    if (!present()) {
        return true;
    }
    if (!jims::available()) {
        m_error = u"JiMS MEI import: the JiMS Kernel bridge is unavailable";
        return false;
    }
    pugi::xml_node mx;
    for (pugi::xml_node child : m_record.children()) {
        if (localName(child) == "musicxml") {
            mx = child;
        }
    }
    if (!mx) {
        m_error = u"JiMS MEI import: jm:record carries no jm:musicxml section";
        return false;
    }

    static const StaffType* jimsPreset = StaffType::preset(StaffTypes::JIMS_12TET);
    UNUSED(jimsPreset);

    // Measure index -> Measure*
    std::vector<Measure*> measures;
    for (Measure* m = score->firstMeasure(); m; m = m->nextMeasure()) {
        measures.push_back(m);
    }
    auto tickOf = [&](pugi::xml_node node, Fraction& tick) -> bool {
        const int midx = node.attribute("measure").as_int() - 1;
        if (midx < 0 || midx >= int(measures.size())) {
            m_error = u"JiMS MEI import: a record names a measure outside the score";
            return false;
        }
        std::string off = node.attribute("off").value();
        const size_t slash = off.find('/');
        if (slash == std::string::npos) {
            m_error = u"JiMS MEI import: a record offset is not a rational number";
            return false;
        }
        const Fraction quarters(std::stoi(off.substr(0, slash)), std::stoi(off.substr(slash + 1)));
        tick = measures.at(midx)->tick() + (quarters / Fraction(4, 1)).reduced();
        return true;
    };

    bool anyState = false;
    for (pugi::xml_node pe : mx.children()) {
        if (localName(pe) != "part") {
            continue;
        }
        const std::string ref = std::string(pe.attribute("ref").value()).substr(1);
        auto nIt = m_staffDefN.find(ref);
        if (nIt == m_staffDefN.end()) {
            m_error = u"JiMS MEI import: a jm:part names no staffDef of this file";
            return false;
        }
        const int staffIdx = staffIndexForN(nIt->second);
        if (staffIdx < 0 || staffIdx >= int(score->nstaves())) {
            m_error = u"JiMS MEI import: a jm:part resolves to no score staff";
            return false;
        }
        Staff* staff = score->staff(staffIdx);
        bool first = true;
        Fraction lastTick(-1, 1);
        for (pugi::xml_node se : pe.children()) {
            const std::string local = localName(se);
            if (local == "state") {
                pugi::xml_node stateNode = childByLocal(se, "staff-state");
                String json;
                if (!stateNode || !stateJsonFromXml(stateNode, json)) {
                    if (m_error.isEmpty()) {
                        m_error = u"JiMS MEI import: jm:state carries no staff-state";
                    }
                    return false;
                }
                String kernelError;
                if (!jims::validateState(json, kernelError)) {
                    m_error = String(u"JiMS MEI import: the Kernel rejected a staff state: %1").arg(kernelError);
                    return false;
                }
                Fraction tick;
                if (!tickOf(se, tick)) {
                    return false;
                }
                StaffType st = *StaffType::preset(StaffTypes::JIMS_12TET);
                st.setJiMS(true);
                st.setJimsJiLines(true);
                st.setJimsStateJson(json);
                if (first) {
                    if (!tick.isZero()) {
                        m_error = u"JiMS MEI import: the first staff state must sit at the start of the score";
                        return false;
                    }
                    staff->setStaffType(Fraction(0, 1), st);
                    first = false;
                } else {
                    if (tick <= lastTick) {
                        m_error = u"JiMS MEI import: staff states must be in strictly increasing order";
                        return false;
                    }
                    Measure* measure = score->tick2measure(tick);
                    if (!measure || tick < measure->tick() || tick >= measure->endTick()) {
                        m_error = u"JiMS MEI import: a staff state does not sit inside a measure";
                        return false;
                    }
                    const Fraction rtick = tick - measure->tick();
                    if (!measure->canAddStaffTypeChange(staff->idx(), rtick)) {
                        m_error = u"JiMS MEI import: cannot place a staff type change at the exact state tick";
                        return false;
                    }
                    StaffTypeChange* stc = Factory::createStaffTypeChange(measure);
                    stc->setTrack(staff->idx() * VOICES);
                    stc->setParent(measure);
                    stc->setRtick(rtick);
                    stc->setStaffType(new StaffType(st), true);
                    if (rtick.isNotZero() && !measure->findSegmentR(Segment::CHORD_REST_OR_TIME_TICK_TYPE, rtick)) {
                        measure->getSegmentR(SegmentType::TimeTick, rtick);
                    }
                    measure->add(stc);
                }
                lastTick = tick;
                anyState = true;
            } else if (local == "notes") {
                for (pugi::xml_node ne : se.children()) {
                    if (localName(ne) != "note") {
                        continue;
                    }
                    const std::string noteRef = std::string(ne.attribute("ref").value()).substr(1);
                    Note* note = noteForId(noteRef);
                    pugi::xml_node px = childByLocal(ne, "pitch");
                    if (!note || !px) {
                        m_error = u"JiMS MEI import: a note-identity record does not resolve";
                        return false;
                    }
                    note->setJimsPitch(px.attribute("n-per").as_int(), px.attribute("n-gen").as_int());
                }
            } else if (local == "trajectory") {
                Fraction tick;
                if (!tickOf(se, tick)) {
                    return false;
                }
                pugi::xml_node tt = childByLocal(se, "tuning-trajectory");
                if (!tt) {
                    continue;
                }
                jims::TuningTrajectory trajectory;
                trajectory.tick = tick;
                trajectory.placement = String(se.attribute("placement").value());
                for (pugi::xml_node sege : tt.children()) {
                    if (localName(sege) != "segment") {
                        continue;
                    }
                    jims::TrajectorySegment seg;
                    // duration-divisions in the JiMS MEI canonical basis of
                    // 960 divisions per quarter note.
                    const int div = sege.attribute("duration-divisions").as_int();
                    seg.duration = Fraction(div, 960 * 4).reduced();
                    seg.startCents = String(sege.attribute("start-cents").value());
                    seg.endCents = String(sege.attribute("end-cents").value());
                    seg.interpolation = String(sege.attribute("interpolation").value());
                    for (pugi::xml_node ce : sege.children()) {
                        if (localName(ce) != "control") {
                            continue;
                        }
                        jims::TrajectoryControl c;
                        c.time = String(ce.attribute("time").value());
                        c.valueCents = String(ce.attribute("value-cents").value());
                        seg.controls.push_back(c);
                    }
                    trajectory.segments.push_back(seg);
                }
                staff->addJimsTuningTrajectory(trajectory);
            }
        }
    }

    // Evidentiary review record: the jm payload supplies the exact fields,
    // the native annotations supply category, prose, agent, and timing.
    pugi::xml_node rv;
    for (pugi::xml_node child : m_record.children()) {
        if (localName(child) == "review") {
            rv = child;
        }
    }
    if (rv) {
        jims::ReviewRecord review;
        review.schema = String(rv.attribute("schema").value());
        review.focusedReviewReasons = m_focusedReviewReasons;
        for (pugi::xml_node child : rv.children()) {
            const std::string tag = localName(child);
            if (tag == "work") {
                for (pugi::xml_node v : child.children()) {
                    review.work = readReviewValue(v);
                }
            } else if (tag == "audit") {
                jims::ReviewAudit a;
                const std::string cid = std::string(child.attribute("change").value()).substr(1);
                a.changeId = String::fromStdString(cid);
                auto cit = m_changeById.find(cid);
                if (cit != m_changeById.end()) {
                    a.date = cit->second.date;
                    a.phase = cit->second.phase;
                    a.reason = cit->second.reason;
                }
                for (pugi::xml_node v : child.children()) {
                    a.record = readReviewValue(v);
                }
                review.audits.push_back(a);
            } else if (tag == "adjudication") {
                jims::ReviewAdjudication adj;
                for (pugi::xml_node v : child.children()) {
                    adj.record = readReviewValue(v);
                }
                if (std::string(child.attribute("stale").value()) == "true") {
                    // a stale record round-trips as stale; it is never
                    // re-anchored to an arbitrary position
                    adj.annotId = String(child.attribute("id").value());
                    adj.tick = Fraction::fromString(String(child.attribute("tick").value()));
                    review.adjudications.push_back(adj);
                    continue;
                }
                const std::string aid = std::string(child.attribute("annot").value()).substr(1);
                auto ait = m_adjAnnots.find(aid);
                if (ait == m_adjAnnots.end()) {
                    m_error = u"JiMS MEI import: an adjudication record does not resolve to its annotation";
                    return false;
                }
                pugi::xml_node annot = ait->second;
                adj.annotId = String::fromStdString(aid);
                const std::string cls = annot.attribute("class").value();
                const size_t dot = cls.rfind('.');
                adj.outcome = String::fromStdString(dot == std::string::npos ? cls : cls.substr(dot + 1));
                auto rit = m_reviewerById.find(std::string(annot.attribute("resp").value()).substr(1));
                if (rit != m_reviewerById.end()) {
                    adj.reviewer = rit->second;
                }
                for (pugi::xml_node p : annot.children("p")) {
                    if (p.child("ptr")) {
                        for (pugi::xml_node ptr : p.children("ptr")) {
                            const std::string type = ptr.attribute("type").value();
                            if (type == "jims-evidence") {
                                adj.evidence.push_back(String(ptr.attribute("target").value()));
                            } else if (type == "jims-source-analysis") {
                                adj.sourceAnalysis = String(ptr.attribute("target").value());
                            }
                        }
                    } else {
                        adj.notes.push_back(String(p.text().as_string()));
                    }
                }
                auto mit = m_adjMeasureIndex.find(aid);
                if (mit != m_adjMeasureIndex.end() && mit->second < int(measures.size())) {
                    Measure* measure = measures.at(mit->second);
                    const double tstamp = annot.attribute("tstamp").as_double(1.0);
                    const Fraction timesig = measure->timesig();
                    const Fraction off = Fraction::fromTicks(
                        int(std::lround((tstamp - 1.0) * timesig.denominator() > 0
                                        ? (tstamp - 1.0) / timesig.denominator() * 4.0 * Constants::DIVISION
                                        : 0.0)));
                    adj.tick = measure->tick() + off;
                }
                review.adjudications.push_back(adj);
            }
        }
        score->setJimsReview(review);
    }

    // Melody-part designation (typed native annotation).
    if (!m_melodyToken.isEmpty()) {
        jims::MelodyPart melodyPart = jims::MelodyPart::Soprano;
        if (jims::melodyPartFromToken(m_melodyToken, melodyPart)) {
            score->setJimsMelodyPart(melodyPart);
        }
    }

    // Provenance: native sources plus the JiMS-constrained supplement.
    pugi::xml_node ss = childByLocal(mx, "source-supplement");
    if (!m_provResources.empty() || ss) {
        jims::Provenance prov;
        prov.resources = m_provResources;
        if (ss) {
            prov.strictFallback = std::string(ss.attribute("strict").value()) == "true";
            int i = 0;
            for (pugi::xml_node se : ss.children()) {
                if (localName(se) != "source") {
                    continue;
                }
                if (i < int(prov.resources.size())) {
                    prov.resources.at(i).role = String(se.attribute("role").value());
                    prov.resources.at(i).text = String(se.text().as_string());
                }
                ++i;
            }
        }
        score->setJimsProvenance(prov);
    }

    if (anyState) {
        score->style().set(Sid::musicalSymbolFont, String(u"JiMSMusic"));
        score->style().set(Sid::hideInstrumentNameIfOneInstrument, false);
    }
    return true;
}
} // namespace mu::iex::mei
