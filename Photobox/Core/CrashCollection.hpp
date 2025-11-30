// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <expected>
#include <system_error>
namespace Pbox
{
[[nodiscard]] std::expected<void, std::error_code> install_crash_handler() noexcept;
}
