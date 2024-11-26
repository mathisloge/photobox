// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "EspHomeClient.hpp"
#include <QNetworkReply>
#include <QTimer>
#include <nlohmann/json.hpp>
namespace Pbox
{

namespace
{
QNetworkRequest prepareRequest(const QUrl &url)
{
    QNetworkRequest request(url);
    request.setRawHeader(QByteArray("Accept"), QByteArray("text/event-stream"));
    request.setHeader(QNetworkRequest::UserAgentHeader, "Photobox");
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute,
                         QNetworkRequest::AlwaysNetwork); // Events shouldn't be cached
    return request;
}
} // namespace

EspHomeClient::EspHomeClient(QUrl base_url)
    : base_url_{std::move(base_url)}
{
    net_manager_.setAutoDeleteReplies(true);
    subscribeEvents();
}

EspHomeClient::~EspHomeClient() = default;

void EspHomeClient::post(std::string_view url_request)
{
    QNetworkRequest req{base_url_.resolved(QString::fromLocal8Bit(url_request))};
    req.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, QStringLiteral("text/plain;charset=UTF-8"));
    auto &&reply = net_manager_.post(req, QByteArray{});
    connect(reply, &QNetworkReply::errorOccurred, this, [](QNetworkReply::NetworkError err) {
        qDebug() << "error during request:" << err;
    });
    connect(reply, &QNetworkReply::sslErrors, this, []() { qDebug() << "ssl error"; });
}

void EspHomeClient::subscribeEvents()
{
    QNetworkRequest request = prepareRequest(base_url_.resolved(QStringLiteral("events")));
    auto *reply = net_manager_.get(request);

    connect(reply, &QNetworkReply::readyRead, this, [this, reply]() { readEventReply(*reply); });
    connect(reply, &QNetworkReply::errorOccurred, this, [this](QNetworkReply::NetworkError error_code) {
        qDebug() << "error for event stream: " << error_code << ". Try to reestablishing connection in 500ms";
        QTimer::singleShot(std::chrono::milliseconds{500}, this, &EspHomeClient::subscribeEvents);
    });
}

void EspHomeClient::readEventReply(QNetworkReply &reply)
{
    bool next_is_state{false};
    while (reply.canReadLine())
    {
        auto event_line = QString{reply.readLine()};
        if (event_line.startsWith(QStringLiteral("event: state")))
        {
            next_is_state = true;
        }
        else if (next_is_state and event_line.startsWith(QStringLiteral("data:")))
        {
            next_is_state = false;
            event_line.remove(0, std::string_view{"data:"}.size());
            const auto json = nlohmann::json::parse(event_line.toLocal8Bit());
            Q_EMIT eventReceived(json);
        }
    }
}

} // namespace Pbox
