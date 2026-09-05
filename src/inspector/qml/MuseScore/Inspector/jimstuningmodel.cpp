/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 * Copyright (C) 2026 Jim Plamondon
 */
#include "jimstuningmodel.h"
#include <cmath>
#include <QLocale>
#include "translation.h"
#include "engraving/dom/score.h"
#include "engraving/dom/staff.h"
#include "engraving/dom/stafftype.h"
#include "engraving/jims/jimsbridge.h"

using namespace mu::inspector;
using namespace mu::engraving;

JimsTuningModel::JimsTuningModel(QObject* parent)
    : QObject(parent), muse::Contextable(muse::iocCtxForQmlObject(this))
{
    jims::generatorRange(m_minimum, m_maximum);
}

JimsTuningModel::~JimsTuningModel()
{
    cancel();
}

void JimsTuningModel::init()
{
    context()->currentNotationChanged().onNotify(this, [this]() { setNotation(); });
    setNotation();
}

void JimsTuningModel::setNotation()
{
    cancel();
    if (m_notation) {
        m_notation->notationChanged().disconnect(this);
    }
    m_controller.reset();
    m_notation = context()->currentNotation();
    m_error.clear();
    if (m_notation) {
        m_notation->notationChanged().onNotify(this, [this]() { refresh(); });
    }
    refresh();
}

void JimsTuningModel::refresh()
{
    if (m_busy || m_previewing) {
        return;
    }
    m_controller.reset();
    Score* score = m_notation ? m_notation->elements()->msScore() : nullptr;
    if (score) {
        for (const Staff* staff : score->staves()) {
            if (staff->staffType(Fraction(0, 1))->isJiMS()) {
                m_controller = std::make_unique<jims::TuningController>(score, staff->idx());
                break;
            }
        }
    }
    emit changed();
}

double JimsTuningModel::cents() const
{
    return m_controller ? m_controller->currentGeneratorCents() : 0.0;
}

bool JimsTuningModel::valid(double value)
{
    if (std::isfinite(value) && value >= m_minimum && value <= m_maximum) {
        m_error.clear();
        return true;
    }
    m_error = muse::qtrc("notation", "Enter a tuning between %1 and %2 cents.")
              .arg(m_minimum, 0, 'f', 3).arg(m_maximum, 0, 'f', 3);
    emit changed();
    return false;
}

bool JimsTuningModel::beginPreview()
{
    if (m_previewing) {
        return true;
    }
    m_originalCents = cents();
    m_previewing = m_controller && m_controller->beginPreview();
    return m_previewing;
}

void JimsTuningModel::notifyNotation()
{
    m_busy = true;
    if (m_notation) {
        m_notation->notationChanged().notify();
    }
    m_busy = false;
    emit changed();
}

void JimsTuningModel::preview(double value)
{
    if (!valid(value) || !beginPreview()) {
        return;
    }
    m_busy = true;
    const bool ok = m_controller->preview(value);
    m_busy = false;
    if (!ok) {
        m_error = muse::qtrc("notation", "This score cannot use that tuning. The previous tuning is preserved.");
        cancel();
    }
    notifyNotation();
}

void JimsTuningModel::commit(double value)
{
    if (!valid(value)) {
        cancel();
        return;
    }
    if (!m_controller || (!m_previewing && std::abs(value - cents()) < 0.000001)) {
        emit changed();
        return;
    }
    if (m_previewing && std::abs(value - m_originalCents) < 0.000001) {
        cancel();
        return;
    }
    if (!beginPreview()) {
        m_error = muse::qtrc("notation", "Select a score with a compatible staff before changing tuning.");
        emit changed();
        return;
    }
    m_busy = true;
    const bool ok = m_controller->commit(value);
    m_previewing = false;
    m_busy = false;
    if (!ok) {
        m_controller->cancel();
        m_error = muse::qtrc("notation", "Tuning could not be applied. The previous tuning is preserved.");
    }
    notifyNotation();
}

void JimsTuningModel::acceptText(const QString& text)
{
    bool ok = false;
    const double value = QLocale().toDouble(text, &ok);
    if (!ok) {
        m_error = muse::qtrc("notation", "Enter a number in cents.");
        emit changed();
        return;
    }
    commit(value);
}

void JimsTuningModel::cancel()
{
    if (!m_controller || !m_previewing) {
        return;
    }
    m_busy = true;
    m_controller->cancel();
    m_previewing = false;
    m_busy = false;
    notifyNotation();
}
