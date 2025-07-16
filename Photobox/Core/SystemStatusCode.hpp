// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QtQmlIntegration>

namespace Pbox::SystemStatusCode
{
QML_ELEMENT
Q_NAMESPACE

enum class Code
{
    Error,
    Connecting,
    Ok
};
Q_ENUM_NS(Code)

} // namespace Pbox::SystemStatusCode
