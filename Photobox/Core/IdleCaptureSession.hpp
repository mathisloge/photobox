// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "ICaptureSession.hpp"

namespace Pbox
{
class IdleCaptureSession : public ICaptureSession
{
    Q_OBJECT
  public:
    static constexpr std::string kName = "IdleSession";

    IdleCaptureSession();
    void triggerCapture() override;
    void imageCaptured(const QImage &captured_image, std::uint32_t image_id) override;
    void imageSaved(const std::filesystem::path &captured_image_path) override;

    bool isCountdownVisible() const override;
    const QString &getCountdownText() const override;
};
} // namespace Pbox
