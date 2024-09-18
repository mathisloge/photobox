#include "CollageImage.hpp"

using namespace Qt::StringLiterals;

namespace Pbox
{
void CollageImage::paint(QPainter *painter)
{
    renderer_.render(painter, width(), height());
}

void CollageImage::loadSource(QString file_path)
{
    renderer_.loadDocument(file_path.remove(0, "file://"_L1.size()).toStdString());
    update();
}

void CollageImage::addPhotoElement(const QString &element_id)
{
    renderer_.addPhotoElement(element_id.toStdString());
}

void CollageImage::removePhotoElement(const QString &element_id)
{
    renderer_.removePhotoElement(element_id.toStdString());
}

void CollageImage::setSourceOfPhoto(const QString &element_id, QString file_path)
{
    renderer_.setSourceOfPhoto(element_id.toStdString(), file_path.remove(0, "file://"_L1.size()).toStdString());
    update();
}
} // namespace Pbox