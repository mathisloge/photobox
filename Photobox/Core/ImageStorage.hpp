#pragma once
#include <QObject>

namespace Pbox
{
class ImageStorage : public QObject
{
    Q_OBJECT
  public:
    ImageStorage();
    void onImageCaptured(const QImage &captured_image);

  private:
    std::uint32_t image_counter_{0};
};
} // namespace Pbox