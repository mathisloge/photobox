// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "RemoteTrigger.hpp"
#include "RemoteTriggerId.hpp"
namespace Pbox
{
/**
 * @brief Interface for creating RemoteTrigger instances.
 *
 * This abstract factory interface defines a method for constructing
 * RemoteTrigger objects using a given configuration. It allows
 * for decoupling the instantiation logic of RemoteTrigger objects
 * from their usage, supporting dependency injection and testing.
 */
class IRemoteTriggerFactory
{
  public:
    /**
     * @brief Creates a new RemoteTrigger instance for a esphome based trigger.
     *
     * @param config The configuration used to initialize the RemoteTrigger.
     * @return std::unique_ptr<RemoteTrigger> A unique pointer to the created RemoteTrigger.
     */
    [[nodiscard]] virtual std::unique_ptr<RemoteTrigger> createEsphomeTrigger(RemoteTriggerId name,
                                                                              std::string uri) = 0;
    virtual ~IRemoteTriggerFactory() = default;
};

} // namespace Pbox
