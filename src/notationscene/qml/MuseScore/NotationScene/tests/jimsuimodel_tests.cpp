// SPDX-License-Identifier: GPL-3.0-only
// MuseScore-Studio-CLA-applies
#include <gtest/gtest.h>
#include "context/internal/globalcontext.h"
#include "engraving/tests/utils/scorerw.h"
#include "engraving/dom/masterscore.h"
#include "engraving/dom/measure.h"
#include "engraving/dom/segment.h"
#include "engraving/dom/staff.h"
#include "engraving/dom/stafftype.h"
#include "engraving/editing/undo.h"
#include "engraving/jims/jimschangecontroller.h"
#include "inspector/internal/elementrepositoryservice.h"
#include "inspector/qml/MuseScore/Inspector/jimsstaffsettingsmodel.h"
#include "inspector/qml/MuseScore/Inspector/jimsscoresettingsmodel.h"
#include "inspector/qml/MuseScore/Inspector/jimstuningmodel.h"
using namespace mu;
using namespace mu::engraving;
using namespace mu::inspector;
namespace {
class TestAnnouncements : public muse::accessibility::IAccessibilityController
{
public:
    void reg(muse::accessibility::IAccessible*) override {}
    void unreg(muse::accessibility::IAccessible*) override {}
    bool isReg(muse::accessibility::IAccessible*) const override { return false; }
    void announce(const QString& text) override { messages << text; }
    QString announcement() const override { return messages.isEmpty() ? QString() : messages.last(); }
    const muse::accessibility::IAccessible* accessibleRoot() const override { return nullptr; }
    const muse::accessibility::IAccessible* lastFocused() const override { return nullptr; }
    bool needToVoicePanelInfo() const override { return false; }
    QString currentPanelAccessibleName() const override { return {}; }
    void setIgnoreQtAccessibilityEvents(bool) override {}
    QStringList messages;
};
class TestElements : public notation::INotationElements
{
public:
    explicit TestElements(Score* score)
        : m_score(score) {}
    Score* msScore() const override { return m_score; }
    std::vector<EngravingItem*> search(const QString&) const override { return {}; }
    std::vector<EngravingItem*> elements(const notation::FilterElementsOptions&) const override { return {}; }
    Measure* measure(int) const override { return m_score->firstMeasure(); }
    const notation::PageList& pages() const override { return m_score->pages(); }
    const Page* pageByPoint(const muse::PointF&) const override { return nullptr; }
private:
    Score* m_score;
};
class TestNotation : public notation::INotation
{
public:
    explicit TestNotation(Score* score)
        : m_elements(std::make_shared<TestElements>(score)) {}
    project::INotationProject* project() const override { return nullptr; }
    notation::IMasterNotationPtr masterNotation() const override { return nullptr; }
    QString name() const override { return {}; }
    QString projectName() const override { return {}; }
    QString projectNameAndPartName() const override { return {}; }
    QString workTitle() const override { return {}; }
    QString projectWorkTitle() const override { return {}; }
    QString projectWorkTitleAndPartName() const override { return {}; }
    bool isOpen() const override { return true; }
    void setIsOpen(bool) override {}
    muse::async::Notification openChanged() const override { return {}; }
    bool hasVisibleParts() const override { return true; }
    bool isMaster() const override { return true; }
    notation::ViewMode viewMode() const override { return {}; }
    void setViewMode(const notation::ViewMode&) override {}
    muse::async::Notification viewModeChanged() const override { return {}; }
    notation::INotationPaintingPtr painting() const override { return nullptr; }
    notation::INotationViewStatePtr viewState() const override { return nullptr; }
    notation::INotationSoloMuteStatePtr soloMuteState() const override { return nullptr; }
    notation::INotationInteractionPtr interaction() const override { return nullptr; }
    notation::INotationMidiInputPtr midiInput() const override { return nullptr; }
    notation::INotationUndoStackPtr undoStack() const override { return nullptr; }
    notation::INotationStylePtr style() const override { return nullptr; }
    notation::INotationElementsPtr elements() const override { return m_elements; }
    notation::INotationAccessibilityPtr accessibility() const override { return nullptr; }
    notation::INotationPartsPtr parts() const override { return nullptr; }
    muse::async::Notification notationChanged() const override { return m_changed; }
private:
    std::shared_ptr<TestElements> m_elements;
    muse::async::Notification m_changed;
};
}
class JimsUiModelTests : public ::testing::Test
{
protected:
    void SetUp() override
    {
        score.reset(ScoreRW::readScore(muse::String::fromUtf8(JIMS_UI_TEST_DATA_ROOT) + u"/jimstaff_data/mode-change.mscx", true));
        ASSERT_TRUE(score);
        global = std::make_shared<context::GlobalContext>();
        notation = std::make_shared<TestNotation>(score.get());
        global->setCurrentNotation(notation);
    }

    EngravingItem* selectMeasure(int index)
    {
        Measure* measure = score->firstMeasure();
        while (index-- && measure) {
            measure = measure->nextMeasure();
        }
        EXPECT_TRUE(measure);
        EngravingItem* item = measure->first(SegmentType::ChordRest)->element(0);
        EXPECT_TRUE(item);
        score->select(item, SelectType::SINGLE);
        repository.updateElementList({ item }, SelState::LIST);
        return item;
    }

    std::unique_ptr<MasterScore> score;
    std::shared_ptr<context::GlobalContext> global;
    std::shared_ptr<TestNotation> notation;
    ElementRepositoryService repository;
};
TEST_F(JimsUiModelTests, StaffSectionsAreRelevantOnlyForCompatibleSelection) {
    auto* selected = selectMeasure(0);
    ElementKeySet keys { AbstractInspectorModel::makeKey(selected) };
    auto sections = AbstractInspectorModel::sectionTypesByElementKeys(keys, false, { selected });
    EXPECT_TRUE(sections.count(InspectorSectionType::SECTION_JIMS_STAFF));
    EXPECT_TRUE(sections.count(InspectorSectionType::SECTION_JIMS_SCORE));
    EXPECT_TRUE(AbstractInspectorModel::sectionTypesByElementKeys({}, false, {}).empty());
    StaffType original = *score->staff(0)->staffType(Fraction(0, 1));
    *score->staff(0)->staffType(Fraction(0, 1)) = *StaffType::preset(StaffTypes::STANDARD);
    sections = AbstractInspectorModel::sectionTypesByElementKeys(keys, false, { selected });
    EXPECT_FALSE(sections.count(InspectorSectionType::SECTION_JIMS_STAFF));
    *score->staff(0)->staffType(Fraction(0, 1)) = original;
}
TEST_F(JimsUiModelTests, StaffPresentationIsUndoableAndPreservesMusicalState) {
    selectMeasure(0);
    JimsStaffSettingsModel model(nullptr, muse::modularity::globalCtx(), &repository);
    model.context.set(global);
    model.loadProperties();
    ASSERT_TRUE(model.settings()["available"].toBool());
    auto before = score->staff(0)->staffType(Fraction(0, 1))->jimsStateJson();
    int undo = score->undoStack()->size();
    model.setStaffOption("labels", 3);
    EXPECT_EQ(score->undoStack()->size(), undo + 1);
    EXPECT_EQ(score->staff(0)->staffType(Fraction(0, 1))->jimsScaleDotLabelMode(), JimsScaleDotLabelMode::Split);
    EXPECT_EQ(score->staff(0)->staffType(Fraction(0, 1))->jimsStateJson(), before);
    model.setStaffOption("labels", 3);
    EXPECT_EQ(score->undoStack()->size(), undo + 1);
    score->undoRedo(true, nullptr);
    model.loadProperties();
    EXPECT_EQ(model.settings()["labels"].toInt(), 0);
}
TEST_F(JimsUiModelTests, SelectionRefreshClearsOldFeedbackAndChangesTheTarget) {
    selectMeasure(0);
    JimsStaffSettingsModel model(nullptr, muse::modularity::globalCtx(), &repository);
    model.context.set(global);
    model.loadProperties();
    auto first = model.settings()["target"].toString();
    model.bindReference("not a number");
    EXPECT_FALSE(model.status().isEmpty());
    selectMeasure(4);
    model.loadProperties();
    EXPECT_TRUE(model.status().isEmpty());
    EXPECT_NE(model.settings()["target"].toString(), first);
    EXPECT_TRUE(model.settings()["hasChange"].toBool());
    EXPECT_FALSE(model.settings()["indicator"].toString().isEmpty());
}
TEST_F(JimsUiModelTests, KeyChoicesDistinguishEveryPeriod) {
    selectMeasure(0);
    JimsStaffSettingsModel model(nullptr, muse::modularity::globalCtx(), &repository);
    model.context.set(global);
    model.loadProperties();
    const auto choices = model.settings()["keys"].toList();
    ASSERT_FALSE(choices.empty());
    QSet<QString> labels;
    for (const auto& choice : choices) {
        const QString label = choice.toMap()["text"].toString();
        EXPECT_FALSE(labels.contains(label)) << label.toStdString();
        labels.insert(label);
    }
    EXPECT_EQ(labels.size(), choices.size());
}
TEST_F(JimsUiModelTests, ScaleChoiceReconcilesOtherPartsAndReportsTheMutation) {
    global->setCurrentNotation(nullptr);
    score.reset(ScoreRW::readScore(muse::String::fromUtf8(JIMS_UI_TEST_DATA_ROOT) + u"/jimstaff_data/m9-satb-hymn.mscx", true));
    ASSERT_TRUE(score);
    ASSERT_EQ(score->nstaves(), 4);
    notation = std::make_shared<TestNotation>(score.get());
    global->setCurrentNotation(notation);
    selectMeasure(1);
    Measure* measure = score->firstMeasure()->nextMeasure();
    muse::String error;
    ASSERT_TRUE(jims::applyChangeToAllJimsParts(score.get(), measure, { u"scale:cycle:double-harmonic-minor" },
                                                error)) << error.toStdString();
    ASSERT_TRUE(jims::removeChange(score.get(), 0, measure, error)) << error.toStdString();
    JimsStaffSettingsModel model(nullptr, muse::modularity::globalCtx(), &repository);
    model.context.set(global);
    model.loadProperties();
    ASSERT_EQ(model.settings()["scalesIndex"].toInt(), 0);
    const int undo = score->undoStack()->size();
    model.applyOption("scales", 0);
    ASSERT_FALSE(model.hasError()) << model.status().toStdString();
    EXPECT_EQ(score->undoStack()->size(), undo + 1);
    EXPECT_FALSE(model.status().contains("no change"));
    for (staff_idx_t staff = 0; staff < 4; ++staff) {
        jims::StateChangeOptions options;
        ASSERT_TRUE(jims::changeOptions(score.get(), staff, measure, options));
        for (const auto& cycle : options.cycles) {
            if (cycle.id == u"scale:cycle:diatonic") {
                EXPECT_TRUE(cycle.current) << staff;
            }
        }
    }
    score->undoRedo(true, nullptr);
    jims::StateChangeOptions options;
    ASSERT_TRUE(jims::changeOptions(score.get(), 1, measure, options));
    for (const auto& cycle : options.cycles) {
        if (cycle.id == u"scale:cycle:double-harmonic-minor") {
            EXPECT_TRUE(cycle.current);
        }
    }
}
TEST_F(JimsUiModelTests, TuningRejectsInvalidAndNoOpGesturesWithoutUndoEntries) {
    JimsTuningModel model;
    model.context.set(global);
    model.init();
    ASSERT_TRUE(model.available());
    double original = model.cents();
    int undo = score->undoStack()->size();
    model.acceptText("invalid");
    EXPECT_FALSE(model.error().isEmpty());
    model.commit(model.maximum() + 1);
    EXPECT_EQ(model.cents(), original);
    ASSERT_TRUE(model.beginPreview());
    model.commit(original);
    EXPECT_EQ(score->undoStack()->size(), undo);
    ASSERT_TRUE(model.beginPreview());
    model.preview(690.0);
    model.cancel();
    EXPECT_NEAR(model.cents(), original, 1e-8);
    EXPECT_EQ(score->undoStack()->size(), undo);
}
TEST_F(JimsUiModelTests, TuningAnnouncesEachRefusalOnceAndRefreshesSilently) {
    auto announcements = std::make_shared<TestAnnouncements>();
    JimsTuningModel model;
    model.context.set(global);
    model.accessibilityController.set(announcements);
    model.init();
    model.acceptText("730");
    ASSERT_EQ(announcements->messages.size(), 1);
    EXPECT_EQ(announcements->messages.last(), model.error());
    notation->notationChanged().notify();
    EXPECT_EQ(announcements->messages.size(), 1);
    model.acceptText("730");
    EXPECT_EQ(announcements->messages.size(), 2);
    model.acceptText("690");
    EXPECT_TRUE(model.error().isEmpty());
    EXPECT_EQ(announcements->messages.size(), 2);
}
TEST_F(JimsUiModelTests, TuningCommitsOnceAndCancelsWhenSwitchingScores) {
    JimsTuningModel model;
    model.context.set(global);
    model.init();
    const double original = model.cents();
    int undo = score->undoStack()->size();
    model.commit(690.0);
    EXPECT_EQ(score->undoStack()->size(), undo + 1);
    EXPECT_NEAR(model.cents(), 690.0, 1e-8);
    score->undoRedo(true, nullptr);
    notation->notationChanged().notify();
    EXPECT_NEAR(model.cents(), original, 1e-8);
    model.preview(690.0);
    global->setCurrentNotation(nullptr);
    EXPECT_FALSE(model.available());
    double cents = 0, period = 0;
    ASSERT_TRUE(jims::staffMetrics(score->staff(0)->staffType(Fraction(0, 1))->jimsStateJson(), cents, period));
    EXPECT_NEAR(cents, original, 1e-8);
}
TEST_F(JimsUiModelTests, ScorePresentationDoesNotChangeMusicalStateAndUndoes) {
    selectMeasure(0);
    JimsScoreSettingsModel model(nullptr, muse::modularity::globalCtx(), &repository);
    model.context.set(global);
    model.loadProperties();
    auto state = score->staff(0)->staffType(Fraction(0, 1))->jimsStateJson();
    bool original = model.settings()["elide"].toBool();
    int undo = score->undoStack()->size();
    model.setOption("elide", !original);
    EXPECT_EQ(score->undoStack()->size(), undo + 1);
    EXPECT_EQ(score->staff(0)->staffType(Fraction(0, 1))->jimsStateJson(), state);
    score->undoRedo(true, nullptr);
    model.loadProperties();
    EXPECT_EQ(model.settings()["elide"].toBool(), original);
}
