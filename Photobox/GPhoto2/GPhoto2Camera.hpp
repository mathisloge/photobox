#pragma once
#include <QCamera>
#include <QImageCapture>
#include <QMediaCaptureSession>
#include <QVideoFrame>
#include "GPhoto2Worker.hpp"
#include "ICamera.hpp"

namespace Pbox
{
class GPhoto2Worker;
class GPhoto2Camera : public ICamera
{
    Q_OBJECT
  public:
    GPhoto2Camera();
    ~GPhoto2Camera() override;
    Q_DISABLE_COPY_MOVE(GPhoto2Camera);

    void requestCapturePhoto() override;

  private:
    GPhoto2Worker worker_thread_;
};
} // namespace Pbox