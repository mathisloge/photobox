// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QObject>
#include <nlohmann/json_fwd.hpp>

namespace Pbox
{
class IEspHomeClient : public QObject
{
    Q_OBJECT
  public:
    virtual void post(std::string_view url_request) = 0;

  Q_SIGNALS:
    void eventReceived(const nlohmann::json &event);
};
} // namespace Pbox