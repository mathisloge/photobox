// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <CameraLed.hpp>

namespace Pbox
{
class IEspHomeClient;
class EspHomeCameraLed : public CameraLed
{
  public:
    Q_DISABLE_COPY_MOVE(EspHomeCameraLed);
    explicit EspHomeCameraLed(std::unique_ptr<IEspHomeClient> client);
    ~EspHomeCameraLed() override;

    void playEffect(CameraLed::Effect effect) override;
    void turnOff() override;

  private:
    std::unique_ptr<IEspHomeClient> client_;
};
} // namespace Pbox
