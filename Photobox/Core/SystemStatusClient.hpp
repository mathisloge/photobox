// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QString>
#include <Pbox/DisableCopyMove.hpp>
#include "SystemStatusCode.hpp"

namespace Pbox
{

class SystemStatusClient : public QObject
{
    Q_OBJECT
  public:
    PBOX_DISABLE_COPY_MOVE(SystemStatusClient);
    SystemStatusClient(QString name, bool required);
    ~SystemStatusClient() override;

    void setSystemStatus(SystemStatusCode::Code code);
    SystemStatusCode::Code systemStatus() const;

    const QString &name() const;
    bool isRequired() const;

  Q_SIGNALS:
    void systemStatusChanged();

  private:
    QString name_;
    bool required_{false};
    SystemStatusCode::Code code_{SystemStatusCode::Code::Connecting};
};

} // namespace Pbox
