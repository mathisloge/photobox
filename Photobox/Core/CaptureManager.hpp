#pragma once
#include <QObject>
#include <QtQmlIntegration>
#include "ICaptureSession.hpp"

namespace Pbox
{
class CollageContext;
class ICamera;
class CaptureManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Provided by ApplicationState")

    Q_PROPERTY(Pbox::ICaptureSession *session READ getSession NOTIFY sessionChanged FINAL);
    Q_PROPERTY(Pbox::ICamera *camera READ getCamera CONSTANT);

  public:
    explicit CaptureManager(ICamera &camera, CollageContext &collage_context);
    Q_INVOKABLE void triggerButtonPressed();

    /// vvv property methods
    Pbox::ICaptureSession *getSession();
    ICamera *getCamera();
    /// ^^^ property methods

  Q_SIGNALS:
    /// vvv property signals
    void sessionChanged();
    /// ^^^ property signals

  private:
    std::unique_ptr<ICaptureSession> session_{nullptr};
    ICamera &camera_;
};
} // namespace Pbox
