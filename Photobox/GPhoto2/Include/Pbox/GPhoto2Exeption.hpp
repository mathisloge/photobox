// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <stdexcept>

namespace Pbox
{
class GPhoto2Exception : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};
} // namespace Pbox
