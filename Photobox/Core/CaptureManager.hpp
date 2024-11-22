#pragma once
#include <QObject>
#include <QtQmlIntegration>
#include "ICaptureSession.hpp"

namespace Pbox
{
class CollageContext;
class CaptureManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Provided by ApplicationState")

    Q_PROPERTY(Pbox::ICaptureSession *session READ getSession NOTIFY sessionChanged FINAL);

  public:
    explicit CaptureManager(CollageContext &collage_context);
    Q_INVOKABLE void beginCollageCaptureSession();

    Pbox::ICaptureSession *getSession();

  Q_SIGNALS:
    void sessionChanged();

  private:
    std::unique_ptr<ICaptureSession> session_{nullptr};
};
} // namespace Pbox
