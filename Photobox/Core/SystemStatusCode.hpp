// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QtQmlIntegration/qqmlintegration.h>

namespace Pbox::SystemStatusCode
{
/**
 * @brief Namespace for system status codes.
 *
 * This namespace defines the possible status codes for system components.
 */
QML_ELEMENT
Q_NAMESPACE

/**
 * @brief Enumeration of system status codes.
 */
enum class Code
{
    Error,      // An error occurred.
    Connecting, // The component is connecting.
    Ok          // The component is operating normally.
};
Q_ENUM_NS(Code)

} // namespace Pbox::SystemStatusCode
