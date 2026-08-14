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
#include "engraving/jims/jimsbridge.h"
#include "engraving/jims/jimstuningcontroller.h"

using namespace mu::notationscene;
using namespace mu::engraving;

JimsTuningPanel::JimsTuningPanel(Score* score, std::function<void()> refreshView,
                                 muse::async::Notification scoreChanged, QWidget* parent)
    : QWidget(parent, Qt::Tool), m_refreshView(std::move(refreshView))
{
    setWindowTitle(QStringLiteral("JiMS Tuning"));
    m_controller = std::make_unique<jims::TuningController>(score, 0);

    // Bounds are the Kernel's diatonic Valid Tuning Range (owner
    // correction 2026-08-14) — never a fork-side constant.
    jims::generatorRange(m_minCents, m_maxCents);

    auto* layout = new QHBoxLayout(this);
    auto* label = new QLabel(QStringLiteral("M5="), this);
    m_slider = new QSlider(Qt::Horizontal, this);
    m_slider->setRange(int(m_minCents / SLIDER_STEP), int(m_maxCents / SLIDER_STEP));
    m_slider->setMinimumWidth(320);
    m_spin = new QDoubleSpinBox(this);
    m_spin->setRange(m_minCents, m_maxCents);
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

    // Undo/redo (and any other score change) resyncs the controls
    // (owner correction 2026-08-14: the staff updated, the slider
    // didn't). Preview drags refresh the view too; skip resync while
    // the user is the one moving the control.
    scoreChanged.onNotify(this, [this]() {
        if (!m_dragging) {
            syncFromScore();
        }
    });
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
