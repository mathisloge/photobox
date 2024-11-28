// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <CollageFontCache.hpp>
#include <CollageRenderer.hpp>
#include <catch2/catch_test_macros.hpp>
#include "TestAssets.hpp"
#include "pixel_match.hpp"
using namespace Pbox;

TEST_CASE("CollageRenderer save as png", "[CollageRenderer]")
{
    const auto asset_path = std::filesystem::path{kAssetsPath};
    CollageFontCache font_cache;
    init_lunasvg(font_cache);
    CollageRenderer renderer{};

    renderer.loadDocument(asset_path / "Collage.svg");
    renderer.addPhotoElement("image-1");
    renderer.addPhotoElement("image-2");

    renderer.setSourceOfPhoto("image-1", asset_path / "capture_1.png");
    renderer.setSourceOfPhoto("image-2", asset_path / "capture_5.png");

    renderer.updateLayout();
    renderer.dumpAsJson(std::filesystem::current_path() / "collage_dump.json");
    renderer.renderToFile(std::filesystem::current_path() / "collage.png");

    const QImage expected_img =
        QImage{(asset_path / "expected_collage.png").c_str()}.convertToFormat(QImage::Format_RGBA8888);
    const QImage acutal_img =
        QImage{(std::filesystem::current_path() / "collage.png").c_str()}.convertToFormat(QImage::Format_RGBA8888);

    QImage diff_image;
    const auto diff = pixel_match(expected_img, acutal_img, PixelCompareOptions{}, diff_image);
    diff_image.save("collage_diff.png");
    REQUIRE(diff < 30);
}
