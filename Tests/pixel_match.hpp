#pragma once
#include <QImage>
#include <array>
#include <cstdint>
namespace Pbox
{
struct PixelCompareOptions
{
    float threshold{0.1};                        // matching threshold (0 to 1); smaller is more sensitive
    bool includeAA{false};                       // whether to skip anti-aliasing detection
    float alpha{0.1};                            // opacity of original image in diff output
    std::array<uint8_t, 3> aaColor{255, 255, 0}; // color of anti-aliased pixels in diff output
    std::array<uint8_t, 3> diffColor{255, 0, 0}; // color of different pixels in diff output
    std::optional<std::array<uint8_t, 3>>
        diffColorAlt; // whether to detect dark on light differences between img1 and img2 and set an alternative color
                      // to differentiate between the two
    bool diffMask{false}; // draw the diff over a transparent background (a mask)
};

int pixel_match(const QImage &expected_img,
                const QImage &actual_img,
                const PixelCompareOptions &options,
                QImage &output_image);
} // namespace Pbox
