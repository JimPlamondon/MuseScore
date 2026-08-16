/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 *
 * JiMStaff Milestone 3 — continuous-tuning panel. See jimstuningpanel.h.
 *
 * Owner corrections 2026-08-15/16: the slider is VERTICAL, high value at
 * the top, set INTO the owner's own continuum figure ("Rank-2
 * temperaments with the generator close to a fifth and period an octave",
 * Wikimedia Commons, CC BY-SA 4.0): the figure is split just left of its
 * integer cent stack and the slider occupies the slot, its handle centre
 * solved onto the figure's cent rows through the slider's own
 * value->pixel mapping.
 */
#include "jimstuningpanel.h"

#include <QDoubleSpinBox>
#include <QFontMetrics>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QSlider>
#include <QStyle>
#include <QStyleOptionSlider>
#include <QVBoxLayout>

#include "engraving/dom/score.h"
#include "engraving/jims/jimsbridge.h"
#include "engraving/jims/jimstuningcontroller.h"

using namespace mu::notationscene;
using namespace mu::engraving;

namespace {
// A QSlider that exposes its groove/handle geometry so the figure's rows
// can be aligned to the exact pixel of each value.
class ScaleSlider : public QSlider
{
public:
    using QSlider::QSlider;

    QRect grooveRect() const
    {
        QStyleOptionSlider opt;
        initStyleOption(&opt);
        return style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);
    }

    QRect handleRect() const
    {
        QStyleOptionSlider opt;
        initStyleOption(&opt);
        return style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    }

    // y (slider coordinates) of the handle centre for VALUE; vertical,
    // not inverted, so the maximum is at the top.
    double yForValue(int value) const
    {
        const QRect groove = grooveRect();
        const int handleLen = handleRect().height();
        const int span = groove.height() - handleLen;
        const int pos = QStyle::sliderPositionFromValue(minimum(), maximum(), value, span, true);
        return groove.top() + handleLen / 2.0 + pos;
    }
};

// The owner's continuum figure (Wikimedia Commons, JimPlamondon, CC BY-SA
// 4.0; see internal/jims/LICENSE.rank2_figure.txt): 461 x 729 px, integer
// cent rows 720 (y 121.5) ... 686 (y 665.5), 16 px per cent, integer stack
// starting at x 173 with the Purer-Tuning Range band ending just left of
// it. The panel splits the figure at x 173 and inserts a slot for the
// slider so it sits "just left of the integer stack, just right of the
// gold band" (owner request 2026-08-16), and solves the slider geometry
// so its handle centre lands exactly on the figure's cent rows.
constexpr double FIG_Y_720 = 121.5;
constexpr double FIG_PX_PER_CENT = 16.0;
constexpr int FIG_SPLIT_X = 173;
// The header band ("Width of the generator (in cents)" and the two
// column captions) ends above the stack's arrow tip (y ~105); it is
// drawn UNSPLIT, centred over slot + stack (owner correction 2026-08-16).
constexpr int FIG_HEADER_Y = 100;

class TuningFigure : public QWidget
{
public:
    TuningFigure(ScaleSlider* slider, double step, double minCents, double maxCents, QWidget* parent)
        : QWidget(parent), m_slider(slider), m_step(step), m_minCents(minCents), m_maxCents(maxCents)
    {
        m_figure.load(QStringLiteral(":/jims/internal/jims/rank2_temperaments_fifth_octave.jpg"));
        m_slider->setParent(this);
        m_slider->setOrientation(Qt::Vertical);
        m_slider->setInvertedAppearance(false);   // max (720) at the top
        setMinimumHeight(560);
        setMinimumWidth(int(m_figure.width() * 0.6) + SLOT_W);
    }

    double scale() const
    {
        return m_figure.isNull() ? 1.0 : double(height()) / double(m_figure.height());
    }

    // Figure y for a cents value, in widget pixels.
    double yForCents(double cents) const
    {
        return (FIG_Y_720 + (720.0 - cents) * FIG_PX_PER_CENT) * scale();
    }

protected:
    void resizeEvent(QResizeEvent*) override
    {
        placeSlider();
    }

    void paintEvent(QPaintEvent*) override
    {
        QPainter p(this);
        p.setRenderHint(QPainter::SmoothPixmapTransform, true);
        if (m_figure.isNull()) {
            p.drawText(rect(), Qt::AlignCenter, QStringLiteral("(continuum figure unavailable)"));
            return;
        }
        const double s = scale();
        const int leftW = int(std::lround(FIG_SPLIT_X * s));
        const double headerH = FIG_HEADER_Y * s;
        const double bodyH = m_figure.height() - FIG_HEADER_Y;
        // Header band, unsplit, shifted half a slot so its captions stay
        // centred over the (slot + stack) column below.
        p.drawPixmap(QRectF(SLOT_W / 2.0, 0, m_figure.width() * s, headerH),
                     m_figure, QRectF(0, 0, m_figure.width(), FIG_HEADER_Y));
        // Body, left of the integer stack (band + non-equal tunings).
        p.drawPixmap(QRectF(0, headerH, leftW, bodyH * s),
                     m_figure, QRectF(0, FIG_HEADER_Y, FIG_SPLIT_X, bodyH));
        // Slot for the slider, then the body right of it (stack onward).
        const int rightX = leftW + SLOT_W;
        const double rightSrcW = m_figure.width() - FIG_SPLIT_X;
        p.drawPixmap(QRectF(rightX, headerH, rightSrcW * s, bodyH * s),
                     m_figure, QRectF(FIG_SPLIT_X, FIG_HEADER_Y, rightSrcW, bodyH));
        // Slot background: continue the stack's pale strip so the slider
        // reads as part of the figure.
        p.fillRect(QRectF(leftW, headerH, SLOT_W, height() - headerH), QColor(236, 224, 236, 120));
    }

    QSize sizeHint() const override
    {
        const int h = 700;
        return QSize(int(m_figure.width() * (double(h) / std::max(1, m_figure.height()))) + SLOT_W, h);
    }

private:
    // Solve the slider height so that (yForValue(min) - yForValue(max))
    // equals the figure's pixel distance for the value range, then offset
    // it so yForValue(max) sits on the 720 row. Two or three iterations
    // converge under any style because span is affine in height.
    void placeSlider()
    {
        const double s = scale();
        const int leftW = int(std::lround(FIG_SPLIT_X * s));
        const double wantSpan = (m_maxCents - m_minCents) * FIG_PX_PER_CENT * s;
        int h = int(wantSpan) + 40;
        m_slider->setGeometry(leftW + (SLOT_W - SLIDER_W) / 2, 0, SLIDER_W, h);
        for (int i = 0; i < 4; ++i) {
            const double got = m_slider->yForValue(m_slider->minimum()) - m_slider->yForValue(m_slider->maximum());
            const int delta = int(std::lround(wantSpan - got));
            if (delta == 0) {
                break;
            }
            h += delta;
            m_slider->setGeometry(m_slider->x(), 0, SLIDER_W, h);
        }
        const double topInSlider = m_slider->yForValue(m_slider->maximum());
        const int y = int(std::lround(yForCents(m_maxCents) - topInSlider));
        m_slider->setGeometry(m_slider->x(), y, SLIDER_W, h);
    }

    static constexpr int SLOT_W = 26;
    static constexpr int SLIDER_W = 22;

    ScaleSlider* m_slider = nullptr;
    QPixmap m_figure;
    double m_step = 0.1;
    double m_minCents = 686.0;
    double m_maxCents = 720.0;
};
} // namespace

JimsTuningPanel::JimsTuningPanel(Score* score, std::function<void()> refreshView,
                                 muse::async::Notification scoreChanged, QWidget* parent)
    : QWidget(parent, Qt::Tool), m_refreshView(std::move(refreshView))
{
    setWindowTitle(QStringLiteral("JiMS Tuning"));
    m_controller = std::make_unique<jims::TuningController>(score, 0);

    // Bounds are the Kernel's diatonic Valid Tuning Range (owner
    // correction 2026-08-14) — never a fork-side constant.
    jims::generatorRange(m_minCents, m_maxCents);

    auto* outer = new QVBoxLayout(this);

    auto* entryRow = new QHBoxLayout();
    auto* label = new QLabel(QStringLiteral("M5="), this);
    m_spin = new QDoubleSpinBox(this);
    m_spin->setRange(m_minCents, m_maxCents);
    m_spin->setDecimals(3);
    m_spin->setSingleStep(SLIDER_STEP);
    m_spin->setSuffix(QStringLiteral("¢"));
    entryRow->addWidget(label);
    entryRow->addWidget(m_spin, 1);
    outer->addLayout(entryRow);

    auto* slider = new ScaleSlider(Qt::Vertical, nullptr);
    slider->setRange(int(m_minCents / SLIDER_STEP), int(m_maxCents / SLIDER_STEP));
    m_slider = slider;
    auto* figure = new TuningFigure(slider, SLIDER_STEP, m_minCents, m_maxCents, this);
    outer->addWidget(figure, 1);
    auto* credit = new QLabel(QStringLiteral("Figure: J. Plamondon, CC BY-SA 4.0 (Wikimedia Commons)"), this);
    credit->setStyleSheet(QStringLiteral("color: gray; font-size: 9pt;"));
    outer->addWidget(credit);

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
