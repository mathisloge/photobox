#include <QSignalSpy>
#include <CollageCaptureSession.hpp>
#include <CollageRenderer.hpp>
#include <Pbox/Logger.hpp>
#include <SvgFontCache.hpp>
#include <catch2/catch_test_macros.hpp>
#include "TestAssets.hpp"
#include "pixel_match.hpp"

using namespace Pbox;

DEFINE_LOGGER(testlog);

TEST_CASE("Test CollageCaptureSession", "[UnitTest]")
{
    const auto asset_path = std::filesystem::path{kAssetsPath};
    SvgFontCache font_cache;
    init_lunasvg(font_cache);
    CollageRenderer renderer{};
    CollageSettings settings{
        .automatic_capture = true, .seconds_between_capture = 3, .image_elements = {"image-1", "image-2"}};

    renderer.loadDocument(asset_path / "Collage.svg");
    std::ranges::for_each(settings.image_elements,
                          std::bind(&CollageRenderer::addPhotoElement, &renderer, std::placeholders::_1));

    CollageCaptureSession session{settings, renderer};
    QSignalSpy finished_spy{&session, &CollageCaptureSession::finished};

    session.imageCaptured(asset_path / "capture_1.png");
    REQUIRE(finished_spy.size() == 0);
    session.imageCaptured(asset_path / "capture_5.png");
    REQUIRE(finished_spy.size() == 1);

    // should not be added anywhere and the signal schould not be emitted
    session.imageCaptured(asset_path / "capture_5.png");
    REQUIRE(finished_spy.size() == 1);

    renderer.updateLayout();
    renderer.renderToFile(std::filesystem::current_path() / "collage_session_test.png");

    const QImage expected_img =
        QImage{(asset_path / "expected_collage.png").c_str()}.convertToFormat(QImage::Format_RGBA8888);
    const QImage acutal_img =
        QImage{(std::filesystem::current_path() / "collage_session_test.png").c_str()}.convertToFormat(
            QImage::Format_RGBA8888);

    QImage diff_image;
    const auto diff = pixel_match(expected_img, acutal_img, PixelCompareOptions{}, diff_image);
    diff_image.save("collage_session_test_diff.png");
    REQUIRE(diff < 30);
}
