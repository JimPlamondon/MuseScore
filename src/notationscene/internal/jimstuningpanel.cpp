/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 *
 * JiMStaff Milestone 3 — continuous-tuning panel. See jimstuningpanel.h.
 */
#include "jimstuningpanel.h"

#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>

#include "engraving/dom/score.h"
#include "engraving/jims/jimstuningcontroller.h"

using namespace mu::notationscene;
using namespace mu::engraving;

JimsTuningPanel::JimsTuningPanel(Score* score, std::function<void()> refreshView, QWidget* parent)
    : QWidget(parent, Qt::Tool), m_refreshView(std::move(refreshView))
{
    setWindowTitle(QStringLiteral("JiMS Tuning"));
    m_controller = std::make_unique<jims::TuningController>(score, 0);

    auto* layout = new QHBoxLayout(this);
    auto* label = new QLabel(QStringLiteral("M5="), this);
    m_slider = new QSlider(Qt::Horizontal, this);
    m_slider->setRange(int(MIN_CENTS / SLIDER_STEP), int(MAX_CENTS / SLIDER_STEP));
    m_slider->setMinimumWidth(320);
    m_spin = new QDoubleSpinBox(this);
    m_spin->setRange(MIN_CENTS, MAX_CENTS);
    m_spin->setDecimals(3);
    m_spin->setSingleStep(SLIDER_STEP);
    m_spin->setSuffix(QStringLiteral("¢"));
    layout->addWidget(label);
    layout->addWidget(m_slider, 1);
    layout->addWidget(m_spin);

    syncFromScore();

    connect(m_slider, &QSlider::sliderPressed, this, &JimsTuningPanel::onSliderPressed);
    connect(m_slider, &QSlider::sliderMoved, this, &JimsTuningPanel::onSliderMoved);
    connect(m_slider, &QSlider::sliderReleased, this, &JimsTuningPanel::onSliderReleased);
    connect(m_spin, &QDoubleSpinBox::editingFinished, this, &JimsTuningPanel::onSpinAccepted);
}

JimsTuningPanel::~JimsTuningPanel() = default;

void JimsTuningPanel::syncFromScore()
{
    const double cents = m_controller->currentGeneratorCents();
    if (cents > 0.0) {
        QSignalBlocker sb(m_slider);
        QSignalBlocker bb(m_spin);
        m_slider->setValue(int(cents / SLIDER_STEP + 0.5));
        m_spin->setValue(cents);
    }
}

void JimsTuningPanel::onSliderPressed()
{
    m_dragging = m_controller->beginPreview();
}

void JimsTuningPanel::onSliderMoved(int value)
{
    if (!m_dragging) {
        return;
    }
    const double cents = value * SLIDER_STEP;
    if (m_controller->preview(cents)) {
        QSignalBlocker bb(m_spin);
        m_spin->setValue(cents);
        if (m_refreshView) {
            m_refreshView();
        }
    }
}

void JimsTuningPanel::onSliderReleased()
{
    if (!m_dragging) {
        return;
    }
    m_dragging = false;
    if (m_controller->commit(m_slider->value() * SLIDER_STEP) && m_refreshView) {
        m_refreshView();
    }
}

void JimsTuningPanel::onSpinAccepted()
{
    const double cents = m_spin->value();
    if (!m_controller->beginPreview()) {
        return;
    }
    if (m_controller->commit(cents)) {
        QSignalBlocker sb(m_slider);
        m_slider->setValue(int(cents / SLIDER_STEP + 0.5));
        if (m_refreshView) {
            m_refreshView();
        }
    }
}
