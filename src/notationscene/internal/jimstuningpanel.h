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
class QCheckBox;

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
                    std::function<void()> refreshView, muse::async::Notification scoreChanged, muse::async::Notification selectionChanged,
                    QWidget* parent = nullptr);
    ~JimsTuningPanel() override;

private:
    void onSliderPressed();
    void onSliderMoved(int value);
    void onSliderReleased();
    void onSpinAccepted();
    void syncFromScore();

    // JiMStaff Milestone 6 (owner decision 1a, 2026-08-16): the change
    // section — mode / key / scale changes at the selected score position, every
    // choice a Kernel-issued option applied through jims::applyChange.
    void buildChangeSection(QWidget* parent, QVBoxLayout* outer);
    void syncChangeSection();
    bool changeTarget(mu::engraving::Measure*& measure, mu::engraving::Fraction& tick, mu::engraving::staff_idx_t& staffIdx) const;
    /// One staff, one Kernel choice. Reference binding only (owner decision 9):
    /// a reference names what THIS staff's Re0 is, so it stays staff-wide.
    void applyChoice(const muse::String& choiceId);
    /// Tonic, key and scale: the whole gesture, applied to every JiMS part of
    /// the score at the selected position as one undo step (owner decision 2a).
    /// A scale entry is several Kernel ids, which is why this takes a list.
    void applyChoices(const std::vector<muse::String>& choiceIds);
    void onMelodyPartChanged(int index);
    void onRemoveChange();
    bool event(QEvent* e) override;

    // JiMStaff Milestone 8 (octave-band elision, owner-approved plan
    // 2026-08-18): two score-wide switches (MuseScore's hide-empty-staves
    // shape) and the selected staff's Auto/On/Off override. Presentation
    // only: undoable style/staff-type edits that trigger a relayout, never
    // touching the Kernel state.
    void buildElisionSection(QWidget* parent, QVBoxLayout* outer);
    void syncElisionSection();
    void onElideToggled(bool on);
    void onFirstSystemToggled(bool on);
    void onStaffOverrideChanged(int index);

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
    QComboBox* m_melodyPartCombo = nullptr;
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

    QGroupBox* m_elisionBox = nullptr;
    QCheckBox* m_elideCheck = nullptr;
    QCheckBox* m_firstSystemCheck = nullptr;
    QComboBox* m_staffOverrideCombo = nullptr;
    QLabel* m_elisionTargetLabel = nullptr;
};
}

#endif
