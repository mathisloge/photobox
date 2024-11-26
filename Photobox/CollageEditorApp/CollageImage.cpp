// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "CollageImage.hpp"
#include <fstream>
#include <CollageSettings.hpp>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

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
    renderer_.updateLayout();
    update();
}

CollageImageModel *CollageImage::getModel()
{
    return std::addressof(model_);
}

void CollageImage::saveConfiguration()
{
    CollageSettings settings;

    const std::filesystem::path output_dir{"collage"};
    if (not std::filesystem::exists(output_dir))
    {
        std::filesystem::create_directory(output_dir);
    }

    std::transform(renderer_.registeredImages().begin(),
                   renderer_.registeredImages().end(),
                   std::back_insert_iterator{settings.image_elements},
                   [](auto &&kv) { return kv.first; });

    json collage_json = settings;

    std::ofstream o(output_dir / "collage_settings.json");
    o << std::setw(4) << collage_json << std::endl;
    const auto collage_svg_path = output_dir / "collage.svg";

    if (std::filesystem::exists(collage_svg_path))
    {
        std::filesystem::remove(collage_svg_path);
    }
    std::filesystem::copy_file(renderer_.getDocumentPath(), collage_svg_path);
}
} // namespace Pbox