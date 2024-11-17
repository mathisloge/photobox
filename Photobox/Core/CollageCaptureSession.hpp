#pragma once
#include <QImage>
#include <QObject>
#include <Pbox/DisableCopyMove.hpp>
#include "CollageSettings.hpp"

namespace Pbox
{
class CollageRenderer;
class CollageSettings;
class CollageCaptureSession : public QObject
{
    Q_OBJECT
  public:
    PBOX_DISABLE_COPY_MOVE(CollageCaptureSession);
    CollageCaptureSession(CollageSettings settings, CollageRenderer &renderer);
    ~CollageCaptureSession() override;

    void imageCaptured(const std::filesystem::path &captured_image_path);

  Q_SIGNALS:
    void finished();

  private:
    CollageSettings settings_;
    CollageRenderer &renderer_;
    int current_capture_{0};
};
} // namespace Pbox
