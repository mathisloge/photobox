// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QObject>
#include <unordered_set>
#include "SystemStatusModel.hpp"

namespace Pbox
{
/**
 * @brief Manager for system status clients.
 *
 * This class manages the registration of system status clients and
 * evaluates the overall system status based on the clients' statuses.
 */
class SystemStatusManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Provided by ApplicationState")

    Q_PROPERTY(Pbox::SystemStatusModel *model READ getModel CONSTANT FINAL);
    Q_PROPERTY(Pbox::SystemStatusManager::Status status READ getStatus NOTIFY statusChanged FINAL);

  public:
    /**
     * @brief Enumeration of possible system statuses.
     */
    enum class Status
    {
        Initializing,              // System is initializing.
        RequiredClientsInitilized, // Required clients are initialized.
        AllClientsInitializied     // All clients are initialized.
    };
    Q_ENUM(Status);

  public:
    /**
     * @brief Registers a system status client.
     * @param client The client to register.
     */
    void registerClient(const Pbox::SystemStatusClient &client);

    /// vvv property methods
    /**
     * @brief Gets the system status model.
     * @return Pointer to the model.
     */
    SystemStatusModel *getModel();
    /**
     * @brief Gets the current system status.
     * @return The status.
     */
    Status getStatus() const;
    /// ^^^ property methods
  Q_SIGNALS:
    /// vvv property signals
    void statusChanged();
    /// ^^^ property signals

  private:
    /**
     * @brief Evaluates the overall system status based on clients.
     */
    void evaluateSystemStatus();

  private:
    std::unordered_set<const SystemStatusClient *> clients_;
    SystemStatusModel model_;
    Status status_{Status::AllClientsInitializied};
};
} // namespace Pbox
