#pragma once
#include <QCamera>
#include <QImageCapture>
#include <QMediaCaptureSession>
#include <QVideoFrame>
#include "ICamera.hpp"

namespace Pbox
{
class MockCamera : public ICamera
{
    Q_OBJECT
  public:
    MockCamera();
    ~MockCamera() override;
    Q_DISABLE_COPY_MOVE(MockCamera);

    void requestCapturePhoto() override;

  private:
    QMediaCaptureSession capture_session_;
    QImageCapture image_capture_;
    QCamera camera_;
};
} // namespace Pbox