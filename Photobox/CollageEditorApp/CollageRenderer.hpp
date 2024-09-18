#pragma once
#include <QPainter>
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

  private:
    std::unique_ptr<lunasvg::Document> document_;
    std::unordered_map<std::string, lunasvg::Element> images_;
};
} // namespace Pbox