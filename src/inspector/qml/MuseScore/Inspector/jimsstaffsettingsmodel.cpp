// SPDX-License-Identifier: GPL-3.0-only
// MuseScore-Studio-CLA-applies
#include "jimsstaffsettingsmodel.h"
#include "inspector/internal/ielementrepositoryservice.h"
#include "engraving/dom/score.h"
#include "engraving/dom/staff.h"
#include "engraving/dom/stafftype.h"
#include "engraving/dom/measure.h"
#include "engraving/dom/segment.h"
#include "engraving/editing/editstaff.h"
#include "engraving/jims/jimschange.h"
#include "engraving/jims/jimsstrings.h"
#include <QLocale>
using namespace mu::inspector;
using namespace mu::engraving;

JimsStaffSettingsModel::JimsStaffSettingsModel(QObject* parent, const muse::modularity::ContextPtr& ctx,
                                               IElementRepositoryService* repository)
    : AbstractInspectorModel(parent, ctx, repository)
{
    setSectionType(InspectorSectionType::SECTION_JIMS_STAFF);
    setTitle(jims::staffUserName().toQString());
}

void JimsStaffSettingsModel::requestElements()
{
    m_elementList = m_repository->takeAllElements();
}

bool JimsStaffSettingsModel::target(Score*& score, Measure*& measure, Fraction& tick, staff_idx_t& staff) const
{
    for (EngravingItem* item : m_repository->takeAllElements()) {
        if (!item || !item->staff() || !item->staff()->staffType(item->tick())->isJiMS()) {
            continue;
        }
        score = item->score();
        staff = item->staffIdx();
        const Selection& selection = score->selection();
        if (selection.isRange() && selection.startSegment()) {
            measure = selection.startSegment()->measure();
            tick = selection.startSegment()->tick();
        } else {
            measure = item->findMeasure();
            tick = item->tick();
        }
        if (measure && tick == measure->endTick() && measure->nextMeasure()) {
            measure = measure->nextMeasure();
            tick = measure->tick();
        }
        return measure != nullptr;
    }
    return false;
}

void JimsStaffSettingsModel::loadProperties()
{
    m_settings.clear();
    m_options = {};
    m_scaleSteps.clear();
    Score* score = nullptr;
    Measure* measure = nullptr;
    Fraction tick;
    staff_idx_t staff = 0;
    bool valid = target(score, measure, tick, staff) && jims::changeOptions(score, staff, measure, tick, m_options);
    const QString identity = valid ? QString::number(quintptr(score)) + ":" + QString::number(staff) + ":"
                             + QString::number(tick.ticks()) : QString();
    if (identity != m_targetIdentity) {
        m_targetIdentity = identity;
        m_status.clear();
        m_hasError = false;
        emit statusChanged();
    }
    m_settings["available"] = valid;
    if (valid) {
        m_settings["target"] = muse::qtrc("inspector",
                                          "Measure %1 · position %2 ticks · staff %3").arg(measure->no()
                                                                                           + 1).arg((tick
                                                                                                     - measure->tick()).ticks()).arg(staff
                                                                                                                                     + 1);
        m_settings["referenceBound"] = m_options.referenceBound;
        m_settings["hasChange"] = jims::changeCarrierAt(measure, staff, tick) != nullptr;
        if (m_settings["hasChange"].toBool()) {
            QString description = muse::qtrc("inspector", "This position carries a change.");
            if (!tick.isZero()) {
                const StaffType* before = score->staff(staff)->staffType(Fraction::fromTicks(tick.ticks() - 1));
                const StaffType* here = score->staff(staff)->staffType(tick);
                jims::ChangeIndicator indicator;
                muse::String why;
                if (before && here && jims::changeIndicator(before->jimsStateJson(), here->jimsStateJson(), indicator, &why)) {
                    description = indicator.empty() ? muse::qtrc("inspector",
                                                                 "This position carries a metadata-only change, so no indicator is drawn.")
                                  : muse::qtrc("inspector", "A tonal change indicator is drawn at this position.");
                } else {
                    description = muse::qtrc("inspector", "No change indicator can be drawn: %1").arg(why.toQString());
                }
            }
            m_settings["indicator"] = description;
        }

        const StaffType* base = score->staff(staff)->staffType(Fraction(0, 1));
        m_settings["elision"] = int(base->jimsElideOctaves());
        m_settings["labels"] = int(base->jimsScaleDotLabelMode());
        auto choices = [this](const char* key, const std::vector<jims::StateChangeOption>& options) {
            QVariantList list;
            int current = -1;
            for (const auto& option : options) {
                if (option.current) {
                    current = int(list.size());
                }
                QString label = option.label.toQString();
                if (QString::fromLatin1(key) == "keys") {
                    label = muse::qtrc("inspector", "%1 (period shift: %2)").arg(label).arg(option.nPer);
                }
                list << QVariantMap { { "text", label }, { "value", int(list.size()) } };
            }
            m_settings[key] = list;
            m_settings[QString::fromLatin1(key) + "Index"] = current;
        };
        choices("tonics", m_options.tonics);
        choices("keys", m_options.keyTargets);
        const jims::StateChangeOption* diatonic = nullptr;
        const jims::StateChangeOption* harmonic = nullptr;
        const jims::StateChangeOption* zero = nullptr;
        const jims::StateChangeOption* minor = nullptr;
        for (const auto& option : m_options.cycles) {
            if (option.id == u"scale:cycle:diatonic") {
                diatonic = &option;
            }
            if (option.id == u"scale:cycle:double-harmonic-minor") {
                harmonic = &option;
            }
        }
        for (const auto& option : m_options.rotations) {
            if (option.id == u"scale:rotation:0") {
                zero = &option;
            }
            if (option.id == u"scale:rotation:-3") {
                minor = &option;
            }
        }
        QVariantList scales;
        int currentScale = -1;
        auto addScale = [&](QString name, const jims::StateChangeOption* cycle, const jims::StateChangeOption* rotation) {
            if (!cycle || !rotation) {
                return;
            }
            if (cycle->current && rotation->current) {
                currentScale = int(scales.size());
            }
            scales << QVariantMap { { "text", name }, { "value", int(scales.size()) } };
            std::vector<muse::String> steps;
            if (cycle != rotation) {
                steps.push_back(cycle->id);
            }
            steps.push_back(rotation->id);
            m_scaleSteps.push_back(steps);
        };
        addScale(muse::qtrc("inspector", "Diatonic (White notes)"), diatonic, zero);
        addScale(muse::qtrc("inspector", "Parallel Minor (Grey notes)"), diatonic, minor);
        addScale(muse::qtrc("inspector", "Double Harmonic Minor"), harmonic, harmonic);
        m_settings["scales"] = scales;
        m_settings["scalesIndex"] = currentScale;
        muse::String why;
        m_settings["canChange"] = jims::canInsertChange(score, staff, measure, tick, why);
        m_settings["reason"] = why.toQString();
    }
    emit settingsChanged();
}

void JimsStaffSettingsModel::finish(bool ok, const muse::String& error, const QString& success)
{
    m_hasError = !ok;
    m_status = ok ? success : error.toQString();
    if (m_status.isEmpty() && !ok) {
        m_status = muse::qtrc("inspector", "This change could not be applied. The score is unchanged.");
    }
    emit statusChanged();
    if (!m_status.isEmpty() && accessibilityController()) {
        accessibilityController()->announce(m_status);
    }
    if (ok) {
        if (currentNotation() && currentNotation()->undoStack()) {
            currentNotation()->undoStack()->stackChanged().notify();
        }
        updateNotation();
    }
    loadProperties();
}

void JimsStaffSettingsModel::applyOption(const QString& group, int index)
{
    // Re-read immediately: an open dropdown may outlive a selection or undo change.
    loadProperties();
    Score* score = nullptr;
    Measure* measure = nullptr;
    Fraction tick;
    staff_idx_t staff = 0;
    if (!target(score, measure, tick, staff)) {
        return;
    }
    std::vector<muse::String> steps;
    const auto& options = group == "tonics" ? m_options.tonics : m_options.keyTargets;
    if (group == "scales" && index >= 0 && size_t(index) < m_scaleSteps.size()) {
        steps = m_scaleSteps[index];
    } else if ((group == "tonics" || group == "keys") && index >= 0 && size_t(index) < options.size()) {
        steps = { options[index].id };
    }
    if (steps.empty()) {
        return;
    }
    auto states = [&]() {
        std::vector<muse::String> result;
        for (staff_idx_t index = 0; index < score->nstaves(); ++index) {
            muse::String state;
            if (jims::effectiveState(score, index, measure, tick, state)) {
                result.push_back(state);
            }
        }
        return result;
    };
    const auto before = states();
    muse::String error;
    bool ok = jims::applyChangeToAllJimsParts(score, measure, tick, steps, error);
    const auto after = states();
    finish(ok, error,
           before == after ? muse::qtrc("inspector", "Already selected; no change was needed.") : muse::qtrc("inspector",
                                                                                                             "Applied to all compatible parts at this position."));
}

void JimsStaffSettingsModel::bindReference(const QString& pitch)
{
    bool numeric = false;
    int value = QLocale().toInt(pitch, &numeric);
    muse::String error;
    if (!numeric) {
        finish(false, muse::mtrc("inspector", "Enter a whole reference-pitch number."), {});
        return;
    }
    Score* score = nullptr;
    Measure* measure = nullptr;
    Fraction tick;
    staff_idx_t staff = 0;
    if (!target(score, measure, tick, staff)) {
        return;
    }
    bool ok = jims::applyChange(score, staff, measure, tick, muse::String(u"bind:reference-pitch:%1").arg(value), error);
    finish(ok, error, muse::qtrc("inspector", "Reference pitch bound for this staff."));
}

void JimsStaffSettingsModel::removeChange()
{
    Score* score = nullptr;
    Measure* measure = nullptr;
    Fraction tick;
    staff_idx_t staff = 0;
    if (!target(score, measure, tick, staff)) {
        return;
    }
    muse::String error;
    bool ok = jims::removeChange(score, staff, measure, tick, error);
    finish(ok, error, muse::qtrc("inspector", "Change removed from this staff."));
}

void JimsStaffSettingsModel::setStaffOption(const QString& name, int value)
{
    if (value < 0 || (name == "elision" ? value > 2 : name != "labels" || value > 3)) {
        return;
    }
    Score* score = nullptr;
    Measure* measure = nullptr;
    Fraction tick;
    staff_idx_t staffIndex = 0;
    if (!target(score, measure, tick, staffIndex)) {
        return;
    }
    Staff* staff = score->staff(staffIndex);
    StaffType edited = *staff->staffType(Fraction(0, 1));
    if (name == "elision") {
        if (int(edited.jimsElideOctaves()) == value) {
            return;
        }
        edited.setJimsElideOctaves(JimsElideOctaves(value));
    } else {
        if (int(edited.jimsScaleDotLabelMode()) == value) {
            return;
        }
        edited.setJimsScaleDotLabelMode(JimsScaleDotLabelMode(value));
    }
    score->startCmd(name == "elision" ? muse::TranslatableString("undoableAction",
                                                                 "Octave-band elision override")
                    : muse::TranslatableString("undoableAction", "Change staff type"));
    score->undo(new ChangeStaffType(staff, edited));
    score->endCmd();
    finish(true, {}, muse::qtrc("inspector", "Staff presentation updated."));
}

QColor JimsStaffSettingsModel::criticalColor() const
{
    return engravingConfiguration()->criticalColor().toQColor();
}
