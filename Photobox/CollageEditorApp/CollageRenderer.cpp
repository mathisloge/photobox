#include "CollageRenderer.hpp"
#include <QDebug>
#include <mutex>
namespace
{

void init_lunasvg()
{
    static std::once_flag has_init;
    std::call_once(has_init, []() {
        lunasvg_add_font_face_from_file("", false, false, "/home/mathis/dev/photobox2/test/Roboto-Regular.ttf");
        lunasvg_add_font_face_from_file("", true, false, "/home/mathis/dev/photobox2/test/Roboto-Bold.ttf");
        lunasvg_add_font_face_from_file("", false, true, "/home/mathis/dev/photobox2/test/Roboto-Italic.ttf");
        lunasvg_add_font_face_from_file("", true, true, "/home/mathis/dev/photobox2/test/Roboto-BoldItalic.ttf");
    });
}

} // namespace
namespace Pbox
{

CollageRenderer::CollageRenderer()
{
    init_lunasvg();
}

void CollageRenderer::loadDocument(const std::string &file_path)
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
    images_[element_id] = std::move(el);
}

void CollageRenderer::removePhotoElement(const std::string &element_id)
{
    images_.erase(element_id);
}

void CollageRenderer::setSourceOfPhoto(const std::string &element_id, const std::string &file_path)
{
    auto it = images_.find(element_id);
    if (it == images_.end())
    {
        qDebug() << "Could not find image element" << element_id << "to set to path" << file_path;
        return;
    }
    it->second.setAttribute("href", file_path);
    document_->updateLayout();
}

void CollageRenderer::render(QPainter *painter, float width, float height)
{
    if (document_ == nullptr or painter == nullptr)
    {
        return;
    }

    if (width > 0 and height > 0)
    {
        const float scale_width = static_cast<float>(width) / document_->width();
        const float scale_height = static_cast<float>(height) / document_->height();

        const float scale = std::min(scale_width, scale_height);

        width = static_cast<float>(document_->width() * scale);
        height = static_cast<float>(document_->height() * scale);
    }
    else
    {
        width = static_cast<float>(document_->width());
        height = static_cast<float>(document_->height());
    }

    const auto bitmap = document_->renderToBitmap(static_cast<int>(width), static_cast<int>(height));
    const QImage pixmap{
        bitmap.data(), static_cast<int>(width), static_cast<int>(height), QImage::Format::Format_ARGB32_Premultiplied};

    painter->save();

    painter->drawImage(pixmap.rect(), pixmap);

    painter->restore();
}

} // namespace Pbox