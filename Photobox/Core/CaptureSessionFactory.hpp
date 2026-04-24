// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QColor>
#include <functional>
#include <Pbox/ObjectUniquePtr.hpp>
#include "Pbox/DisableCopyMove.hpp"

namespace Pbox
{
class ICaptureSession;

using CaptureSessionPtr = unique_object_ptr<ICaptureSession>;
using CaptureSessionFactoryFnc = std::function<CaptureSessionPtr()>;
using CaptureSessionId = std::string;

/**
 * @brief Abstract factory interface for creating capture sessions.
 *
 * This interface defines methods for creating capture sessions with initial countdown,
 * and providing metadata like session ID, name, and color.
 */
class ICaptureSessionFactory
{
  public:
    PBOX_DISABLE_COPY_MOVE(ICaptureSessionFactory);
    ICaptureSessionFactory() = default;
    virtual ~ICaptureSessionFactory() = default;
    /**
     * @brief Creates a new capture session with the specified initial countdown.
     * @param initial_countdown The initial countdown duration.
     * @return A unique pointer to the created capture session.
     */
    virtual CaptureSessionPtr create(std::chrono::seconds initial_countdown) const = 0;

    /**
     * @brief Gets the unique session ID.
     * @return The session ID.
     */
    virtual const CaptureSessionId &sessionId() const = 0;
    /**
     * @brief Gets the human-readable session name.
     * @return The session name.
     */
    virtual const std::string &name() const = 0;
    /**
     * @brief Gets the UI color associated with the session.
     * @return The color.
     */
    virtual QColor color() const = 0;
};

} // namespace Pbox
