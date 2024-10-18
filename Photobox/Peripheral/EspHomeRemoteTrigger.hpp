#pragma once
#include <RemoteTrigger.hpp>

namespace Pbox
{
class IEspHomeClient;
class EspHomeRemoteTrigger : public RemoteTrigger
{
  public:
    Q_DISABLE_COPY_MOVE(EspHomeRemoteTrigger);
    explicit EspHomeRemoteTrigger(std::unique_ptr<IEspHomeClient> client);
    ~EspHomeRemoteTrigger() override;

    void playEffect(RemoteTrigger::Effect effect) override;

  private:
    void updatePressedState(bool is_pressed);

  private:
    std::unique_ptr<IEspHomeClient> client_;
    bool pressed_{false};
};
} // namespace Pbox