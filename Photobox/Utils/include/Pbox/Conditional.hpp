// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <exec/variant_sender.hpp>
#include <stdexec/execution.hpp>

namespace Pbox
{
inline auto conditional(bool cond, stdexec::sender auto left, stdexec::sender auto right)
    -> exec::variant_sender<decltype(left), decltype(right)>
{
    if (cond)
    {
        return left;
    }
    return right;
};
} // namespace Pbox
