#pragma once
#include <QPainter>
#include <filesystem>
#include <unordered_map>
#include <lunasvg.h>

namespace Pbox
{
class CollageRenderer
{
  public:
    CollageRenderer();
    void loadDocument(const std::string &file_path);
    void addPhotoElement(const std::string &element_id);
    void removePhotoElement(const std::string &element_id);

    void setSourceOfPhoto(const std::string &element_id, const std::string &file_path);

    void render(QPainter *painter, float width = -1, float height = -1);
    void renderToFile();

    const std::unordered_map<std::string, lunasvg::Element> &registeredImages() const;

    void saveConfiguration();

  private:
    std::filesystem::path base_image_file_path_;
    std::unique_ptr<lunasvg::Document> document_;
    std::unordered_map<std::string, lunasvg::Element> images_;
};
} // namespace Pbox