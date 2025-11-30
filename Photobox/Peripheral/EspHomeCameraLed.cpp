// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "EspHomeCameraLed.hpp"
#include <QNetworkRequest>
#include <fmt/core.h>
#include "IEspHomeClient.hpp"

namespace Pbox
{
EspHomeCameraLed::EspHomeCameraLed(std::unique_ptr<IEspHomeClient> client)
    : client_{std::move(client)}
{}

EspHomeCameraLed::~EspHomeCameraLed()
{
    turnOff();
}

void EspHomeCameraLed::playEffect(CameraLed::Effect effect)
{
    static const std::unordered_map<Effect, std::string_view> kEffectStrings{
        {Effect::Capture, "Capture"},
        {Effect::Pulsate, "Pulsate"},
    };
    const auto it = kEffectStrings.find(effect);
    if (it == kEffectStrings.end())
    {
        return;
    }
    const auto request_url = fmt::format("light/cameralenslight/turn_on?effect={}", it->second);
    client_->post(request_url);
}

void EspHomeCameraLed::turnOff()
{
    constexpr std::string_view kRequestUrl = "light/cameralenslight/turn_off";
    client_->post(kRequestUrl);
}
} // namespace Pbox
