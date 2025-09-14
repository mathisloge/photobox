// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

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
