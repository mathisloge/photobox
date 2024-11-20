#pragma once
#include <QtQmlIntegration>

namespace Pbox::SystemStatusCode
{
QML_ELEMENT
Q_NAMESPACE

enum class Code
{
    Error,
    Connecting,
    Ok
};
Q_ENUM_NS(Code)

} // namespace Pbox::SystemStatusCode
