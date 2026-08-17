/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 *
 * JiMStaff Milestone 3 — the continuous-tuning panel (owner decision
 * 1a, 2026-08-14): a slider and an editable numeric generator_cents
 * field, nothing else. Owner correction 2026-08-15: the slider is
 * vertical (high at top) inside a painted scale that mirrors the
 * rank-2 temperament continuum figure (see jimstuningpanel.cpp). Both input paths route through the shared
 * jims::TuningController seam; dragging previews transiently, release
 * or numeric-entry acceptance commits exactly one undoable edit.
 */
#ifndef MU_NOTATIONSCENE_JIMSTUNINGPANEL_H
#define MU_NOTATIONSCENE_JIMSTUNINGPANEL_H

#include <functional>
#include <memory>

#include <QWidget>
#include <vector>

#include "engraving/types/types.h"
#include "types/string.h"

#include "async/asyncable.h"
#include "async/notification.h"

class QSlider;
class QDoubleSpinBox;
class QComboBox;
class QSpinBox;
class QLabel;
class QPushButton;
class QGroupBox;
class QVBoxLayout;

namespace mu::engraving {
class Score;
class Measure;
namespace jims {
class TuningController;
}
}

namespace mu::notationscene {
class JimsTuningPanel : public QWidget, public muse::async::Asyncable
{
    Q_OBJECT

public:
    JimsTuningPanel(mu::engraving::Score* score,
                    std::function<void()> refreshView, muse::async::Notification scoreChanged, QWidget* parent = nullptr);
    ~JimsTuningPanel() override;

private:
    void onSliderPressed();
    void onSliderMoved(int value);
    void onSliderReleased();
    void onSpinAccepted();
    void syncFromScore();

    // JiMStaff Milestone 6 (owner decision 1a, 2026-08-16): the change
    // section — mode / key / scale changes at the selected measure, every
    // choice a Kernel-issued option applied through jims::applyChange.
    void buildChangeSection(QWidget* parent, QVBoxLayout* outer);
    void syncChangeSection();
    bool changeTarget(mu::engraving::Measure*& measure, mu::engraving::staff_idx_t& staffIdx) const;
    void applyChoice(const muse::String& choiceId);
    void onRemoveChange();
    bool event(QEvent* e) override;

    static constexpr double SLIDER_STEP = 0.1;
    double m_minCents = 686.0;   // overwritten by the Kernel's range
    double m_maxCents = 720.0;

    std::unique_ptr<mu::engraving::jims::TuningController> m_controller;
    std::function<void()> m_refreshView;
    QSlider* m_slider = nullptr;
    QDoubleSpinBox* m_spin = nullptr;
    bool m_dragging = false;

    mu::engraving::Score* m_score = nullptr;
    QGroupBox* m_changeBox = nullptr;
    QLabel* m_targetLabel = nullptr;
    QComboBox* m_tonicCombo = nullptr;
    QComboBox* m_keyClassCombo = nullptr;
    QSpinBox* m_keyOctaveSpin = nullptr;
    QPushButton* m_keyApply = nullptr;
    QSpinBox* m_bindSpin = nullptr;
    QPushButton* m_bindApply = nullptr;
    QComboBox* m_scaleCombo = nullptr;
    QPushButton* m_removeButton = nullptr;
    QLabel* m_statusLabel = nullptr;
    std::vector<muse::String> m_tonicIds;
    std::vector<muse::String> m_keyClassIds;   // "key:<nPer>:<nGen>" for the chosen octave is built at apply time
    std::vector<int> m_keyClassNGens;
    std::vector<muse::String> m_scaleIds;
    std::vector<std::vector<muse::String> > m_scaleSteps;   // Kernel choice ids applied in order for each entry
};
}

#endif
