#include <QSignalSpy>
#include <QTest>
#include <EspHomeRemoteTrigger.hpp>
#include <IEspHomeClient.hpp>
#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>

using namespace Pbox;

namespace
{
class EspHomeClientMock : public IEspHomeClient
{
  public:
    void post(std::string_view url_request) override
    {
        last_request = url_request;
    }

    void emitEventReceived(const nlohmann::json &json)
    {
        Q_EMIT eventReceived(json);
    }

    std::string last_request;
};
} // namespace

TEST_CASE("CollageRenderer save as png", "[RemoteTrigger]")
{
    auto mock_obj_dont_use = std::make_unique<EspHomeClientMock>();
    auto *mock_ptr = mock_obj_dont_use.get();
    EspHomeRemoteTrigger trigger{std::move(mock_obj_dont_use)};

    SECTION("After a release event a trigger should be emitted")
    {
        QSignalSpy trigger_spy{&trigger, &EspHomeRemoteTrigger::triggered};
        mock_ptr->emitEventReceived(nlohmann::json{{"id", "binary_sensor-phototriggerbutton"}, {"value", true}});
        REQUIRE(trigger_spy.size() == 0);
        mock_ptr->emitEventReceived(nlohmann::json{{"id", "binary_sensor-phototriggerbutton"}, {"value", false}});
        REQUIRE(trigger_spy.size() == 1);
    }

    SECTION("Play the correct effects")
    {
        trigger.playEffect(RemoteTrigger::Effect::Countdown);
        REQUIRE(mock_ptr->last_request == "light/statuslight/turn_on?effect=CountdownMode");
        trigger.playEffect(RemoteTrigger::Effect::Idle);
        REQUIRE(mock_ptr->last_request == "light/statuslight/turn_on?effect=IdleMode");
    }
}