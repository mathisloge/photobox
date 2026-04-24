// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QObject>
#include <QtQmlIntegration/qqmlintegration.h>
#include "SystemStatusClient.hpp"

namespace Pbox
{
/**
 * @brief Interface for remote trigger effects.
 *
 * This class provides methods to play trigger effects and access the system status client.
 */
class RemoteTrigger : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("only enum")
  public:
    /**
     * @brief Enumeration of trigger effects.
     */
    enum class Effect
    {
        Idle,     // Idle effect.
        Countdown // Countdown effect.
    };
    Q_ENUM(Effect);

  public:
    Q_DISABLE_COPY_MOVE(RemoteTrigger);
    /**
     * @brief Constructs a RemoteTrigger.
     * @param name The name of the trigger.
     */
    explicit RemoteTrigger(QString name);
    /**
     * @brief Destructs the RemoteTrigger.
     */
    ~RemoteTrigger() override;

    /**
     * @brief Plays the specified trigger effect.
     * @param effect The effect to play.
     */
    Q_INVOKABLE virtual void playEffect(RemoteTrigger::Effect effect) = 0;
    /**
     * @brief Gets the system status client.
     * @return Reference to the status client.
     */
    const SystemStatusClient &systemStatusClient() const;

  Q_SIGNALS:
    /**
     * @brief Emitted when the trigger is activated.
     */
    void triggered();

  protected:
    SystemStatusClient system_status_client_; // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)
};
} // namespace Pbox
