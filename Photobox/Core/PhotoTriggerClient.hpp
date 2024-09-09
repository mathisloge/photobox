#pragma once
#include <QNetworkAccessManager>
#include <QObject>
#include <QtQmlIntegration>

namespace Pbox
{
class PhotoTriggerClient : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS
  public:
    PhotoTriggerClient();
    ~PhotoTriggerClient() override;
    Q_DISABLE_COPY_MOVE(PhotoTriggerClient);

  Q_SIGNALS:
    void triggered();

  private:
    void handleEvent(const QJsonDocument &json);
    void handleTriggerButtonEvent(const QJsonDocument &json);

  private:
    bool trigger_state_{false};
    QNetworkAccessManager net_manager_;
};
} // namespace Pbox