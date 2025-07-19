// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathis.opensource@tuta.io>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <functional>
#include "ObjectUniquePtr.hpp"
namespace Pbox
{
class ICaptureSession;

using CaptureSessionPtr = unique_object_ptr<ICaptureSession>;
using CaptureSessionFactoryFnc = std::function<CaptureSessionPtr()>;

} // namespace Pbox
