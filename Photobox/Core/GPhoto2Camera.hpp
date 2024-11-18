#pragma once
#include <GPhoto2Context.hpp>
#include <exec/async_scope.hpp>
#include "ICamera.hpp"
#include "Scheduler.hpp"

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

  private:
    void processPreviewImage(const QImage &image);

  private:
    std::stop_source stoken_;
    exec::async_scope async_scope_;
    bool capture_photo_{false};
};
} // namespace Pbox
