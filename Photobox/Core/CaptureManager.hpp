// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QObject>
#include <QtQmlIntegration>
#include <exec/async_scope.hpp>
#include "CaptureSessionFactory.hpp"
#include "ICaptureSession.hpp"
#include "ImageProvider.hpp"
namespace Pbox
{
class Scheduler;
class ImageStorage;
class CollageContext;
class RemoteTrigger;
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
                            RemoteTrigger &remote_trigger,
                            CaptureSessionFactoryFnc collage_session_factory);
    ~CaptureManager() override;
    Q_INVOKABLE void triggerButtonPressed();

    ImageProvider *createImageProvider();

    /// vvv property methods
    Pbox::ICaptureSession *getSession();
    ICamera *getCamera();
    /// ^^^ property methods

  Q_SIGNALS:
    /// vvv property signals
    void sessionChanged();
    /// ^^^ property signals
    void imageCaptured(const QImage &image, std::uint32_t image_id);
    void resetImages();

  private:
    void sessionFinished();
    void switchToSession(CaptureSessionPtr &&new_session);
    void handleSessionStatusChange();

  private:
    Scheduler &scheduler_;
    ImageStorage &image_storage_;
    ICamera &camera_;
    RemoteTrigger &remote_trigger_;
    CaptureSessionFactoryFnc collage_session_factory_;
    std::unique_ptr<ICaptureSession> session_{nullptr};
    exec::async_scope async_scope_;
    std::atomic_uint32_t image_ids_;
};
} // namespace Pbox
