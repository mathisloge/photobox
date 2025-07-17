// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathis.opensource@tuta.io>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "CollageRenderer.hpp"
#include <QImage>
#include <QPainter>
#include <fstream>
#include <mutex>
#include <Pbox/Logger.hpp>
#include <nlohmann/json.hpp>
#include "CollageFontCache.hpp"

DEFINE_LOGGER(collagerenderer);
namespace Pbox
{

CollageRenderer::CollageRenderer() = default;

CollageRenderer::~CollageRenderer() = default;

void CollageRenderer::loadDocument(const std::string &file_path)
{
    base_image_file_path_ = file_path;
    document_ = lunasvg::Document::loadFromFile(file_path);
    LOG_DEBUG(collagerenderer, "loded image from {}. Is valid document: {}", file_path, (document_ != nullptr));
}

void CollageRenderer::addPhotoElement(const std::string &element_id)
{
    auto el = document_->getElementById(element_id);
    if (el.isNull())
    {
        LOG_ERROR(collagerenderer, "The element '{}' could not be found in the SVG.", element_id);
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
        LOG_ERROR(collagerenderer,
                  "Can't set image '{}' to element href since the element '{}' could not be found",
                  file_path,
                  element_id);
        return;
    }
    it->second.setAttribute("href", file_path);
}

void CollageRenderer::updateLayout()
{
    document_->updateLayout();
}

lunasvg::Bitmap CollageRenderer::scaledBitmap(float width, float height) const
{
    if (not document_)
    {
        return lunasvg::Bitmap{};
    }

    if (width > 0 and height > 0)
    {
        const float scale_width = width / document_->width();
        const float scale_height = height / document_->height();

        const float scale = std::min(scale_width, scale_height);

        width = static_cast<float>(document_->width() * scale);
        height = static_cast<float>(document_->height() * scale);
    }
    else
    {
        width = document_->width();
        height = document_->height();
    }

    return document_->renderToBitmap(static_cast<int>(width), static_cast<int>(height));
}

void CollageRenderer::render(QPainter *painter, float width, float height) const
{
    const auto bitmap = scaledBitmap(width, height);
    if (bitmap.isNull())
    {
        return;
    }
    const QImage pixmap{bitmap.data(), bitmap.width(), bitmap.height(), QImage::Format::Format_ARGB32_Premultiplied};

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
    const auto bitmap = scaledBitmap(3840, 2160);
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

void init_lunasvg(Pbox::CollageFontCache &font_cache)
{
    static thread_local std::once_flag has_init;
    std::call_once(has_init, [&font_cache]() {
        for (auto &&font : font_cache.getFonts())
        {
            lunasvg_add_font_face_from_data(
                font.name.c_str(),
                font.bold,
                font.italic,
                font.font_data.data(),
                font.font_data.size(),
                [](void * /*closure*/) {},
                nullptr);
        }
    });
}

} // namespace Pbox
