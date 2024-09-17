#include "CollageRenderer.hpp"
#include <QDebug>

namespace Pbox
{
void CollageRenderer::loadImage(const std::string &file_path)
{
    document_ = lunasvg::Document::loadFromFile(file_path);
    qDebug() << "loaded image" << file_path << "valid=" << (document_ != nullptr);
}

void CollageRenderer::addPhotoElement(const std::string &element_id)
{
    auto el = document_->getElementById(element_id);
    if (el.isNull())
    {
        qDebug() << "null element";
        return;
    }
    images_.insert_or_assign(element_id, std::move(el));
}

void CollageRenderer::removePhotoElement(const std::string &element_id)
{
    images_.erase(element_id);
}

void CollageRenderer::setSourceOfPhoto(const std::string &element_id, const std::string &file_path)
{
    auto it = images_.find(element_id);
    if (it != images_.end())
    {
        qDebug() << "Could not find image element" << element_id << "to set to path" << file_path;
        return;
    }
    it->second.setAttribute("href", file_path);
}

void CollageRenderer::render(QPainter *painter)
{
    if (document_ == nullptr or painter == nullptr)
    {
        return;
    }

    painter->save();

    painter->setPen(Qt::GlobalColor::red);
    painter->drawLine(QPoint{0, 0}, QPoint{200, 200});

    const auto bitmap = document_->renderToBitmap();
    const QImage pixmap{bitmap.data(),
                        static_cast<int>(bitmap.width()),
                        static_cast<int>(bitmap.height()),
                        QImage::Format::Format_ARGB32_Premultiplied};
    qDebug() << "draw" << bitmap.width() << bitmap.height() << pixmap.rect();
    painter->drawImage(pixmap.rect(), pixmap);

    painter->restore();
}

} // namespace Pbox