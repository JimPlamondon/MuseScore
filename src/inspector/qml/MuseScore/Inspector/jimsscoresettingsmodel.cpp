// SPDX-License-Identifier: GPL-3.0-only
// MuseScore-Studio-CLA-applies
#include "jimsscoresettingsmodel.h"
#include "inspector/internal/ielementrepositoryservice.h"
#include "engraving/dom/score.h"
#include "engraving/editing/editscoreproperties.h"
#include "engraving/jims/jimsstrings.h"
using namespace mu::inspector;
using namespace mu::engraving;
JimsScoreSettingsModel::JimsScoreSettingsModel(QObject* parent, const muse::modularity::ContextPtr& ctx,
                                               IElementRepositoryService* repository)
    : AbstractInspectorModel(parent, ctx, repository)
{
    setSectionType(InspectorSectionType::SECTION_JIMS_SCORE);
    setTitle(muse::qtrc("inspector", "%1 score").arg(jims::featureName().toQString()));
}

void JimsScoreSettingsModel::requestElements() { m_elementList = m_repository->takeAllElements(); }
void JimsScoreSettingsModel::loadProperties()
{
    m_settings.clear();
    Score* score = currentNotation() ? currentNotation()->elements()->msScore() : nullptr;
    if (score) {
        m_settings["elide"] = score->style().styleB(Sid::jimsElideEmptyOctaves);
        m_settings["firstSystem"] = score->style().styleB(Sid::jimsShowAllOctavesInFirstSystem);
        m_settings["melody"] = int(score->jimsMelodyPart());
    }
    emit settingsChanged();
}

void JimsScoreSettingsModel::setOption(const QString& name, const QVariant& value)
{
    Score* score = currentNotation() ? currentNotation()->elements()->msScore() : nullptr;
    if (!score) {
        return;
    }
    if (name == "melody") {
        bool valid = false;
        int part = value.toInt(&valid);
        if (!valid || part < 0 || part > 3 || part == int(score->jimsMelodyPart())) {
            return;
        }
        score->startCmd(muse::TranslatableString("undoableAction", "Change JiMS melody part"));
        score->undo(new ChangeJimsMelodyPart(score, jims::MelodyPart(part)));
        score->endCmd();
    } else {
        if (name != "elide" && name != "firstSystem") {
            return;
        }
        Sid sid = name == "elide" ? Sid::jimsElideEmptyOctaves : Sid::jimsShowAllOctavesInFirstSystem;
        if (score->style().styleB(sid) == value.toBool()) {
            return;
        }
        score->startCmd(name == "elide" ? muse::TranslatableString("undoableAction",
                                                                   "Elide empty octaves") : muse::TranslatableString("undoableAction",
                                                                                                                     "Show all octaves in first system"));
        score->undoChangeStyleVal(sid, value.toBool());
        score->endCmd();
    }
    if (currentNotation()->undoStack()) {
        currentNotation()->undoStack()->stackChanged().notify();
    }
    updateNotation();
    loadProperties();
}
