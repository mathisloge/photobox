#include "ImageStorage.hpp"
#include <QDebug>
#include <QImage>
using namespace Qt::Literals::StringLiterals;

namespace Pbox
{
void ImageStorage::onImageCaptured(const QImage &captured_image)
{
    if (not captured_image.save("Test.png"_L1))
    {
        qCritical() << "Could not save image";
    }
}
} // namespace Pbox