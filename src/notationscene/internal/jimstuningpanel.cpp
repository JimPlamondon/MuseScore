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

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QEvent>
#include <QFontMetrics>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QPainter>
#include <QPixmap>
#include <QSlider>
#include <QStyle>
#include <QStyleOptionSlider>
#include <QVBoxLayout>

#include "engraving/dom/measure.h"
#include "engraving/dom/score.h"
#include "engraving/dom/segment.h"
#include "engraving/dom/staff.h"
#include "engraving/dom/stafftype.h"
#include "engraving/dom/select.h"
#include "engraving/jims/jimsbridge.h"
#include "engraving/jims/jimschange.h"
#include "engraving/jims/jimschangecontroller.h"
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

// The continuum figure by Andrew Milne & Jim Plamondon (Wikimedia Commons,
// CC BY-SA 4.0; the Commons page credits Plamondon alone — that is wrong; see internal/jims/LICENSE.rank2_figure.txt): 461 x 729 px, integer
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
    : QWidget(parent, Qt::Tool), m_refreshView(std::move(refreshView)), m_score(score)
{
    setWindowTitle(QStringLiteral("JiMS Staff"));
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
    auto* credit = new QLabel(QStringLiteral("Figure: Andrew Milne & Jim Plamondon, CC BY-SA 4.0 (Wikimedia Commons)"), this);
    credit->setStyleSheet(QStringLiteral("color: gray; font-size: 9pt;"));
    outer->addWidget(credit);

    // Milestone 6: mode / key / scale changes at the selected measure.
    buildChangeSection(this, outer);

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

// ---------------------------------------------------------------------------
// Milestone 6 — change section (owner decision 1a, 2026-08-16). Every list
// is populated from the Kernel's state_change_options for the effective
// state at the target measure; every button applies one Kernel-issued
// choice id through jims::applyChange (one undo step); the fork holds no
// musical table and computes no new state.
// ---------------------------------------------------------------------------

void JimsTuningPanel::buildChangeSection(QWidget* parent, QVBoxLayout* outer)
{
    m_changeBox = new QGroupBox(QStringLiteral("Change at the selected bar"), parent);
    auto* grid = new QVBoxLayout(m_changeBox);

    m_targetLabel = new QLabel(QStringLiteral("Select a note or bar on the JiMStaff"), m_changeBox);
    grid->addWidget(m_targetLabel);

    // Mode: new tonic (Kernel tonic candidates, canonical solfa labels).
    auto* modeRow = new QHBoxLayout();
    modeRow->addWidget(new QLabel(QStringLiteral("Mode: tonic"), m_changeBox));
    m_tonicCombo = new QComboBox(m_changeBox);
    modeRow->addWidget(m_tonicCombo, 1);
    auto* modeApply = new QPushButton(QStringLiteral("Apply"), m_changeBox);
    modeRow->addWidget(modeApply);
    grid->addLayout(modeRow);

    // Key: Do0 moves to Xx N (the SHIFT–Do0–XxN gesture); needs a bound reference.
    auto* keyRow = new QHBoxLayout();
    keyRow->addWidget(new QLabel(QStringLiteral("Key: Do0 →"), m_changeBox));
    m_keyClassCombo = new QComboBox(m_changeBox);
    keyRow->addWidget(m_keyClassCombo, 1);
    m_keyOctaveSpin = new QSpinBox(m_changeBox);
    m_keyOctaveSpin->setRange(-1, 1);
    m_keyOctaveSpin->setValue(0);
    m_keyOctaveSpin->setToolTip(QStringLiteral("Octave N of XxN relative to Do0"));
    keyRow->addWidget(m_keyOctaveSpin);
    m_keyApply = new QPushButton(QStringLiteral("Apply"), m_changeBox);
    keyRow->addWidget(m_keyApply);
    grid->addLayout(keyRow);

    auto* bindRow = new QHBoxLayout();
    bindRow->addWidget(new QLabel(QStringLiteral("Bind Re0 to key number"), m_changeBox));
    m_bindSpin = new QSpinBox(m_changeBox);
    m_bindSpin->setRange(0, 127);
    m_bindSpin->setValue(62);
    bindRow->addWidget(m_bindSpin);
    m_bindApply = new QPushButton(QStringLiteral("Bind"), m_changeBox);
    bindRow->addWidget(m_bindApply);
    grid->addLayout(bindRow);

    // Scale: collection rotations and catalogue cycles.
    auto* scaleRow = new QHBoxLayout();
    scaleRow->addWidget(new QLabel(QStringLiteral("Scale"), m_changeBox));
    m_scaleCombo = new QComboBox(m_changeBox);
    scaleRow->addWidget(m_scaleCombo, 1);
    auto* scaleApply = new QPushButton(QStringLiteral("Apply"), m_changeBox);
    scaleRow->addWidget(scaleApply);
    grid->addLayout(scaleRow);

    auto* bottomRow = new QHBoxLayout();
    m_removeButton = new QPushButton(QStringLiteral("Remove change at this bar"), m_changeBox);
    bottomRow->addWidget(m_removeButton);
    auto* refresh = new QPushButton(QStringLiteral("Refresh"), m_changeBox);
    bottomRow->addWidget(refresh);
    grid->addLayout(bottomRow);

    m_statusLabel = new QLabel(QString(), m_changeBox);
    m_statusLabel->setWordWrap(true);
    m_statusLabel->setStyleSheet(QStringLiteral("color: gray; font-size: 9pt;"));
    grid->addWidget(m_statusLabel);

    outer->addWidget(m_changeBox);

    connect(modeApply, &QPushButton::clicked, this, [this]() {
        const int i = m_tonicCombo->currentIndex();
        if (i >= 0 && i < int(m_tonicIds.size())) {
            applyChoice(m_tonicIds[i]);
        }
    });
    connect(m_keyApply, &QPushButton::clicked, this, [this]() {
        const int i = m_keyClassCombo->currentIndex();
        if (i >= 0 && i < int(m_keyClassNGens.size())) {
            applyChoice(muse::String(u"key:%1:%2").arg(m_keyOctaveSpin->value()).arg(m_keyClassNGens[i]));
        }
    });
    connect(m_bindApply, &QPushButton::clicked, this, [this]() {
        applyChoice(muse::String(u"bind:reference-pitch:%1").arg(m_bindSpin->value()));
    });
    connect(scaleApply, &QPushButton::clicked, this, [this]() {
        const int i = m_scaleCombo->currentIndex();
        if (i >= 0 && i < int(m_scaleSteps.size())) {
            for (const muse::String& id : m_scaleSteps[i]) {
                applyChoice(id);
            }
        }
    });
    connect(m_removeButton, &QPushButton::clicked, this, &JimsTuningPanel::onRemoveChange);
    connect(refresh, &QPushButton::clicked, this, &JimsTuningPanel::syncChangeSection);
}

bool JimsTuningPanel::changeTarget(Measure*& measure, staff_idx_t& staffIdx) const
{
    measure = nullptr;
    staffIdx = 0;
    if (!m_score) {
        return false;
    }
    const Selection& sel = m_score->selection();
    if (sel.isRange() && sel.startSegment()) {
        measure = sel.startSegment()->measure();
        staffIdx = sel.staffStart();
    } else if (EngravingItem* e = sel.element()) {
        measure = e->findMeasure();
        staffIdx = e->staffIdx();
    }
    return measure != nullptr;
}

void JimsTuningPanel::syncChangeSection()
{
    if (!m_changeBox) {
        return;
    }
    Measure* measure = nullptr;
    staff_idx_t staffIdx = 0;
    const bool haveTarget = changeTarget(measure, staffIdx);
    jims::StateChangeOptions options;
    const bool ok = haveTarget && jims::changeOptions(m_score, staffIdx, measure, options);
    m_changeBox->setEnabled(ok);
    if (!ok) {
        m_targetLabel->setText(haveTarget
                               ? QStringLiteral("The selected staff is not a JiMStaff")
                               : QStringLiteral("Select a note or bar on the JiMStaff"));
        return;
    }
    const bool hasCarrier = jims::changeCarrier(measure, staffIdx) != nullptr;
    QString carrierNote;
    if (hasCarrier && !measure->tick().isZero()) {
        // Say what the Kernel derives from (state before, state here): the
        // indicator kinds, or its reason when nothing can be drawn — a
        // change that renders nothing must never be silent.
        const Staff* staff = m_score->staff(staffIdx);
        const StaffType* before = staff->staffType(Fraction::fromTicks(measure->tick().ticks() - 1));
        const StaffType* here = staff->staffType(measure->tick());
        jims::ChangeIndicator ind;
        muse::String why;
        if (before && here && before->isJiMS() && here->isJiMS()
            && jims::changeIndicator(before->jimsStateJson(), here->jimsStateJson(), ind, &why)) {
            QStringList kinds;
            for (const muse::String& k : ind.kinds) {
                kinds << k.toQString();
            }
            carrierNote = kinds.isEmpty()
                          ? QStringLiteral(" — carries a change (no indicator: states differ only in metadata)")
                          : QStringLiteral(" — carries a change (%1)").arg(kinds.join(QStringLiteral(", ")));
        } else {
            carrierNote = QStringLiteral(" — carries a change (no indicator: %1)").arg(why.toQString());
        }
    } else if (hasCarrier) {
        carrierNote = QStringLiteral(" — carries a change");
    }
    m_targetLabel->setText(QStringLiteral("Bar %1, staff %2%3")
                           .arg(measure->no() + 1).arg(int(staffIdx) + 1).arg(carrierNote));

    QSignalBlocker b1(m_tonicCombo);
    m_tonicCombo->clear();
    m_tonicIds.clear();
    int currentTonic = 0;
    for (size_t i = 0; i < options.tonics.size(); ++i) {
        m_tonicCombo->addItem(options.tonics[i].label.toQString());
        m_tonicIds.push_back(options.tonics[i].id);
        if (options.tonics[i].current) {
            currentTonic = int(i);
        }
    }
    m_tonicCombo->setCurrentIndex(currentTonic);

    QSignalBlocker b2(m_keyClassCombo);
    m_keyClassCombo->clear();
    m_keyClassNGens.clear();
    // One entry per note class (the Kernel lists every class for each
    // octave; the octave spin picks N).
    for (const jims::StateChangeOption& k : options.keyTargets) {
        if (k.nPer != 0) {
            continue;
        }
        m_keyClassCombo->addItem(k.label.toQString());
        m_keyClassNGens.push_back(k.nGen);
    }
    for (int i = 0; i < int(m_keyClassNGens.size()); ++i) {
        if (m_keyClassNGens[i] == 3) {           // La (Do-relative: Do+3 generators) — the owner's worked example
            m_keyClassCombo->setCurrentIndex(i);
        }
    }
    m_keyApply->setEnabled(options.referenceBound);
    m_keyApply->setToolTip(options.referenceBound ? QString()
                           : QStringLiteral("Bind Re0 to a reference pitch first"));
    m_bindApply->setEnabled(!options.referenceBound);
    m_bindSpin->setEnabled(!options.referenceBound);

    QSignalBlocker b3(m_scaleCombo);
    m_scaleCombo->clear();
    m_scaleIds.clear();
    m_scaleSteps.clear();
    int currentScale = 0;
    // The panel offers exactly the JiMS scales (owner ruling 2026-08-17:
    // Diatonic, Parallel Minor — the Grey notes — and Harmonic Minor), each
    // as the Kernel-issued choice(s) that reach it; the Kernel's full
    // option list is not a menu. Members beside each are Kernel labels.
    const jims::StateChangeOption* rotation0 = nullptr;
    const jims::StateChangeOption* rotationMinus3 = nullptr;
    for (const jims::StateChangeOption& r : options.rotations) {
        if (r.id == u"scale:rotation:0") {
            rotation0 = &r;
        } else if (r.id == u"scale:rotation:-3") {
            rotationMinus3 = &r;
        }
    }
    const jims::StateChangeOption* diatonicCycle = nullptr;
    const jims::StateChangeOption* harmonicMinorCycle = nullptr;
    for (const jims::StateChangeOption& c : options.cycles) {
        if (c.id == u"scale:cycle:diatonic") {
            diatonicCycle = &c;
        } else if (c.id == u"scale:cycle:harmonic-minor") {
            harmonicMinorCycle = &c;
        }
    }
    const bool onDiatonic = diatonicCycle && diatonicCycle->current;
    auto membersOf = [](const jims::StateChangeOption* o) {
        QStringList members;
        if (o) {
            for (const muse::String& m : o->memberLabels) {
                members << m.toQString();
            }
        }
        return members.join(' ');
    };
    if (diatonicCycle && rotation0) {
        // Diatonic (White notes): the diatonic cycle at rotation 0.
        m_scaleCombo->addItem(QStringLiteral("Diatonic (White notes): %1").arg(membersOf(rotation0)));
        std::vector<muse::String> steps;
        if (!onDiatonic) {
            steps.push_back(diatonicCycle->id);
        }
        steps.push_back(rotation0->id);
        m_scaleIds.push_back(rotation0->id);
        m_scaleSteps.push_back(steps);
        if (onDiatonic && rotation0->current) {
            currentScale = int(m_scaleIds.size()) - 1;
        }
    }
    if (diatonicCycle && rotationMinus3) {
        // Parallel Minor (Grey notes): the diatonic collection rotated -3,
        // tonic staying on Do.
        m_scaleCombo->addItem(QStringLiteral("Parallel Minor (Grey notes): %1").arg(membersOf(rotationMinus3)));
        std::vector<muse::String> steps;
        if (!onDiatonic) {
            steps.push_back(diatonicCycle->id);
        }
        steps.push_back(rotationMinus3->id);
        m_scaleIds.push_back(rotationMinus3->id);
        m_scaleSteps.push_back(steps);
        if (onDiatonic && rotationMinus3->current) {
            currentScale = int(m_scaleIds.size()) - 1;
        }
    }
    if (harmonicMinorCycle) {
        m_scaleCombo->addItem(QStringLiteral("Harmonic Minor: %1").arg(membersOf(harmonicMinorCycle)));
        m_scaleIds.push_back(harmonicMinorCycle->id);
        m_scaleSteps.push_back({ harmonicMinorCycle->id });
        if (harmonicMinorCycle->current) {
            currentScale = int(m_scaleIds.size()) - 1;
        }
    }
    m_scaleCombo->setCurrentIndex(currentScale);
    m_removeButton->setEnabled(hasCarrier);
    muse::String why;
    if (!jims::canInsertChange(m_score, staffIdx, measure, why)) {
        m_statusLabel->setText(why.toQString());
    }
}

void JimsTuningPanel::applyChoice(const muse::String& choiceId)
{
    Measure* measure = nullptr;
    staff_idx_t staffIdx = 0;
    if (!changeTarget(measure, staffIdx)) {
        return;
    }
    muse::String error;
    if (!jims::applyChange(m_score, staffIdx, measure, choiceId, error)) {
        m_statusLabel->setText(error.toQString());
        return;
    }
    m_statusLabel->setText(QStringLiteral("Applied %1").arg(choiceId.toQString()));
    if (m_refreshView) {
        m_refreshView();
    }
    syncChangeSection();
}

void JimsTuningPanel::onRemoveChange()
{
    Measure* measure = nullptr;
    staff_idx_t staffIdx = 0;
    if (!changeTarget(measure, staffIdx)) {
        return;
    }
    muse::String error;
    if (!jims::removeChange(m_score, staffIdx, measure, error)) {
        m_statusLabel->setText(error.toQString());
        return;
    }
    m_statusLabel->setText(QStringLiteral("Change removed"));
    if (m_refreshView) {
        m_refreshView();
    }
    syncChangeSection();
}

bool JimsTuningPanel::event(QEvent* e)
{
    // Re-read the selection whenever the panel comes to the front.
    if (e->type() == QEvent::WindowActivate || e->type() == QEvent::Show) {
        syncChangeSection();
    }
    return QWidget::event(e);
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
    syncChangeSection();
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
