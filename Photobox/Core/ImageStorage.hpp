#pragma once
#include <QObject>

namespace Pbox
{
class ImageStorage : public QObject
{
    Q_OBJECT
  public:
    void onImageCaptured(const QImage &captured_image);
};
} // namespace Pbox