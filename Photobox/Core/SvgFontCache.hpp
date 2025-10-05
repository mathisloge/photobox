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
struct RegisteredFont
{
    std::string family;
    bool bold{};
    bool italic{};
    std::filesystem::path path;

    auto operator<=>(const RegisteredFont &rhs) const = default;
};
} // namespace Detail
} // namespace Pbox
template <>
struct std::hash<Pbox::Detail::RegisteredFont>
{
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
class SvgFontCache
{
  public:
    void registerFont(std::string family, bool bold, bool italic, std::filesystem::path path);

  private:
    std::unordered_set<Detail::RegisteredFont> registered_fonts_;
};

} // namespace Pbox
