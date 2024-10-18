#include "EspHomeRemoteTrigger.hpp"
#include <QNetworkRequest>
#include <fmt/core.h>
#include <nlohmann/json.hpp>
#include "IEspHomeClient.hpp"

namespace
{
bool parseTriggerButton(const nlohmann::json &json)
{
    bool pressed{false};
    constexpr std::string_view kIdTrigger = "binary_sensor-phototriggerbutton";
    try
    {
        if (json.at("id") == kIdTrigger)
        {
            pressed = json.at("value").get<bool>();
        }
    }
    catch (const std::exception &ex)
    {
        qCritical() << "Could not parse event json." << ex.what();
    }
    return pressed;
}
} // namespace

namespace Pbox
{
EspHomeRemoteTrigger::EspHomeRemoteTrigger(std::unique_ptr<IEspHomeClient> client)
    : client_{std::move(client)}
{
    connect(client_.get(), &IEspHomeClient::eventReceived, this, [this](const nlohmann::json &json) {
        const auto pressed = parseTriggerButton(json);
        updatePressedState(pressed);
    });
}

EspHomeRemoteTrigger::~EspHomeRemoteTrigger() = default;

void EspHomeRemoteTrigger::playEffect(RemoteTrigger::Effect effect)
{
    static const std::unordered_map<Effect, std::string_view> kEffectStrings{
        {Effect::Idle, "IdleMode"},
        {Effect::Countdown, "CountdownMode"},
    };
    const auto it = kEffectStrings.find(effect);
    if (it == kEffectStrings.end())
    {
        return;
    }
    const auto request_url = fmt::format("light/statuslight/turn_on?effect={}", it->second);
    client_->post(request_url);
}

void EspHomeRemoteTrigger::updatePressedState(bool is_pressed)
{
    if (pressed_ != is_pressed)
    {
        pressed_ = is_pressed;
        if (not pressed_)
        {
            Q_EMIT triggered();
        }
    }
}
} // namespace Pbox