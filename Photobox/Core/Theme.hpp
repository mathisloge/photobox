#pragma once
#include <QColor>
#include <QObject>
#include <QtQmlIntegration/qqmlintegration.h>

namespace Pbox
{
namespace Details
{
consteval int hex_digit(char c)
{
    if (c >= '0' && c <= '9')
    {
        return c - '0';
    }
    if (c >= 'a' && c <= 'f')
    {
        return 10 + (c - 'a');
    }
    if (c >= 'A' && c <= 'F')
    {
        return 10 + (c - 'A');
    }
    throw "invalid hex digit";
}

consteval int hex_byte(char hi, char lo)
{
    return (hex_digit(hi) << 4) | hex_digit(lo);
}

consteval QColor operator""_qcolor(const char *str, std::size_t len)
{
    if (len != 7)
    {
        throw "Expected format \"#RRGGBB\"";
    }

    if (str[0] != '#')
    {
        throw "Expected format \"#RRGGBB\"";
    }

    const int r = hex_byte(str[1], str[2]);
    const int g = hex_byte(str[3], str[4]);
    const int b = hex_byte(str[5], str[6]);

    return QColor{r, g, b};
}
} // namespace Details
using Details::operator""_qcolor;

class Theme : public QObject
{
    Q_OBJECT
    QML_SINGLETON
    QML_ELEMENT
    Q_PROPERTY(QColor primary READ primary CONSTANT);
    Q_PROPERTY(QColor secondary READ secondary CONSTANT);
    Q_PROPERTY(QColor tertiary READ tertiary CONSTANT);
    Q_PROPERTY(QColor neutral READ neutral CONSTANT);
    Q_PROPERTY(QColor neutral_20 READ neutral_20 CONSTANT);

  public:
    static constexpr QColor primary()
    {
        return "#FF00FF"_qcolor;
    }

    static constexpr QColor secondary()
    {
        return "#FFD700"_qcolor;
    }

    static constexpr QColor tertiary()
    {
        return "#6200EA"_qcolor;
    }

    static constexpr QColor neutral()
    {
        return "#0A0E27"_qcolor;
    }

    static constexpr QColor neutral_20()
    {
        return "#2B2F49"_qcolor;
    }

  private:
};
} // namespace Pbox
