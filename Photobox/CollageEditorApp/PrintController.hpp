// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathisloge.opensource@pm.me>
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