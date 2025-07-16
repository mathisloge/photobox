// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <filesystem>
#include <unordered_map>
#include <Pbox/DisableCopyMove.hpp>
#include <lunasvg.h>

class QPainter;
namespace Pbox
{
class CollageFontCache;

void init_lunasvg(CollageFontCache &font_cache);

class CollageRenderer
{
  public:
    PBOX_DISABLE_COPY_MOVE(CollageRenderer);
    explicit CollageRenderer();
    ~CollageRenderer();

    void loadDocument(const std::string &file_path);
    void addPhotoElement(const std::string &element_id);
    void removePhotoElement(const std::string &element_id);

    void setSourceOfPhoto(const std::string &element_id, const std::string &file_path);

    void updateLayout();
    void render(QPainter *painter, float width = -1, float height = -1) const;
    void renderToFile(const std::filesystem::path &image_path) const;
    void dumpAsJson(const std::filesystem::path &json_path) const;

    const std::unordered_map<std::string, lunasvg::Element> &registeredImages() const;
    const std::filesystem::path &getDocumentPath() const;

  private:
    lunasvg::Bitmap scaledBitmap(float width, float height) const;

  private:
    std::filesystem::path base_image_file_path_;
    std::unique_ptr<lunasvg::Document> document_;
    std::unordered_map<std::string, lunasvg::Element> images_;
};
} // namespace Pbox
