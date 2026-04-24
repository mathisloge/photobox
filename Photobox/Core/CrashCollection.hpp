// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <expected>
#include <system_error>
namespace Pbox
{
/**
 * @brief Installs the crash handler for the application.
 *
 * This function sets up crash dumping functionality to collect
 * diagnostic information when the application crashes.
 *
 * @return An expected result: void on success, error code on failure.
 */
[[nodiscard]] std::expected<void, std::error_code> install_crash_handler() noexcept;
} // namespace Pbox
