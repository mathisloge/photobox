// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QObject>
#include <QString>
#include <Pbox/DisableCopyMove.hpp>
#include "SystemStatusCode.hpp"

namespace Pbox
{
/**
 * @brief Represents a client that reports system status.
 *
 * This class holds the name, required flag, and current status of a system component.
 */
class SystemStatusClient : public QObject
{
    Q_OBJECT
  public:
    PBOX_DISABLE_COPY_MOVE(SystemStatusClient);
    /**
     * @brief Constructs a SystemStatusClient.
     * @param name The name of the client.
     * @param required Whether this client is required for system operation.
     */
    SystemStatusClient(QString name, bool required);
    /**
     * @brief Destructs the SystemStatusClient.
     */
    ~SystemStatusClient() override;

    /**
     * @brief Sets the system status code.
     * @param code The new status code.
     */
    void setSystemStatus(SystemStatusCode::Code code);
    /**
     * @brief Gets the current system status code.
     * @return The status code.
     */
    SystemStatusCode::Code systemStatus() const;

    /**
     * @brief Gets the name of the client.
     * @return The name.
     */
    const QString &name() const;
    /**
     * @brief Checks if this client is required.
     * @return True if required.
     */
    bool isRequired() const;

  Q_SIGNALS:
    /**
     * @brief Emitted when the system status changes.
     */
    void systemStatusChanged();

  private:
    QString name_;
    bool required_{false};
    SystemStatusCode::Code code_{SystemStatusCode::Code::Connecting};
};

} // namespace Pbox
