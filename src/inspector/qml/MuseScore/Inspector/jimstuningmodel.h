/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 * Copyright (C) 2026 Jim Plamondon
 */
#pragma once

#include <QObject>
#include <qqmlintegration.h>
#include <memory>
#include "async/asyncable.h"
#include "context/iglobalcontext.h"
#include "notation/inotation.h"
#include "engraving/jims/jimstuningcontroller.h"
#include "accessibility/iaccessibilitycontroller.h"
#include "engraving/iengravingconfiguration.h"
#include <QColor>

namespace mu::inspector {
class JimsTuningModel : public QObject, public muse::async::Asyncable, public muse::Contextable
{
    Q_OBJECT
    QML_ELEMENT;
    Q_PROPERTY(bool available READ available NOTIFY changed)
    Q_PROPERTY(double cents READ cents NOTIFY changed)
    Q_PROPERTY(double minimum READ minimum CONSTANT)
    Q_PROPERTY(double maximum READ maximum CONSTANT)
    Q_PROPERTY(QColor criticalColor READ criticalColor NOTIFY changed)
    Q_PROPERTY(QString error READ error NOTIFY changed)
public:
    muse::GlobalInject<engraving::IEngravingConfiguration> engravingConfiguration;
    muse::ContextInject<context::IGlobalContext> context = { this };
    muse::ContextInject<muse::accessibility::IAccessibilityController> accessibilityController = { this };
    explicit JimsTuningModel(QObject* parent = nullptr);
    ~JimsTuningModel() override;
    Q_INVOKABLE void init();
    Q_INVOKABLE bool beginPreview();
    Q_INVOKABLE void preview(double value);
    Q_INVOKABLE void commit(double value);
    Q_INVOKABLE void acceptText(const QString& text);
    Q_INVOKABLE void cancel();
    bool available() const { return bool(m_controller); }
    double cents() const;
    double minimum() const { return m_minimum; }
    double maximum() const { return m_maximum; }
    QString error() const { return m_error; }
    QColor criticalColor() const;
signals:
    void changed();
private:
    void setNotation();
    void refresh();
    void notifyNotation();
    bool valid(double value);
    void reportError(const QString& error);
    notation::INotationPtr m_notation;
    std::unique_ptr<engraving::jims::TuningController> m_controller;
    double m_minimum = 0.0;
    double m_maximum = 0.0;
    double m_originalCents = 0.0;
    bool m_previewing = false;
    bool m_busy = false;
    QString m_error;
};
}
