// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QNetworkAccessManager>
#include <QPointer>
#include <QTimer>
#include "IEspHomeClient.hpp"

namespace Pbox
{
class EspHomeClient : public IEspHomeClient
{
  public:
    Q_DISABLE_COPY_MOVE(EspHomeClient);
    explicit EspHomeClient(QUrl base_url);
    ~EspHomeClient() override;

    void post(std::string_view url_request) override;

  private:
    void subscribeEvents();
    void readEventReply(QNetworkReply &reply);

  private:
    QTimer event_timeout_;
    QUrl base_url_;
    QNetworkAccessManager net_manager_;
    QPointer<QNetworkReply> sse_reply_{nullptr};
};
} // namespace Pbox
