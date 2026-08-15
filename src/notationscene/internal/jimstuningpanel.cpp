/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 *
 * JiMStaff Milestone 3 — continuous-tuning panel. See jimstuningpanel.h.
 *
 * Owner correction 2026-08-15: the slider is VERTICAL, high value at the
 * top, laid out like the Wikipedia "Rank-2 temperaments with the
 * generator close to a fifth and period an octave" figure: the
 * Purer-Tuning Range band and notable non-equal tunings to the LEFT of
 * the slider, the stack of integer cent values immediately to its RIGHT,
 * and the notable equal tunings beyond that. Every annotation row is
 * positioned through the slider's own value->pixel mapping, so the
 * integer stack is aligned with the slider values by construction.
 */
#include "jimstuningpanel.h"

#include <QDoubleSpinBox>
#include <QFontMetrics>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
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
// A QSlider that exposes its groove/handle geometry so sibling
// annotations can be aligned to the exact pixel of each value.
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

    // y (in this slider's coordinates) at which the given slider VALUE
    // sits: the handle centre for that value. Vertical, not inverted, so
    // the maximum is at the top (upsideDown == true in Qt's terms).
    double yForValue(int value) const
    {
        const QRect groove = grooveRect();
        const int handleLen = handleRect().height();
        const int span = groove.height() - handleLen;
        const int pos = QStyle::sliderPositionFromValue(minimum(), maximum(), value, span, true);
        return groove.top() + handleLen / 2.0 + pos;
    }
};

// Rows of the figure. Equal tunings are placed on the integer row the
// figure shows them on (its rounding of each ET's fifth); the slider
// itself remains continuous at SLIDER_STEP resolution.
struct EqualTuning {
    int cents;
    const char* et;
    const char* name;
};
const EqualTuning EQUAL_TUNINGS[] = {
    { 720, "5-TET", "Indonesian slendro" },
    { 709, "22-TET", "" },
    { 706, "17-TET", "" },
    { 702, "53-TET", "Turkish" },
    { 700, "12-TET", "Western (c. 1800 - now)" },
    { 698, "43-TET", "" },
    { 697, "31-TET", "Huygens/Fokker" },
    { 696, "50-TET", "" },
    { 695, "19-TET", "Guillaume Costeley" },
    { 692, "26-TET", "" },
    { 686, "7-TET", "Thai traditional, Mandinka balafon" },
};

struct NonEqualTuning {
    int cents;
    const char* pure;    // far-left "Pure n/m" column (may be empty)
    const char* name;    // column adjacent to the band
};
const NonEqualTuning NON_EQUAL_TUNINGS[] = {
    { 702, "Pure 3/2", "Pythagorean" },
    { 698, "", "1/5-comma" },
    { 697, "Pure 5/4", "1/4-comma" },
    { 696, "", "2/7-comma" },
    { 695, "Pure 6/5", "1/3-comma" },
};
// Ordinal Tuning Ranges (right side of the figure): the generator ranges
// over which the temperament's intervals keep the ordinal ranking of the
// stated prime limit's Just Intonation intervals. Endpoints are read off
// the figure (owner request 2026-08-15); the 5-limit band spans the whole
// diatonic Valid Tuning Range.
struct OrdinalRange {
    double lowCents;
    double highCents;
    const char* caption;
    QColor color;
};
const OrdinalRange ORDINAL_RANGES[] = {
    { 686.0, 720.0, "5-limit", QColor(120, 150, 200, 110) },
    { 696.5, 700.5, "7-limit", QColor(200, 90, 80, 150) },
    { 695.5, 697.0, "11-limit", QColor(240, 160, 60, 160) },
};

constexpr int PURER_TOP_CENTS = 702;      // band spans these rows inclusive
constexpr int PURER_BOTTOM_CENTS = 695;
constexpr int INTEGER_TOP = 720;
constexpr int INTEGER_BOTTOM = 686;

// The scale: paints the annotations and positions the child slider.
class TuningScale : public QWidget
{
public:
    TuningScale(ScaleSlider* slider, double step, QWidget* parent)
        : QWidget(parent), m_slider(slider), m_step(step)
    {
        m_slider->setParent(this);
        m_slider->setOrientation(Qt::Vertical);
        m_slider->setInvertedAppearance(false);   // max (720) at the top
        setMinimumHeight(ROW_PX * (INTEGER_TOP - INTEGER_BOTTOM + 1) + 2 * MARGIN_Y);
        computeColumns();
        setMinimumWidth(m_totalWidth);
    }

protected:
    void resizeEvent(QResizeEvent*) override
    {
        m_slider->setGeometry(m_sliderX, MARGIN_Y, SLIDER_W, height() - 2 * MARGIN_Y);
    }

    void paintEvent(QPaintEvent*) override
    {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing, true);
        p.setRenderHint(QPainter::TextAntialiasing, true);
        const QFontMetrics fm(font());
        QFont boldForMetrics = font();
        boldForMetrics.setBold(true);
        const QFontMetrics fmb(boldForMetrics);
        const int textH = fm.height();

        auto yFor = [this](int cents) {
            return m_slider->y() + m_slider->yForValue(int(cents / m_step + 0.5));
        };
        auto rowRect = [&](int cents, int x, int w) {
            return QRectF(x, yFor(cents) - textH / 2.0, w, textH);
        };

        // Purer-Tuning Range band: just left of the slider, spanning the
        // named non-equal tunings, gold.
        const double bandTop = yFor(PURER_TOP_CENTS) - textH / 2.0;
        const double bandBottom = yFor(PURER_BOTTOM_CENTS) + textH / 2.0;
        const QRectF band(m_bandX, bandTop, m_sliderX - GAP - m_bandX, bandBottom - bandTop);
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(212, 175, 55, 140));      // gold, translucent
        p.drawRoundedRect(band, 4, 4);

        // Rotated caption for the band, far left.
        p.setPen(palette().color(QPalette::WindowText));
        {
            QFont f = font();
            f.setBold(true);
            p.save();
            p.setFont(f);
            const QString caption = QStringLiteral("Purer-Tuning Range");
            const double cx = MARGIN_X + textH / 2.0;
            const double cy = (bandTop + bandBottom) / 2.0;
            p.translate(cx, cy);
            p.rotate(-90);
            p.drawText(QRectF(-m_captionLen / 2.0, -textH / 2.0, m_captionLen, textH),
                       Qt::AlignCenter, caption);
            p.restore();
        }

        // Non-equal tunings, right-aligned against the band's right edge.
        for (const NonEqualTuning& t : NON_EQUAL_TUNINGS) {
            p.drawText(rowRect(t.cents, m_pureX, m_pureW), Qt::AlignRight | Qt::AlignVCenter,
                       QString::fromUtf8(t.pure));
            p.drawText(rowRect(t.cents, m_nameX, m_nameW), Qt::AlignRight | Qt::AlignVCenter,
                       QString::fromUtf8(t.name));
        }

        // Integer stack immediately right of the slider, one row per cent.
        for (int c = INTEGER_TOP; c >= INTEGER_BOTTOM; --c) {
            p.drawText(rowRect(c, m_intX, m_intW), Qt::AlignRight | Qt::AlignVCenter,
                       QString::number(c));
        }

        // Equal tunings: bold ET name, then the cultural label.
        QFont bold = font();
        bold.setBold(true);
        for (const EqualTuning& t : EQUAL_TUNINGS) {
            p.setFont(bold);
            p.drawText(rowRect(t.cents, m_etX, m_etW), Qt::AlignLeft | Qt::AlignVCenter,
                       QString::fromUtf8(t.et));
            p.setFont(font());
            p.drawText(rowRect(t.cents, m_cultX, m_cultW), Qt::AlignLeft | Qt::AlignVCenter,
                       QString::fromUtf8(t.name));
        }

        // Ordinal Tuning Ranges: one translucent band per prime limit,
        // continuous endpoints mapped through the slider like everything
        // else, rotated caption centred on each band.
        auto yForD = [this](double cents) {
            return m_slider->y() + m_slider->yForValue(int(cents / m_step + 0.5));
        };
        int bx = m_ordX;
        for (const OrdinalRange& r : ORDINAL_RANGES) {
            const double top = yForD(r.highCents);
            const double bottom = yForD(r.lowCents);
            p.setPen(Qt::NoPen);
            p.setBrush(r.color);
            p.drawRoundedRect(QRectF(bx, top, BAND_W, bottom - top), 3, 3);
            p.setPen(palette().color(QPalette::WindowText));
            p.setFont(bold);
            p.save();
            const double cx = bx + BAND_W + GAP / 2.0 + textH / 2.0;
            const double cy = (top + bottom) / 2.0;
            const int len = fmb.horizontalAdvance(QString::fromUtf8(r.caption));
            p.translate(cx, cy);
            p.rotate(-90);
            p.drawText(QRectF(-len / 2.0, -textH / 2.0, len, textH), Qt::AlignCenter,
                       QString::fromUtf8(r.caption));
            p.restore();
            p.setFont(font());
            bx += BAND_W + GAP / 2 + textH + GAP;
        }
        {
            p.setFont(bold);
            p.save();
            const QString caption = QStringLiteral("Ordinal Tuning Ranges");
            const double cx = m_ordCapX + textH / 2.0;
            const double cy = (yForD(INTEGER_TOP) + yForD(INTEGER_BOTTOM)) / 2.0;
            p.translate(cx, cy);
            p.rotate(-90);
            p.drawText(QRectF(-m_ordCapLen / 2.0, -textH / 2.0, m_ordCapLen, textH),
                       Qt::AlignCenter, caption);
            p.restore();
            p.setFont(font());
        }
    }

private:
    void computeColumns()
    {
        const QFontMetrics fm(font());
        QFont bold = font();
        bold.setBold(true);
        const QFontMetrics fmb(bold);
        const int textH = fm.height();

        m_captionLen = fmb.horizontalAdvance(QStringLiteral("Purer-Tuning Range"));
        m_pureW = 0;
        m_nameW = 0;
        for (const NonEqualTuning& t : NON_EQUAL_TUNINGS) {
            m_pureW = std::max(m_pureW, fm.horizontalAdvance(QString::fromUtf8(t.pure)));
            m_nameW = std::max(m_nameW, fm.horizontalAdvance(QString::fromUtf8(t.name)));
        }
        m_intW = fm.horizontalAdvance(QStringLiteral("720"));
        m_etW = 0;
        m_cultW = 0;
        for (const EqualTuning& t : EQUAL_TUNINGS) {
            m_etW = std::max(m_etW, fmb.horizontalAdvance(QString::fromUtf8(t.et)));
            m_cultW = std::max(m_cultW, fm.horizontalAdvance(QString::fromUtf8(t.name)));
        }

        int x = MARGIN_X;
        x += textH + GAP;                 // rotated caption column
        m_bandX = x;
        m_pureX = x + GAP;
        x = m_pureX + m_pureW + GAP;
        m_nameX = x;
        x = m_nameX + m_nameW + GAP;      // band ends here (+GAP before slider)
        m_sliderX = x + GAP;
        x = m_sliderX + SLIDER_W + GAP;
        m_intX = x;
        x = m_intX + m_intW + GAP * 2;
        m_etX = x;
        x = m_etX + m_etW + GAP * 2;
        m_cultX = x;
        x = m_cultX + m_cultW + GAP * 2;
        m_ordX = x;
        for (const OrdinalRange& r : ORDINAL_RANGES) {
            (void)r;
            x += BAND_W + GAP / 2 + textH + GAP;
        }
        m_ordCapX = x + GAP;
        m_ordCapLen = fmb.horizontalAdvance(QStringLiteral("Ordinal Tuning Ranges"));
        m_totalWidth = m_ordCapX + textH + MARGIN_X;
    }

    static constexpr int ROW_PX = 15;     // minimum pixels per cent
    static constexpr int MARGIN_X = 8;
    static constexpr int MARGIN_Y = 10;
    static constexpr int GAP = 6;
    static constexpr int SLIDER_W = 24;
    static constexpr int BAND_W = 12;

    ScaleSlider* m_slider = nullptr;
    double m_step = 0.1;
    int m_captionLen = 0;
    int m_bandX = 0, m_pureX = 0, m_pureW = 0, m_nameX = 0, m_nameW = 0;
    int m_sliderX = 0, m_intX = 0, m_intW = 0, m_etX = 0, m_etW = 0, m_cultX = 0, m_cultW = 0;
    int m_ordX = 0, m_ordCapX = 0, m_ordCapLen = 0;
    int m_totalWidth = 0;
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
    auto* scale = new TuningScale(slider, SLIDER_STEP, this);
    outer->addWidget(scale, 1);

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
