#include <Pbox/Logger.hpp>
#include <Pbox/Sessions/CollageRenderer.hpp>
#include <SvgFontCache.hpp>
#include <catch2/catch_test_macros.hpp>
#include <quill/StopWatch.h>
#include <quite/image.hpp>
#include <quite/image_utils.hpp>
#include "TestAssets.hpp"

using namespace Pbox;

DEFINE_LOGGER(test);

TEST_CASE("Test collage renderer with valid svg", "[sessions]")
{
    const auto asset_path = std::filesystem::path{kAssetsPath};
    SvgFontCache font_cache;
    quill::StopWatchTsc swt;

    font_cache.registerFont("Roboto", false, false, asset_path / "Roboto" / "Roboto-Regular.ttf");
    LOG_INFO(logger_test(), "registerFont(Roboto) elapsed: {:.6}", swt);
    font_cache.registerFont("Great Vibes", false, true, asset_path / "GreatVibes" / "GreatVibes-Regular.ttf");
    LOG_INFO(logger_test(), "registerFont(Great Vibes) elapsed: {:.6}", swt);

    CollageRenderer renderer;
    renderer.loadDocument(asset_path / "two-collage.svg");
    LOG_INFO(logger_test(), "loadDocument elapsed: {:.6}", swt);

    renderer.addPhotoElement("image-1");
    LOG_INFO(logger_test(), "addPhotoElement(image-1) elapsed: {:.6}", swt);

    renderer.addPhotoElement("image-2");
    LOG_INFO(logger_test(), "addPhotoElement(image-2) elapsed: {:.6}", swt);

    renderer.setSourceOfPhoto("image-1", asset_path / "capture_1.png");
    LOG_INFO(logger_test(), "setSourceOfPhoto(image-1) elapsed: {:.6}", swt);

    renderer.setSourceOfPhoto("image-2", asset_path / "capture_5.png");
    LOG_INFO(logger_test(), "setSourceOfPhoto(image-2) elapsed: {:.6}", swt);

    renderer.renderToFile("collage_result.png");
    LOG_INFO(logger_test(), "renderToFile elapsed: {:.6}", swt);

    const quite::Image expected_img{asset_path / "expected_collage.png"};
    const quite::Image actual_img{std::filesystem::current_path() / "collage_result.png"};

    quite::Image diff_img;
    const auto result = quite::pixel_match(expected_img.data(), actual_img.data(), quite::PixelCompareOptions{});
    if (not result.has_value())
    {
        LOG_ERROR(logger_test(), "Could not compare images. Error: '{}'", result.error().message);
    }
    REQUIRE(result.has_value());

    result->diff_image.save_to("diff_collage.png");
    REQUIRE(result->diff < 30);
}
