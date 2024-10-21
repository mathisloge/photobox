#pragma once
#include <QByteArray>

namespace Pbox
{
struct CachedFont
{
    std::string name;
    bool bold;
    bool italic;
    QByteArray font_data;
};

class SvgFontCache
{
  public:
    Q_DISABLE_COPY_MOVE(SvgFontCache);
    SvgFontCache();
    ~SvgFontCache();

    std::span<const CachedFont> getFonts() const;

  private:
    std::vector<CachedFont> fonts_;
};
} // namespace Pbox
