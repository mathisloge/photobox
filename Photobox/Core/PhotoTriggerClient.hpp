#pragma once
#include <QNetworkAccessManager>
#include <QObject>
#include <QtQmlIntegration>

namespace Pbox
{
class PhotoTriggerClient : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("only enum")

  public:
    enum class Effect
    {
        Idle,
        Countdown
    };
    Q_ENUM(Effect);

    PhotoTriggerClient(QUrl base_url);
    ~PhotoTriggerClient() override;
    Q_DISABLE_COPY_MOVE(PhotoTriggerClient);

    Q_INVOKABLE void playEffect(Effect new_effect);

  Q_SIGNALS:
    void triggered();

  private:
    void handleEvent(const QJsonDocument &json);
    void handleTriggerButtonEvent(const QJsonDocument &json);
    void setupRequestReply(QNetworkReply *reply);

  private:
    QUrl base_url_;
    bool trigger_state_{false};
    QNetworkAccessManager net_manager_;
};
} // namespace Pbox