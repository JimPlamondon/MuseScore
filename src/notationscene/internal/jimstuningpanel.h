/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 *
 * JiMStaff Milestone 3 — the continuous-tuning panel (owner decision
 * 1a, 2026-08-14): a slider and an editable numeric generator_cents
 * field, nothing else. Both input paths route through the shared
 * jims::TuningController seam; dragging previews transiently, release
 * or numeric-entry acceptance commits exactly one undoable edit.
 */
#ifndef MU_NOTATIONSCENE_JIMSTUNINGPANEL_H
#define MU_NOTATIONSCENE_JIMSTUNINGPANEL_H

#include <functional>
#include <memory>

#include <QWidget>

class QSlider;
class QDoubleSpinBox;

namespace mu::engraving {
class Score;
namespace jims {
class TuningController;
}
}

namespace mu::notationscene {
class JimsTuningPanel : public QWidget
{
    Q_OBJECT

public:
    JimsTuningPanel(mu::engraving::Score* score, std::function<void()> refreshView,
                    QWidget* parent = nullptr);
    ~JimsTuningPanel() override;

private:
    void onSliderPressed();
    void onSliderMoved(int value);
    void onSliderReleased();
    void onSpinAccepted();
    void syncFromScore();

    static constexpr double MIN_CENTS = 600.0;
    static constexpr double MAX_CENTS = 800.0;
    static constexpr double SLIDER_STEP = 0.1;

    std::unique_ptr<mu::engraving::jims::TuningController> m_controller;
    std::function<void()> m_refreshView;
    QSlider* m_slider = nullptr;
    QDoubleSpinBox* m_spin = nullptr;
    bool m_dragging = false;
};
}

#endif
