#include "PhotoTriggerClient.hpp"
#include <QJsonDocument>
#include <QNetworkReply>

using namespace Qt::Literals::StringLiterals;
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

PhotoTriggerClient::PhotoTriggerClient()
{
    net_manager_.setAutoDeleteReplies(true);

    QNetworkRequest request = prepareRequest(QUrl("http://192.168.0.31/events"));
    auto *reply = net_manager_.get(request);

    connect(reply, &QNetworkReply::readyRead, this, [this, reply]() {
        const auto event_type = QString{reply->readLine()};
        auto payload = reply->readAll();
        if (event_type.startsWith("event: state"_L1))
        {
            payload.remove(0, "data:"_L1.size());
            QJsonParseError parse_error;
            auto json = QJsonDocument::fromJson(payload, &parse_error);
            if (parse_error.error == QJsonParseError::NoError and json.isObject())
            {
                handleEvent(json);
            }
            else
            {
                qDebug() << "got event with data but couldn't parse json. Reason:" << parse_error.errorString()
                         << "payload=" << payload;
            }
        }
    });
}

void PhotoTriggerClient::handleEvent(const QJsonDocument &json)
{
    qDebug() << json;
    const auto id = json["id"_L1].toString();
    if (id == "binary_sensor-phototriggerbutton"_L1)
    {
        handleTriggerButtonEvent(json);
    }
}

void PhotoTriggerClient::handleTriggerButtonEvent(const QJsonDocument &json)
{
    const auto is_pressed = json["value"].toBool(false);
    if (is_pressed != trigger_state_)
    {
        if (trigger_state_)
        {
            Q_EMIT triggered();
        }
        trigger_state_ = is_pressed;
        qDebug() << "trigger event" << trigger_state_;
    }
}

PhotoTriggerClient::~PhotoTriggerClient() = default;
} // namespace Pbox