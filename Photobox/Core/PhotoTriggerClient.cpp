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
        bool next_is_state{false};
        while (reply->canReadLine())
        {
            auto event_line = QString{reply->readLine()};
            if (event_line.startsWith("event: state"_L1))
            {
                next_is_state = true;
            }
            else if (next_is_state and event_line.startsWith("data:"_L1))
            {
                next_is_state = false;
                event_line.remove(0, "data:"_L1.size());
                QJsonParseError parse_error;
                auto json = QJsonDocument::fromJson(event_line.toLocal8Bit(), &parse_error);
                if (parse_error.error == QJsonParseError::NoError and json.isObject())
                {
                    handleEvent(json);
                }
                else
                {
                    qDebug() << "got event with data but couldn't parse json. Reason:" << parse_error.errorString()
                             << "payload=" << event_line;
                }
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

void PhotoTriggerClient::playEffect(Effect new_effect)
{
    static const std::unordered_map<Effect, QString> kEffectStrings{
        {Effect::Idle, QStringLiteral("IdleMode")},
        {Effect::Countdown, QStringLiteral("CountdownMode")},
    };

    const auto it = kEffectStrings.find(new_effect);
    if (it == kEffectStrings.end())
    {
        return;
    }
    QNetworkRequest req{QStringLiteral("http://192.168.0.31/light/statuslight/turn_on?effect=%1").arg(it->second)};
    qDebug() << "Requuest" << req.url();
    req.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "text/plain;charset=UTF-8"_L1);
    auto &&reply = net_manager_.post(req, QByteArray{});
    setupRequestReply(reply);
}

void PhotoTriggerClient::setupRequestReply(QNetworkReply *reply)
{
    connect(reply, &QNetworkReply::errorOccurred, this, [](QNetworkReply::NetworkError err) {
        qDebug() << "error during request:" << err;
    });
    connect(reply, &QNetworkReply::sslErrors, this, []() { qDebug() << "ssl error"; });
}

PhotoTriggerClient::~PhotoTriggerClient() = default;
} // namespace Pbox