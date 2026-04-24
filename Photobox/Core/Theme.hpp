#pragma once
#include <QColor>
#include <QObject>
#include <QtQmlIntegration/qqmlintegration.h>

namespace Pbox
{
namespace Details
{
/**
 * @brief Converts a hex digit character to its integer value.
 * @param c The hex digit character.
 * @return The integer value.
 */
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

/**
 * @brief Converts two hex digit characters to a byte value.
 * @param hi The high nibble character.
 * @param lo The low nibble character.
 * @return The byte value.
 */
consteval int hex_byte(char hi, char lo)
{
    return (hex_digit(hi) << 4) | hex_digit(lo);
}

/**
 * @brief User-defined literal for creating QColor from hex string.
 *
 * This operator allows creating QColor objects from string literals
 * in the format "#RRGGBB" at compile time.
 *
 * @param str The hex string.
 * @param len The length of the string.
 * @return The QColor object.
 */
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

/**
 * @brief QML singleton providing application color theme.
 *
 * This class defines the color palette used throughout the application,
 * with compile-time color literals for type safety.
 */
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
    /**
     * @brief Gets the primary color.
     * @return The primary color.
     */
    static constexpr QColor primary()
    {
        return "#FF00FF"_qcolor;
    }

    /**
     * @brief Gets the secondary color.
     * @return The secondary color.
     */
    static constexpr QColor secondary()
    {
        return "#FFD700"_qcolor;
    }

    /**
     * @brief Gets the tertiary color.
     * @return The tertiary color.
     */
    static constexpr QColor tertiary()
    {
        return "#6200EA"_qcolor;
    }

    /**
     * @brief Gets the neutral color.
     * @return The neutral color.
     */
    static constexpr QColor neutral()
    {
        return "#0A0E27"_qcolor;
    }

    /**
     * @brief Gets the neutral 20% color.
     * @return The neutral 20% color.
     */
    static constexpr QColor neutral_20()
    {
        return "#2B2F49"_qcolor;
    }
};
} // namespace Pbox
