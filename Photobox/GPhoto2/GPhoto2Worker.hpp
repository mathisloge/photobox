#pragma once
#include <QThread>
#include "GPhoto2CameraX.hpp"

namespace Pbox
{
class GPhoto2Worker : public QThread
{
    Q_OBJECT
  public:
    void requestStop();
    void requestCapture();

  private:
    void run() override;

    void run_normal_operation();
    void run_event_queue();

  Q_SIGNALS:
    void gotPreviewImage(QImage image);
    void gotCapturedImage(QImage image);

  private:
    bool running_{true};
    std::atomic_bool capture_requested_{false};
    GPhoto2CameraX camera_;
};
} // namespace Pbox