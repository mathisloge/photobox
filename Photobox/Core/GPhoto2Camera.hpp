// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <GPhoto2Context.hpp>
#include <exec/async_scope.hpp>
#include <exec/task.hpp>
#include "ICamera.hpp"
#include "Scheduler.hpp"
#include "SystemStatusClient.hpp"

namespace Pbox
{
class GPhoto2Camera : public ICamera
{
    Q_OBJECT
  public:
    GPhoto2Camera(Scheduler &scheduler);
    ~GPhoto2Camera() override;
    Q_DISABLE_COPY_MOVE(GPhoto2Camera);

    void requestCapturePhoto() override;
    const SystemStatusClient &systemStatusClient() const override;

  private:
    void processPreviewImage(const QImage &image);

    exec::task<void> asyncCaptureLoop();
    exec::task<GPhoto2::Context> asyncAutoconnect();

  private:
    Scheduler &scheduler_;
    SystemStatusClient status_client_;
    exec::async_scope async_scope_;
    bool capture_photo_{false};
};
} // namespace Pbox
