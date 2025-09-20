// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <memory>
namespace Pbox
{
template <typename T>
using Instance = std::shared_ptr<T>;
}
