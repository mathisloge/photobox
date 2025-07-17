// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathis.opensource@tuta.io>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <optional>

class QImage;
namespace Pbox::GPhoto2
{
struct Context;

bool autodetectAndConnectCamera(Context &context);

std::optional<QImage> capturePreviewImage(Context &context);

std::optional<QImage> captureImage(Context &context);
} // namespace Pbox::GPhoto2
