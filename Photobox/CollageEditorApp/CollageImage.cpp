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

void CollageImage::setSourceOfPhoto(const QString &element_id, QString file_path)
{
    renderer_.setSourceOfPhoto(element_id.toStdString(), file_path.remove(0, "file://"_L1.size()).toStdString());
    update();
}

CollageImageModel *CollageImage::getModel()
{
    return std::addressof(model_);
}

void CollageImage::saveConfiguration()
{
    renderer_.saveConfiguration();
}
} // namespace Pbox