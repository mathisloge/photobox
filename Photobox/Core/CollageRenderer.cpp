#include "CollageRenderer.hpp"
#include <QDebug>
#include <QFile>
#include <fstream>
#include <mutex>
#include <nlohmann/json.hpp>
namespace
{

void init_lunasvg()
{
    static std::once_flag has_init;
    std::call_once(has_init, []() {
        // QFile great_vibes{":/qt/qml/Photobox/Core/Fonts/GreatVibes/GreatVibes-Regular.ttf"};
        // if(not great_vibes.open(QFile::OpenModeFlag::ReadOnly)) {
        //     qDebug() << "Could not open resource";
        // }
        // static const auto data = great_vibes.readAll();
        // lunasvg_add_font_face_from_data("Great Vibes",false, false, data.data(), data.size(), [](void* closure){
        //     qDebug() << "data loaded and can be deleted";
        // }, nullptr);
        lunasvg_add_font_face_from_file(
            "Great Vibes",
            false,
            false,
            "/home/mathis/dev/photobox2/Photobox/Core/Fonts/GreatVibes/GreatVibes-Regular.ttf");
        // lunasvg_add_font_face_from_file("", false, false, "/home/mathis/dev/photobox2/test/Roboto-Regular.ttf");
        // lunasvg_add_font_face_from_file("", false, false, "/home/mathis/dev/photobox2/test/Roboto-Regular.ttf");
        // lunasvg_add_font_face_from_file("", true, false, "/home/mathis/dev/photobox2/test/Roboto-Bold.ttf");
        // lunasvg_add_font_face_from_file("", false, true, "/home/mathis/dev/photobox2/test/Roboto-Italic.ttf");
        // lunasvg_add_font_face_from_file("", true, true, "/home/mathis/dev/photobox2/test/Roboto-BoldItalic.ttf");
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
    base_image_file_path_ = file_path;
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
}

void CollageRenderer::updateLayout()
{
    document_->updateLayout();
}

void CollageRenderer::render(QPainter *painter, float width, float height) const
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

void CollageRenderer::renderToFile(const std::filesystem::path &image_path) const
{
    if (document_ == nullptr)
    {
        return;
    }

    const auto bitmap = document_->renderToBitmap();
    bitmap.writeToPng(image_path);
}

void CollageRenderer::dumpAsJson(const std::filesystem::path &json_path) const
{
    using nlohmann::json;

    json j;
    for (auto &&image : images_)
    {
        const auto image_path = image.second.getAttribute("href");
        j[image.first] = image_path;
    }
    std::ofstream o{json_path};
    o << std::setw(4) << j << std::endl;
}

const std::unordered_map<std::string, lunasvg::Element> &CollageRenderer::registeredImages() const
{
    return images_;
}

const std::filesystem::path &CollageRenderer::getDocumentPath() const
{
    return base_image_file_path_;
}
} // namespace Pbox