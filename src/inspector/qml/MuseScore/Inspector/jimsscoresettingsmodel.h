// SPDX-License-Identifier: GPL-3.0-only
// MuseScore-Studio-CLA-applies
#pragma once
#include "abstractinspectormodel.h"
namespace mu::inspector {
class JimsScoreSettingsModel : public AbstractInspectorModel
{
    Q_OBJECT
    QML_ELEMENT
        QML_UNCREATABLE("Created by the inspector")
    Q_PROPERTY(QVariantMap settings READ settings NOTIFY settingsChanged)
public:
    JimsScoreSettingsModel(QObject* parent, const muse::modularity::ContextPtr& ctx, IElementRepositoryService* repository);
    QVariantMap settings() const { return m_settings; }
    void createProperties() override {}
    void resetProperties() override {}
    void requestElements() override;
    void loadProperties() override;
    Q_INVOKABLE void setOption(const QString& name, const QVariant& value);
signals:
    void settingsChanged();
protected:
    bool shouldUpdateOnEmptyPropertyAndStyleIdSets() const override { return true; }
    void onNotationChanged(const engraving::PropertyIdSet&, const engraving::StyleIdSet&) override { loadProperties(); }
private:
    QVariantMap m_settings;
};
}
