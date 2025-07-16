// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <exec/async_scope.hpp>
#include <stdexec/execution.hpp>

namespace Pbox
{
inline void cleanup_async_scope(exec::async_scope &scope)
{
    auto cleanup_sender = scope.on_empty();
    stdexec::sync_wait(std::move(cleanup_sender));
}
} // namespace Pbox
