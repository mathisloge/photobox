// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QByteArray>
#include <span>
#include <vector>
#include <Pbox/DisableCopyMove.hpp>

namespace Pbox
{
struct CachedFont
{
    std::string name;
    bool bold;
    bool italic;
    QByteArray font_data;
};

class CollageFontCache final
{
  public:
    PBOX_DISABLE_COPY_MOVE(CollageFontCache);
    CollageFontCache();
    ~CollageFontCache();

    std::span<const CachedFont> getFonts() const;

  private:
    std::vector<CachedFont> fonts_;
};
} // namespace Pbox
