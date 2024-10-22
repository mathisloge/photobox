#include "SvgFontCache.hpp"
#include <QFile>

namespace Pbox
{
namespace
{
std::vector<CachedFont> initFonts();
}

SvgFontCache::SvgFontCache()
    : fonts_{initFonts()}
{}

SvgFontCache::~SvgFontCache() = default;

std::span<const CachedFont> SvgFontCache::getFonts() const
{
    return fonts_;
}

namespace
{
std::optional<CachedFont> fromQFile(QFile font_file)
{
    if (not font_file.open(QFile::OpenModeFlag::ReadOnly))
    {
        return std::nullopt;
    }
    return CachedFont{.font_data = font_file.readAll()};
}

std::vector<CachedFont> initFonts()
{
    std::vector<CachedFont> fonts;

    auto great_vibes_regular = fromQFile(QFile{":/qt/qml/Photobox/Core/Fonts/GreatVibes/GreatVibes-Regular.ttf"})
                                   .and_then([](auto &&font) -> std::optional<CachedFont> {
                                       font.name = "Great Vibes";
                                       font.bold = false;
                                       font.italic = false;
                                       return font;
                                   });
    if (great_vibes_regular.has_value())
    {
        fonts.emplace_back(*std::move(great_vibes_regular));
    }

    auto roboto_regular = fromQFile(QFile{":/qt/qml/Photobox/Core/Fonts/Roboto/Roboto-Regular.ttf"})
                              .and_then([](auto &&font) -> std::optional<CachedFont> {
                                  font.name = "Roboto";
                                  font.bold = false;
                                  font.italic = false;
                                  return font;
                              });
    if (roboto_regular.has_value())
    {
        fonts.emplace_back(*std::move(roboto_regular));
    }

    return fonts;
}
} // namespace

} // namespace Pbox
