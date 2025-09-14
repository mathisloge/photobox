// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QObject>
#include <QtQmlIntegration>
#include "SystemStatusClient.hpp"

namespace Pbox
{
class RemoteTrigger : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("only enum")
  public:
    enum class Effect
    {
        Idle,
        Countdown
    };
    Q_ENUM(Effect);

  public:
    Q_DISABLE_COPY_MOVE(RemoteTrigger);
    explicit RemoteTrigger(QString name);
    ~RemoteTrigger() override;

    Q_INVOKABLE virtual void playEffect(RemoteTrigger::Effect effect) = 0;
    const SystemStatusClient &systemStatusClient() const;

  Q_SIGNALS:
    void triggered();

  protected:
    SystemStatusClient system_status_client_; // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)
};
} // namespace Pbox
