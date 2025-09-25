#include "SvgFontCache.hpp"
#include <Pbox/Logger.hpp>
#include <lunasvg.h>

DEFINE_LOGGER(svg_font_cache)
namespace Pbox
{

void SvgFontCache::registerFont(std::string family, bool bold, bool italic, std::filesystem::path path)
{
    auto &&[font, emplaced] =
        registered_fonts_.emplace(Detail::RegisteredFont{.family = std::move(family), .bold = bold, .italic = italic});

    LOG_DEBUG(
        logger_svg_font_cache(), "Register font '{}', bold={}, italic={}.", font->family, font->bold, font->italic);
    if (emplaced)
    {
        lunasvg_add_font_face_from_file(font->family.c_str(), font->bold, font->italic, path.c_str());
    }
    else
    {
        LOG_WARNING(logger_svg_font_cache(),
                    "Font '{}', bold={}, italic={} was already registered.",
                    font->family,
                    font->bold,
                    font->italic);
    }
}

} // namespace Pbox
