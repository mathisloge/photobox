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

  const CameraState& state() const;

Q_SIGNALS:
  void cameraStateChanged();

private:
  void requestCameraState();
  void parseNetworkReply(QNetworkReply *reply);

  void readCamRply(QXmlStreamReader &xml);
  Status readCamStatus(QXmlStreamReader &xml);
  void readState(QXmlStreamReader &xml);

private:
  QNetworkAccessManager network_manager_;
  QTimer heartbeat_timer_;

  CameraState camera_state_;
};
} // namespace pbox
