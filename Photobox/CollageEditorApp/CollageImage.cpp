#include "CollageImage.hpp"

using namespace Qt::StringLiterals;

namespace Pbox
{
void CollageImage::paint(QPainter *painter)
{
    renderer_.render(painter);
}

void CollageImage::loadSource(QString file_path)
{
    renderer_.loadImage(file_path.remove(0, "file://"_L1.size()).toStdString());
    update();
}
} // namespace Pbox