#pragma once
#include <QtQmlIntegration>

namespace Pbox::SystemStatus
{
QML_ELEMENT
Q_NAMESPACE

enum class StatusType
{
    Error,
    Connecting,
    Ok
};
Q_ENUM_NS(StatusType)

} // namespace Pbox::SystemStatus
