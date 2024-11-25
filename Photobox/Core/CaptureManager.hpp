#pragma once
#include <QObject>
#include <QtQmlIntegration>
#include <exec/async_scope.hpp>
#include "ICaptureSession.hpp"

namespace Pbox
{
class Scheduler;
class ImageStorage;
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
    PBOX_DISABLE_COPY_MOVE(CaptureManager);
    explicit CaptureManager(Scheduler &scheduler,
                            ImageStorage &image_storage,
                            ICamera &camera,
                            CollageContext &collage_context);
    ~CaptureManager();
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
    Scheduler &scheduler_;
    ImageStorage &image_storage_;
    ICamera &camera_;
    std::unique_ptr<ICaptureSession> session_{nullptr};
    exec::async_scope async_scope_;
};
} // namespace Pbox
