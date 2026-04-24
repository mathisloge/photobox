// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <filesystem>
#include <string>
#include <unordered_set>
#include <boost/functional/hash.hpp>

namespace Pbox
{
namespace Detail
{
/**
 * @brief Describes a registered font.
 *
 * This struct holds information about a font registered with lunasvg.
 */
struct RegisteredFont
{
    std::string family;         /**< Font family name. */
    bool bold{};                /**< Whether the font is bold. */
    bool italic{};              /**< Whether the font is italic. */
    std::filesystem::path path; /**< Path to the font file. */

    auto operator<=>(const RegisteredFont &rhs) const = default; /**< Comparison operator. */
};
} // namespace Detail
} // namespace Pbox
template <>
struct std::hash<Pbox::Detail::RegisteredFont>
{
    /**
     * @brief Hash function for RegisteredFont.
     * @param s The font to hash.
     * @return The hash value.
     */
    constexpr std::size_t operator()(const Pbox::Detail::RegisteredFont &s) const noexcept
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, s.family);
        boost::hash_combine(seed, s.bold);
        boost::hash_combine(seed, s.italic);
        return seed;
    }
};

namespace Pbox
{
/**
 * @brief Manages font registration for SVG rendering.
 *
 * This class handles registering fonts with the lunasvg library
 * for use in SVG collage templates.
 */
class SvgFontCache
{
  public:
    /**
     * @brief Registers a font with lunasvg.
     * @param family The font family name.
     * @param bold Whether the font is bold.
     * @param italic Whether the font is italic.
     * @param path The path to the font file.
     */
    void registerFont(std::string family, bool bold, bool italic, std::filesystem::path path);

  private:
    std::unordered_set<Detail::RegisteredFont> registered_fonts_; /**< Set of registered fonts. */
};

} // namespace Pbox
