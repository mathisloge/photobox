#include "CameraNetwork.hpp"
#include <QNetworkReply>

using namespace Qt::Literals::StringLiterals;

namespace {
int parseFractionToInt(const QString &fraction);
}
namespace pbox {
CameraNetwork::CameraNetwork(QObject *parent) : QObject{parent} {
  network_manager_.setAutoDeleteReplies(true);
  network_manager_.setTransferTimeout(std::chrono::seconds{5});

  connect(&network_manager_, &QNetworkAccessManager::finished, this,
          &CameraNetwork::parseNetworkReply);

  heartbeat_timer_.setInterval(std::chrono::seconds{5});
  connect(&heartbeat_timer_, &QTimer::timeout, this, [this]() {
    qDebug() << "heartbeat";
    requestCameraState();
  });

  heartbeat_timer_.start();
}

void CameraNetwork::requestCapabilities() {}

/// cam.cgi?mode=getstate

void CameraNetwork::requestCameraState() {
  static const QUrl getStateUrl{"http://192.168.54.1/cam.cgi?mode=getstate"};

  auto &&reply = network_manager_.get(QNetworkRequest(getStateUrl));
  connect(reply, &QNetworkReply::errorOccurred, this,
          [](QNetworkReply::NetworkError err) {
            qDebug() << "error during request:" << err;
          });
  connect(reply, &QNetworkReply::sslErrors, this,
          []() { qDebug() << "ssl error"; });
}

void CameraNetwork::parseNetworkReply(QNetworkReply *reply) {
  QXmlStreamReader xml{reply};

  if (xml.readNextStartElement()) {
    if (xml.name() == "camrply"_L1) {
      readCamRply(xml);
    } else {
      xml.raiseError(QObject::tr("The xml is not of camrply type."));
    }
  }

  if (xml.hasError()) {
    qDebug() << "Got xml error" << xml.errorString();
  }
}

void CameraNetwork::readCamRply(QXmlStreamReader &xml) {
  Q_ASSERT(xml.isStartElement() && xml.name() == "camrply"_L1);
  while (xml.readNextStartElement()) {
    if (xml.name() == "result"_L1) {
      const auto status = readCamStatus(xml);
      qDebug() << "Status" << "=" << status;
    } else if (xml.name() == "state"_L1) {
      qDebug() << "got state";
      readState(xml);
    } else {
      qDebug() << "skipping" << xml.name();
      xml.skipCurrentElement();
    }
  }
  qDebug() << "end" << xml.name();
}

CameraNetwork::Status CameraNetwork::readCamStatus(QXmlStreamReader &xml) {
  Q_ASSERT(xml.name() == "result"_L1);
  if (xml.readElementText() == "ok"_L1) {
    return Status::Ok;
  }
  return Status::Error;
}

void CameraNetwork::readState(QXmlStreamReader &xml) {
  Q_ASSERT(xml.name() == "state"_L1);

  CameraState state;
  while (xml.readNextStartElement()) {
    if (xml.name() == "batt"_L1) {
      state.battery_remaining = parseFractionToInt(xml.readElementText());
    } else {
      xml.skipCurrentElement();
    }
  }

  camera_state_ = state;
  Q_EMIT cameraStateChanged();
}

const CameraState &CameraNetwork::state() const { return camera_state_; }
} // namespace pbox

namespace {
int parseFractionToInt(const QString &fraction) {
  QStringList parts = fraction.split('/');
  if (parts.size() != 2) {
    return -1; // Fehlerwert, falls das Format ungültig ist
  }

  bool ok_numerator = false;
  bool ok_denominator = false;

  const int numerator = parts[0].toInt(&ok_numerator);
  const int denominator = parts[1].toInt(&ok_denominator);

  if (!ok_numerator || !ok_denominator || denominator == 0) {
    return -1; // Fehlerwert, falls Konvertierung fehlschlägt oder Denominator 0
               // ist
  }

  // Umwandlung in Prozentwert
  return (numerator * 100) / denominator;
}
} // namespace
