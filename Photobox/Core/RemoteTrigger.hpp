// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QObject>
#include <QtQmlIntegration>

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
    RemoteTrigger();
    ~RemoteTrigger() override;

    Q_INVOKABLE virtual void playEffect(RemoteTrigger::Effect effect) = 0;

  Q_SIGNALS:
    void triggered();
};
} // namespace Pbox