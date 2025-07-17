// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathis.opensource@tuta.io>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QObject>
#include <QtQmlIntegration>
namespace Pbox
{
class PrintController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
  public:
    Q_INVOKABLE void showPrintSettings();
};
} // namespace Pbox
