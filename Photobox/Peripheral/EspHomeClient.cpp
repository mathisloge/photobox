// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "EspHomeClient.hpp"
#include <QNetworkReply>
#include <QTimer>
#include <Pbox/Logger.hpp>
#include <nlohmann/json.hpp>

DEFINE_LOGGER(esphome_client)

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
    event_timeout_.setSingleShot(false);
    event_timeout_.setInterval(std::chrono::seconds{10});
    event_timeout_.setTimerType(Qt::TimerType::PreciseTimer);
    net_manager_.setAutoDeleteReplies(true);
    subscribeEvents();
    connect(&event_timeout_, &QTimer::timeout, this, &EspHomeClient::subscribeEvents);
    connect(this, &EspHomeClient::connecting, &event_timeout_, &QTimer::stop);
}

EspHomeClient::~EspHomeClient() = default;

void EspHomeClient::post(std::string_view url_request)
{
    const auto url = base_url_.resolved(QString::fromLocal8Bit(url_request));
    QNetworkRequest req{url};
    req.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, QStringLiteral("text/plain;charset=UTF-8"));
    auto &&reply = net_manager_.post(req, QByteArray{});
    connect(reply, &QNetworkReply::errorOccurred, this, [this](QNetworkReply::NetworkError err) {
        LOG_ERROR(logger_esphome_client(),
                  "Error during request to {}: {}",
                  base_url_.toString().toStdString(),
                  QMetaEnum::fromType<QNetworkReply::NetworkError>().valueToKey(err));
    });
    connect(reply, &QNetworkReply::sslErrors, this, []() { qDebug() << "ssl error"; });
}

void EspHomeClient::subscribeEvents()
{
    if (sse_reply_ != nullptr)
    {
        sse_reply_->abort();
        LOG_DEBUG(logger_esphome_client(), "Network reply was still active. Stopping it.");
    }
    const auto request_url = base_url_.resolved(QStringLiteral("events"));
    QNetworkRequest request = prepareRequest(request_url);
    sse_reply_ = net_manager_.get(request);

    connect(sse_reply_, &QNetworkReply::readyRead, this, [this]() {
        Q_ASSERT(sse_reply_ != nullptr);
        Q_EMIT connected();
        readEventReply(*sse_reply_);
    });
    connect(sse_reply_, &QNetworkReply::finished, this, [this, request_url]() {
        LOG_DEBUG(logger_esphome_client(), "Network reply finished to {}", request_url.toString().toStdString());
        Q_ASSERT(sse_reply_ != nullptr);
        if (sse_reply_->error() != QNetworkReply::NoError)
        {
            constexpr std::chrono::milliseconds kRetryTime{500};
            LOG_ERROR(logger_esphome_client(),
                      "Network reply finished with error from {}: {}, retrying in {}ms",
                      request_url.toString().toStdString(),
                      QMetaEnum::fromType<QNetworkReply::NetworkError>().valueToKey(sse_reply_->error()),
                      kRetryTime.count());
            Q_EMIT connecting();
            QTimer::singleShot(kRetryTime, this, &EspHomeClient::subscribeEvents);
            return;
        }
    });
}

void EspHomeClient::readEventReply(QNetworkReply &reply)
{
    event_timeout_.start();
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
