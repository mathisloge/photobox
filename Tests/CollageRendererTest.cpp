#include <CollageRenderer.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace Pbox;

TEST_CASE("CollageRenderer save as png", "[CollageRenderer]")
{
    CollageRenderer renderer{};

    renderer.loadDocument("/home/mathis/Pictures/Collage.svg");
    renderer.addPhotoElement("image-1");
    renderer.addPhotoElement("image-2");

    renderer.setSourceOfPhoto("image-1", "/home/mathis/Pictures/capture_13.png");
    renderer.setSourceOfPhoto("image-2", "/home/mathis/Pictures/capture_16.png");

    renderer.updateLayout();
    renderer.dumpAsJson("test.json");
    renderer.renderToFile("test.png");
}