#pragma once
#include <QNetworkAccessManager>
#include <QTimer>
#include <QtQmlIntegration>

namespace pbox {
struct CameraState {
  int battery_remaining;

  Q_GADGET
  Q_PROPERTY(int batteryRemaining MEMBER battery_remaining);
};

class CameraNetwork : public QObject {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(pbox::CameraState state READ state NOTIFY cameraStateChanged);

public:
  enum class Status { Unknown, Ok, Error };
  Q_ENUM(Status);

public:
  explicit CameraNetwork(QObject *parent = nullptr);
  void requestCapabilities();
  Q_INVOKABLE void startStream();
  Q_INVOKABLE void stopStream();

  const CameraState &state() const;

Q_SIGNALS:
  void cameraStateChanged();

private:
  void requestLcdOn();
  void requestCameraState();
  void sendRequest(const QUrl& url);
  void parseNetworkReply(QNetworkReply *reply);

  void readCamRply(QXmlStreamReader &xml);
  Status readCamStatus(QXmlStreamReader &xml);
  void readState(QXmlStreamReader &xml);

private:
  QNetworkAccessManager network_manager_;
  QTimer heartbeat_timer_;
  QTimer lcd_keepalive_timer_;

  CameraState camera_state_;
};
} // namespace pbox
