// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pixel_match.hpp"
#include <cstdint>
#include <span>
#include <fmt/core.h>
using PixelData = std::span<uint8_t>;
using CPixelData = std::span<const uint8_t>;

namespace
{
constexpr bool is_pixel_data(const CPixelData data)
{
    return !data.empty();
}

constexpr auto rgb2y(uint8_t r, uint8_t g, uint8_t b)
{
    return (r * 0.29889531) + (g * 0.58662247) + (b * 0.11448223);
}

constexpr auto rgb2i(uint8_t r, uint8_t g, uint8_t b)
{
    return (r * 0.59597799) - (g * 0.27417610) - (b * 0.32180189);
}

constexpr auto rgb2q(uint8_t r, uint8_t g, uint8_t b)
{
    return (r * 0.21147017) - (g * 0.52261711) + (b * 0.31114694);
}

constexpr uint8_t blend(double c, double a)
{
    return static_cast<uint8_t>(255 + ((c - 255) * a));
}

constexpr void draw_pixel(PixelData output, size_t pos, uint8_t r, uint8_t g, uint8_t b)
{
    output[pos + 0] = r;
    output[pos + 1] = g;
    output[pos + 2] = b;
    output[pos + 3] = 255;
}

constexpr void draw_gray_pixel(const CPixelData img, int i, double alpha, PixelData output)
{
    const auto r = img[i + 0];
    const auto g = img[i + 1];
    const auto b = img[i + 2];
    auto val = blend(rgb2y(static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b)),
                     alpha * static_cast<uint8_t>(img[i + 3]) / 255.0);
    draw_pixel(output, i, val, val, val);
}

constexpr double color_delta(const CPixelData img1, const CPixelData img2, int k, int m, bool yOnly = false)
{
    uint8_t r1 = static_cast<uint8_t>(img1[k + 0]);
    uint8_t g1 = static_cast<uint8_t>(img1[k + 1]);
    uint8_t b1 = static_cast<uint8_t>(img1[k + 2]);
    uint8_t a1 = static_cast<uint8_t>(img1[k + 3]);
    uint8_t r2 = static_cast<uint8_t>(img2[m + 0]);
    uint8_t g2 = static_cast<uint8_t>(img2[m + 1]);
    uint8_t b2 = static_cast<uint8_t>(img2[m + 2]);
    uint8_t a2 = static_cast<uint8_t>(img2[m + 3]);

    if (a1 == a2 && r1 == r2 && g1 == g2 && b1 == b2)
    {
        return 0;
    }

    if (a1 < 255)
    {
        double a1d = a1 / 255.0;
        r1 = blend(r1, a1d);
        g1 = blend(g1, a1d);
        b1 = blend(b1, a1d);
    }

    if (a2 < 255)
    {
        double a2d = a2 / 255.0;
        r2 = blend(r2, a2d);
        g2 = blend(g2, a2d);
        b2 = blend(b2, a2d);
    }

    const double y1 = rgb2y(r1, g1, b1);
    const double y2 = rgb2y(r2, g2, b2);
    const double y = y1 - y2;

    if (yOnly)
    {
        return y;
    }

    const auto i = rgb2i(r1, g1, b1) - rgb2i(r2, g2, b2);
    const auto q = rgb2q(r1, g1, b1) - rgb2q(r2, g2, b2);

    const auto delta = 0.5053 * y * y + 0.299 * i * i + 0.1957 * q * q;
    return y1 > y2 ? -delta : delta;
}

constexpr bool has_many_siblings(const CPixelData img, int x1, int y1, int width, int height)
{
    const int x0 = std::max(x1 - 1, 0);
    const int y0 = std::max(y1 - 1, 0);
    const int x2 = std::min(x1 + 1, width - 1);
    const int y2 = std::min(y1 + 1, height - 1);
    const auto pos = (y1 * width + x1) * 4;
    int zeroes = (x1 == x0 || x1 == x2 || y1 == y0 || y1 == y2) ? 1 : 0;

    for (auto x = x0; x <= x2; x++)
    {
        for (auto y = y0; y <= y2; y++)
        {
            if (x == x1 && y == y1)
            {
                continue;
            }

            auto pos2 = (y * width + x) * 4;
            if (img[pos] == img[pos2] && img[pos + 1] == img[pos2 + 1] && img[pos + 2] == img[pos2 + 2] &&
                img[pos + 3] == img[pos2 + 3])
            {
                zeroes++;
            }

            if (zeroes > 2)
            {
                return true;
            }
        }
    }
    return false;
}

constexpr bool antialiased(const CPixelData img, int x1, int y1, int width, int height, const CPixelData img2)
{
    int x0 = std::max(x1 - 1, 0);
    int y0 = std::max(y1 - 1, 0);
    int x2 = std::min(x1 + 1, width - 1);
    int y2 = std::min(y1 + 1, height - 1);
    size_t pos = (y1 * width + x1) * 4;
    int zeroes = (x1 == x0 || x1 == x2 || y1 == y0 || y1 == y2) ? 1 : 0;
    double min = 0;
    double max = 0;
    int minX = 0;
    int minY = 0;
    int maxX = 0;
    int maxY = 0;

    for (int x = x0; x <= x2; x++)
    {
        for (int y = y0; y <= y2; y++)
        {
            if (x == x1 && y == y1)
            {
                continue;
            }

            const auto delta = color_delta(img, img, pos, (y * width + x) * 4, true);

            if (delta == 0)
            {
                zeroes++;
                if (zeroes > 2)
                {
                    return false;
                }
            }
            else if (delta < min)
            {
                min = delta;
                minX = x;
                minY = y;
            }
            else if (delta > max)
            {
                max = delta;
                maxX = x;
                maxY = y;
            }
        }
    }

    if (min == 0 || max == 0)
    {
        return false;
    }

    return (has_many_siblings(img, minX, minY, width, height) && has_many_siblings(img2, minX, minY, width, height)) ||
           (has_many_siblings(img, maxX, maxY, width, height) && has_many_siblings(img2, maxX, maxY, width, height));
}

} // namespace

namespace Pbox
{
int pixel_match(const QImage &expected_img,
                const QImage &actual_img,
                const PixelCompareOptions &options,
                QImage &output_image)
{
    constexpr int kChannels = 4;
    output_image = QImage{expected_img.size(), QImage::Format_RGBA8888};

    const auto expected_has_data =
        is_pixel_data(CPixelData{expected_img.bits(), static_cast<std::size_t>(expected_img.sizeInBytes())});
    const auto actual_has_data =
        is_pixel_data(CPixelData{actual_img.bits(), static_cast<std::size_t>(actual_img.sizeInBytes())});
    const auto output_has_data =
        is_pixel_data(CPixelData{output_image.constBits(), static_cast<std::size_t>(output_image.sizeInBytes())});
    if (not expected_has_data or not actual_has_data or not output_has_data)
    {
        throw std::runtime_error(fmt::format("Could not run pixel_match since one of the given images doesn't have any "
                                             "size. expected_has_data={}; actual_has_data={}; output_has_data={}",
                                             expected_has_data,
                                             actual_has_data,
                                             output_has_data));
    }

    const auto expected_img_size = expected_img.size();
    const auto actual_img_size = actual_img.size();
    if (expected_img_size != actual_img_size)
    {
        throw std::runtime_error(
            fmt::format("Image sizes are not matching. expected_img.size()={}x{}; actual_img.size()={}x{}",
                        expected_img_size.width(),
                        expected_img_size.height(),
                        actual_img_size.width(),
                        actual_img_size.height()));
    }

    // check if images are identical
    if (expected_img == actual_img)
    {
        if (!options.diffMask)
        {
            for (size_t i = 0; i < (actual_img.width() * actual_img.height()); i++)
            {
                draw_gray_pixel(CPixelData{actual_img.constBits(), static_cast<std::size_t>(actual_img.sizeInBytes())},
                                kChannels,
                                options.alpha,
                                PixelData{output_image.bits(), static_cast<std::size_t>(output_image.sizeInBytes())});
            }
        }
        return 0;
    }

    double maxDelta = 35215 * options.threshold * options.threshold;
    auto &&aaColor = options.aaColor;
    auto &&diffColor = options.diffColor;
    auto &&diffColorAlt = options.diffColorAlt.has_value() ? options.diffColor : *options.diffColorAlt;
    int diff = 0;

    for (int y = 0; y < actual_img.height(); y++)
    {
        for (int x = 0; x < actual_img.width(); x++)
        {
            auto output_image_data =
                PixelData{output_image.bits(), static_cast<std::size_t>(output_image.sizeInBytes())};
            const auto actual_img_data =
                CPixelData{actual_img.bits(), static_cast<std::size_t>(actual_img.sizeInBytes())};
            const auto expected_img_data =
                CPixelData{expected_img.bits(), static_cast<std::size_t>(expected_img.sizeInBytes())};
            const size_t pos = (y * actual_img.width() + x) * kChannels;
            const double delta = color_delta(expected_img_data, actual_img_data, pos, pos);
            if (std::abs(delta) > maxDelta)
            {
                if (!options.includeAA &&
                    (antialiased(
                         expected_img_data, x, y, expected_img.width(), expected_img.height(), actual_img_data) ||
                     antialiased(
                         actual_img_data, x, y, expected_img.width(), expected_img.height(), expected_img_data)))
                {
                    if (!options.diffMask)
                    {
                        draw_pixel(output_image_data, pos, aaColor[0], aaColor[1], aaColor[2]);
                    }
                }
                else
                {
                    if (delta < 0)
                    {
                        draw_pixel(output_image_data, pos, diffColorAlt[0], diffColorAlt[1], diffColorAlt[2]);
                    }
                    else
                    {
                        draw_pixel(output_image_data, pos, diffColor[0], diffColor[1], diffColor[2]);
                    }
                    diff++;
                }
            }
            else if (!options.diffMask)
            {
                draw_gray_pixel(expected_img_data, pos, options.alpha, output_image_data);
            }
        }
    }
    return diff;
}
} // namespace Pbox
