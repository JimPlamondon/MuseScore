// SPDX-License-Identifier: GPL-3.0-only
// MuseScore-Studio-CLA-applies
#include <vector>

#include <gtest/gtest.h>
#include <QtTest/QTest>
#include <QQuickWindow>
#include <QQuickItem>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QCoreApplication>
#include "context/internal/globalcontext.h"
#include "engraving/tests/utils/scorerw.h"
#include "engraving/dom/masterscore.h"
#include "engraving/dom/chord.h"
#include "engraving/dom/note.h"
#include "engraving/dom/harmony.h"
#include "engraving/dom/measure.h"
#include "engraving/dom/segment.h"
#include "engraving/dom/staff.h"
#include "engraving/dom/stafftype.h"
#include "engraving/editing/undo.h"
#include "engraving/melo/melochangecontroller.h"
#include "inspector/internal/elementrepositoryservice.h"
#include "inspector/qml/MuseScore/Inspector/melostaffsettingsmodel.h"
#include "inspector/qml/MuseScore/Inspector/meloscoresettingsmodel.h"
#include "inspector/qml/MuseScore/Inspector/melotuningmodel.h"
#include "inspector/qml/MuseScore/Inspector/notation/chordsymbols/chordsymbolsettingsmodel.h"
#include "notation/tests/mocks/notationinteractionmock.h"
#include "playback/tests/mocks/playbackcontrollermock.h"
#include "notationscene/qml/MuseScore/NotationScene/noteinputbarmodel.h"
#include "ui/qml/Muse/Ui/navigationpanel.h"
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
    notation::INotationInteractionPtr interaction() const override { return testInteraction; }
    notation::INotationInteractionPtr testInteraction;
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
class MeloUiModelTests : public ::testing::Test
{
protected:
    void SetUp() override
    {
        score.reset(ScoreRW::readScore(muse::String::fromUtf8(MELO_UI_TEST_DATA_ROOT) + u"/jimstaff_data/mode-change.mscx", true));
        ASSERT_TRUE(score);
        global = std::make_shared<context::GlobalContext>();
        notation = std::make_shared<TestNotation>(score.get());
        playback = std::make_shared<testing::NiceMock<playback::PlaybackControllerMock> >();
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
    std::shared_ptr<testing::NiceMock<playback::PlaybackControllerMock> > playback;
    ElementRepositoryService repository;
};
TEST_F(MeloUiModelTests, MeloChordCursorControlsUseExactDurationsWithoutEditingNotes) {
    selectMeasure(0);
    auto* segment = score->firstMeasure()->first(SegmentType::ChordRest);
    auto* harmony = new Harmony(segment);
    harmony->setTrack(0);
    harmony->setHarmonyType(HarmonyType::MELO);
    harmony->setHarmony(u"Do5");
    segment->add(harmony);
    repository.updateElementList({ harmony }, SelState::LIST);
    auto interaction = std::make_shared<testing::NiceMock<notation::NotationInteractionMock> >();
    notation->testInteraction = interaction;
    class CursorModel : public ChordSymbolSettingsModel
    {
    public:
        using ChordSymbolSettingsModel::ChordSymbolSettingsModel;
        void useHarmony(Harmony* item) { m_elementList = { item }; }
    };
    CursorModel model(nullptr, muse::modularity::globalCtx(), &repository);
    model.context.set(global);
    model.useHarmony(harmony);
    ASSERT_TRUE(model.hasMeloSelection());
    QQmlEngine engine;
    engine.addImportPath("qrc:/qt/qml");
    QQmlComponent component(&engine, QUrl("qrc:/qt/qml/MuseScore/Inspector/notation/chordsymbols/ChordSymbolSettings.qml"));
    ASSERT_TRUE(component.isReady()) << component.errorString().toStdString();
    ASSERT_GE(model.metaObject()->indexOfMethod("advanceMeloChordCursor(int)"), 0);
    ON_CALL(*interaction, isTextEditingStarted()).WillByDefault(testing::Return(false));
    EXPECT_CALL(*interaction, startEditText(harmony, testing::_)).Times(2);
    EXPECT_CALL(*interaction, navigateToHarmony(Fraction(1, 4))).Times(1);
    EXPECT_CALL(*interaction, navigateToHarmony(Fraction(1, 8))).Times(1);
    ASSERT_TRUE(QMetaObject::invokeMethod(&model, "advanceMeloChordCursor", Q_ARG(int, 4)));
    ASSERT_TRUE(QMetaObject::invokeMethod(&model, "advanceMeloChordCursor", Q_ARG(int, 8)));
    ASSERT_TRUE(QMetaObject::invokeMethod(&model, "advanceMeloChordCursor", Q_ARG(int, 0)));
    ASSERT_TRUE(QMetaObject::invokeMethod(&model, "advanceMeloChordCursor", Q_ARG(int, 3)));
}
TEST_F(MeloUiModelTests, MeloAccidentalPickersUseTheMatchingNoteheads) {
    selectMeasure(0);
    const auto presentation = notation::NoteInputBarModel::accidentalPresentationForScore(score.get(), score->engravingFont());
    const std::pair<const char*, NoteHeadGroup> cases[] = {
        { "sharp", NoteHeadGroup::HEAD_TRIANGLE_UP },
        { "flat", NoteHeadGroup::HEAD_TRIANGLE_DOWN },
        { "sharp2", NoteHeadGroup::HEAD_DIAMOND },
        { "flat2", NoteHeadGroup::HEAD_LA },
    };
    auto font = score->engravingFont();
    for (const auto& [action, group] : cases) {
        const auto item = presentation.value(action).toMap();
        EXPECT_EQ(item.value("icon").toUInt(), font->symCode(Note::noteHead(0, group, NoteHeadType::HEAD_QUARTER))) << action;
        EXPECT_FALSE(item.value("title").toString().isEmpty()) << action;
    }
    EXPECT_FALSE(presentation.contains("nat"));
    EXPECT_FALSE(presentation.contains("quarter"));
}

TEST_F(MeloUiModelTests, AccidentalPickersFollowSelectionAndInputStaffType) {
    selectMeasure(0);
    ASSERT_FALSE(notation::NoteInputBarModel::accidentalPresentationForScore(score.get(), score->engravingFont()).isEmpty());
    StaffType* type = score->staff(0)->staffType(Fraction(0, 1));
    const StaffType original = *type;
    *type = *StaffType::preset(StaffTypes::STANDARD);
    EXPECT_TRUE(notation::NoteInputBarModel::accidentalPresentationForScore(score.get(), score->engravingFont()).isEmpty());
    Measure* second = score->firstMeasure()->nextMeasure();
    ASSERT_TRUE(second);
    score->staff(0)->setStaffType(second->tick(), original);
    auto& input = score->inputState();
    input.setTrack(0);
    input.setSegment(second->first(SegmentType::ChordRest));
    input.setNoteEntryMode(true);
    EXPECT_FALSE(notation::NoteInputBarModel::accidentalPresentationForScore(score.get(), score->engravingFont()).isEmpty());
    input.setNoteEntryMode(false);
    EXPECT_TRUE(notation::NoteInputBarModel::accidentalPresentationForScore(score.get(), score->engravingFont()).isEmpty());
    *type = original;
    score->deselectAll();
    EXPECT_FALSE(notation::NoteInputBarModel::accidentalPresentationForScore(score.get(), score->engravingFont()).isEmpty());
    EXPECT_TRUE(notation::NoteInputBarModel::accidentalPresentationForScore(nullptr, score->engravingFont()).isEmpty());
}

TEST_F(MeloUiModelTests, StaffSectionsAreRelevantOnlyForCompatibleSelection) {
    auto* selected = selectMeasure(0);
    ElementKeySet keys { AbstractInspectorModel::makeKey(selected) };
    auto sections = AbstractInspectorModel::sectionTypesByElementKeys(keys, false, { selected });
    EXPECT_TRUE(sections.count(InspectorSectionType::SECTION_MELO_STAFF));
    EXPECT_TRUE(sections.count(InspectorSectionType::SECTION_MELO_SCORE));
    EXPECT_TRUE(AbstractInspectorModel::sectionTypesByElementKeys({}, false, {}).empty());
    StaffType original = *score->staff(0)->staffType(Fraction(0, 1));
    *score->staff(0)->staffType(Fraction(0, 1)) = *StaffType::preset(StaffTypes::STANDARD);
    sections = AbstractInspectorModel::sectionTypesByElementKeys(keys, false, { selected });
    EXPECT_FALSE(sections.count(InspectorSectionType::SECTION_MELO_STAFF));
    *score->staff(0)->staffType(Fraction(0, 1)) = original;
}
TEST_F(MeloUiModelTests, StaffPresentationIsUndoableAndPreservesMusicalState) {
    selectMeasure(0);
    MeloStaffSettingsModel model(nullptr, muse::modularity::globalCtx(), &repository);
    model.context.set(global);
    model.loadProperties();
    ASSERT_TRUE(model.settings()["available"].toBool());
    auto before = score->staff(0)->staffType(Fraction(0, 1))->meloStateJson();
    int undo = score->undoStack()->size();
    model.setStaffOption("labels", 3);
    EXPECT_EQ(score->undoStack()->size(), undo + 1);
    EXPECT_EQ(score->staff(0)->staffType(Fraction(0, 1))->meloScaleDotLabelMode(), MeloScaleDotLabelMode::Split);
    EXPECT_EQ(score->staff(0)->staffType(Fraction(0, 1))->meloStateJson(), before);
    model.setStaffOption("labels", 3);
    EXPECT_EQ(score->undoStack()->size(), undo + 1);
    score->undoRedo(true, nullptr);
    model.loadProperties();
    EXPECT_EQ(model.settings()["labels"].toInt(), 0);
}
TEST_F(MeloUiModelTests, SelectionRefreshClearsOldFeedbackAndChangesTheTarget) {
    selectMeasure(0);
    MeloStaffSettingsModel model(nullptr, muse::modularity::globalCtx(), &repository);
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
TEST_F(MeloUiModelTests, KeyChoicesDistinguishEveryPeriod) {
    selectMeasure(0);
    MeloStaffSettingsModel model(nullptr, muse::modularity::globalCtx(), &repository);
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
TEST_F(MeloUiModelTests, ScaleChoiceReconcilesOtherPartsAndReportsTheMutation) {
    global->setCurrentNotation(nullptr);
    score.reset(ScoreRW::readScore(muse::String::fromUtf8(MELO_UI_TEST_DATA_ROOT) + u"/jimstaff_data/m9-satb-hymn.mscx", true));
    ASSERT_TRUE(score);
    ASSERT_EQ(score->nstaves(), 4);
    notation = std::make_shared<TestNotation>(score.get());
    global->setCurrentNotation(notation);
    selectMeasure(1);
    Measure* measure = score->firstMeasure()->nextMeasure();
    muse::String error;
    ASSERT_TRUE(melo::applyChangeToAllMeloParts(score.get(), measure, { u"scale:cycle:double-harmonic-minor" },
                                                error)) << error.toStdString();
    ASSERT_TRUE(melo::removeChange(score.get(), 0, measure, error)) << error.toStdString();
    MeloStaffSettingsModel model(nullptr, muse::modularity::globalCtx(), &repository);
    model.context.set(global);
    model.loadProperties();
    ASSERT_EQ(model.settings()["scalesIndex"].toInt(), 0);
    const int undo = score->undoStack()->size();
    model.applyOption("scales", 0);
    ASSERT_FALSE(model.hasError()) << model.status().toStdString();
    EXPECT_EQ(score->undoStack()->size(), undo + 1);
    EXPECT_FALSE(model.status().contains("no change"));
    for (staff_idx_t staff = 0; staff < 4; ++staff) {
        melo::StateChangeOptions options;
        ASSERT_TRUE(melo::changeOptions(score.get(), staff, measure, options));
        for (const auto& cycle : options.cycles) {
            if (cycle.id == u"scale:cycle:diatonic") {
                EXPECT_TRUE(cycle.current) << staff;
            }
        }
    }
    score->undoRedo(true, nullptr);
    melo::StateChangeOptions options;
    ASSERT_TRUE(melo::changeOptions(score.get(), 1, measure, options));
    for (const auto& cycle : options.cycles) {
        if (cycle.id == u"scale:cycle:double-harmonic-minor") {
            EXPECT_TRUE(cycle.current);
        }
    }
}
TEST_F(MeloUiModelTests, TuningRejectsInvalidAndNoOpGesturesWithoutUndoEntries) {
    MeloTuningModel model;
    model.context.set(global);
    model.playbackController.set(playback);
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
TEST_F(MeloUiModelTests, TuningAnnouncesEachRefusalOnceAndRefreshesSilently) {
    auto announcements = std::make_shared<TestAnnouncements>();
    MeloTuningModel model;
    model.context.set(global);
    model.playbackController.set(playback);
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
TEST_F(MeloUiModelTests, TuningCommitsOnceAndCancelsWhenSwitchingScores) {
    MeloTuningModel model;
    model.context.set(global);
    model.playbackController.set(playback);
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
    ASSERT_TRUE(melo::staffMetrics(score->staff(0)->staffType(Fraction(0, 1))->meloStateJson(), cents, period));
    EXPECT_NEAR(cents, original, 1e-8);
}
TEST_F(MeloUiModelTests, TuningRoutesLiveGeneratorOnlyForTheCurrentScore)
{
    std::vector<melo::ToneDiamondSetting> settings;
    uint32_t generatorParamId = 0;
    uint32_t xParamId = 0;
    uint32_t yParamId = 0;
    ASSERT_TRUE(melo::toneDiamondSettings(settings, generatorParamId, xParamId, yParamId));

    struct RoutedValue {
        notation::INotationPtr notation;
        muse::audio::AudioResourceId resourceId;
        uint32_t paramId = 0;
        double value = 0.0;
    };
    std::vector<RoutedValue> routed;
    ON_CALL(*playback, setInputParamPlainForResource(testing::_, testing::_, testing::_, testing::_))
    .WillByDefault([&routed](const notation::INotationPtr& expectedNotation, const muse::audio::AudioResourceId& resourceId,
                             uint32_t paramId, double value) {
        routed.push_back({ expectedNotation, resourceId, paramId, value });
    });

    muse::async::Notification resourceChanged;
    ON_CALL(*playback, inputResourceChanged()).WillByDefault(testing::Return(resourceChanged));
    MeloTuningModel model;
    model.context.set(global);
    model.playbackController.set(playback);
    model.init();
    ASSERT_FALSE(routed.empty());
    EXPECT_EQ(routed.back().notation, notation);
    EXPECT_EQ(routed.back().resourceId, muse::audio::AudioResourceId("JiMSynth"));
    EXPECT_EQ(routed.back().paramId, generatorParamId);

    const size_t callsBeforeRejectedPreview = routed.size();
    model.preview(model.maximum() + 1.0);
    EXPECT_EQ(routed.size(), callsBeforeRejectedPreview);

    const double original = model.cents();
    model.preview(690.0);
    ASSERT_FALSE(routed.empty());
    EXPECT_DOUBLE_EQ(routed.back().value, 690.0);
    model.cancel();
    EXPECT_DOUBLE_EQ(routed.back().value, original);

    model.commit(690.0);
    EXPECT_DOUBLE_EQ(routed.back().value, 690.0);
    score->undoRedo(true, nullptr);
    notation->notationChanged().notify();
    EXPECT_DOUBLE_EQ(routed.back().value, original);
    score->undoRedo(false, nullptr);
    notation->notationChanged().notify();
    EXPECT_DOUBLE_EQ(routed.back().value, 690.0);

    // Selecting MeloPresto Synth after the track already exists must adopt the
    // score's current generator without requiring another tuning gesture.
    const size_t callsBeforeResourceChange = routed.size();
    resourceChanged.notify();
    ASSERT_GT(routed.size(), callsBeforeResourceChange);
    EXPECT_DOUBLE_EQ(routed.back().value, 690.0);

    model.preview(691.0);
    const size_t callsBeforeScoreSwitch = routed.size();
    auto nextNotation = std::make_shared<TestNotation>(score.get());
    global->setCurrentNotation(nextNotation);
    ASSERT_GT(routed.size(), callsBeforeScoreSwitch);
    EXPECT_EQ(routed.back().notation, nextNotation);
    EXPECT_DOUBLE_EQ(routed.back().value, 690.0);
}
TEST_F(MeloUiModelTests, RejectedCommitRestoresLiveTuningAfterAValidPreview)
{
    Note* boundaryNote = nullptr;
    for (Segment* segment = score->firstSegment(SegmentType::ChordRest); segment;
         segment = segment->next1(SegmentType::ChordRest)) {
        EngravingItem* item = segment->element(0);
        if (item && item->isChord()) {
            boundaryNote = toChord(item)->notes().front();
            break;
        }
    }
    ASSERT_TRUE(boundaryNote);
    // This exact lattice identity is playable at generator700 and690;
    // generator720 projects it beyond the host's MIDI range.
    boundaryNote->setMeloPitch(-1, 11);
    size_t repairs = 0;
    muse::String error;
    ASSERT_TRUE(melo::normalizeStoredPitchesAfterLoad(score.get(), repairs, error, false));
    std::vector<double> routed;
    ON_CALL(*playback, setInputParamPlainForResource(testing::_, testing::_, testing::_, testing::_))
    .WillByDefault([&routed](const notation::INotationPtr&, const muse::audio::AudioResourceId&, uint32_t, double value) {
        routed.push_back(value);
    });
    MeloTuningModel model;
    model.context.set(global);
    model.playbackController.set(playback);
    model.init();
    const double original = model.cents();
    const int originalUndo = score->undoStack()->size();
    model.preview(690.0);
    ASSERT_FALSE(routed.empty());
    ASSERT_DOUBLE_EQ(routed.back(), 690.0);
    model.commit(720.0);
    EXPECT_FALSE(model.error().isEmpty());
    EXPECT_DOUBLE_EQ(model.cents(), original);
    EXPECT_DOUBLE_EQ(routed.back(), original);
    EXPECT_EQ(score->undoStack()->size(), originalUndo);
    EXPECT_EQ(boundaryNote->meloNPer(), -1);
    EXPECT_EQ(boundaryNote->meloNGen(), 11);
}
TEST_F(MeloUiModelTests, TuningControlMouseDragPreviewsCommitsOneUndoAndEscCancels)
{
    MeloTuningModel model;
    model.context.set(global);
    model.init();
    ASSERT_TRUE(model.available());

    QQmlEngine engine;
    const QVariantMap theme {
        { "accentColor", QColor(Qt::blue) }, { "accentOpacityNormal", 1.0 }, { "bodyFont", QFont() },
        { "borderWidth", 1 }, { "buttonColor", QColor(Qt::white) }, { "buttonOpacityHit", 1.0 },
        { "buttonOpacityHover", 1.0 }, { "buttonOpacityNormal", 1.0 }, { "defaultButtonSize", 24 },
        { "fontPrimaryColor", QColor(Qt::black) }, { "iconsFont", QFont() }, { "itemOpacityDisabled", 0.5 },
        { "linkColor", QColor(Qt::blue) }, { "navCtrlBorderWidth", 1 }, { "popupBackgroundColor", QColor(Qt::white) },
        { "strokeColor", QColor(Qt::black) }, { "textFieldColor", QColor(Qt::white) },
    };
    engine.rootContext()->setContextProperty("ui", QVariantMap { { "theme", theme } });
    engine.globalObject().setProperty("qsTrc", engine.evaluate("(function(context, text) { return text; })"));
    engine.addImportPath("qrc:/qt/qml");
    QQmlComponent component(&engine, QUrl("qrc:/qt/qml/MuseScore/Inspector/MeloTuningControl.qml"));
    ASSERT_TRUE(component.isReady()) << component.errorString().toStdString();

    muse::ui::NavigationPanel panel;
    QQuickWindow window;
    window.resize(800, 1000);
    auto* control = qobject_cast<QQuickItem*>(component.createWithInitialProperties({
        { "model", QVariant::fromValue(&model) }, { "navigationPanel", QVariant::fromValue(&panel) },
    }));
    ASSERT_TRUE(control) << component.errorString().toStdString();
    control->setParentItem(window.contentItem());
    window.show();
    QCoreApplication::processEvents();

    auto* slider = control->findChild<QQuickItem*>("jimsTuningSlider");
    ASSERT_TRUE(slider);
    ASSERT_GT(slider->height(), 20.0);
    const double original = model.cents();
    const int undoBefore = score->undoStack()->size();
    const QPoint press = slider->mapToScene({ slider->width() / 2.0, slider->height() / 2.0 }).toPoint();
    const QPoint moved = slider->mapToScene({ slider->width() / 2.0, slider->height() * 0.75 }).toPoint();

    QTest::mousePress(&window, Qt::LeftButton, Qt::NoModifier, press);
    QTest::mouseMove(&window, moved);
    QCoreApplication::processEvents();
    const double previewed = slider->property("value").toDouble();
    EXPECT_NEAR(model.cents(), previewed, 0.1);
    EXPECT_EQ(score->undoStack()->size(), undoBefore);
    EXPECT_GT(std::abs(previewed - original), 0.1);

    QTest::mouseRelease(&window, Qt::LeftButton, Qt::NoModifier, moved);
    QCoreApplication::processEvents();
    EXPECT_EQ(score->undoStack()->size(), undoBefore + 1);
    EXPECT_NEAR(model.cents(), previewed, 0.1);
    score->undoRedo(true, nullptr);
    notation->notationChanged().notify();
    EXPECT_NEAR(model.cents(), original, 0.1);

    QTest::mousePress(&window, Qt::LeftButton, Qt::NoModifier, press);
    QTest::mouseMove(&window, moved);
    QCoreApplication::processEvents();
    slider->forceActiveFocus();
    QTest::keyClick(&window, Qt::Key_Escape);
    QCoreApplication::processEvents();
    EXPECT_NEAR(model.cents(), original, 0.1);
    EXPECT_EQ(score->undoStack()->size(), undoBefore + 1);
}
TEST_F(MeloUiModelTests, ScorePresentationDoesNotChangeMusicalStateAndUndoes) {
    selectMeasure(0);
    MeloScoreSettingsModel model(nullptr, muse::modularity::globalCtx(), &repository);
    model.context.set(global);
    model.loadProperties();
    auto state = score->staff(0)->staffType(Fraction(0, 1))->meloStateJson();
    bool original = model.settings()["elide"].toBool();
    int undo = score->undoStack()->size();
    model.setOption("elide", !original);
    EXPECT_EQ(score->undoStack()->size(), undo + 1);
    EXPECT_EQ(score->staff(0)->staffType(Fraction(0, 1))->meloStateJson(), state);
    score->undoRedo(true, nullptr);
    model.loadProperties();
    EXPECT_EQ(model.settings()["elide"].toBool(), original);
}
