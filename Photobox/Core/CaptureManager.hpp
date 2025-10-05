// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QObject>
#include <QtQmlIntegration>
#include <Pbox/ObjectUniquePtr.hpp>
#include <exec/async_scope.hpp>
#include "CaptureSessionFactory.hpp"
#include "ICamera.hpp"
#include "ICaptureSession.hpp"
#include "ImageProvider.hpp"
#include "Pbox/Instance.hpp"
#include "TriggerManager.hpp"
namespace Pbox
{
class Scheduler;
class ImageStorage;
class CollageContext;
class RemoteTrigger;
class CameraLed;
class CaptureManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Provided by ApplicationState")

    Q_PROPERTY(Pbox::ICaptureSession *session READ getSession NOTIFY sessionChanged FINAL);
    Q_PROPERTY(Pbox::ICamera *camera READ getCamera CONSTANT);

  public:
    PBOX_DISABLE_COPY_MOVE(CaptureManager);
    explicit CaptureManager(Instance<Scheduler> scheduler,
                            Instance<ImageStorage> image_storage,
                            Instance<ICamera> camera,
                            Instance<TriggerManager> trigger_manager,
                            Instance<CameraLed> camera_led,
                            Instance<CaptureSessionManager> capture_session_manager);
    ~CaptureManager() override;
    Q_INVOKABLE void triggerButtonPressed(const QString &trigger_id);

    ImageProvider *createImageProvider() const;

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
    void switchToSession(CaptureSessionPtr new_session);
    void handleSessionStatusChange();
    void handleSessionCaptureStatusChange();

  private:
    Instance<Scheduler> scheduler_;
    Instance<ImageStorage> image_storage_;
    Instance<ICamera> camera_;
    Instance<TriggerManager> trigger_manager_;
    Instance<CameraLed> camera_led_;
    Instance<CaptureSessionManager> capture_session_manager_;
    unique_object_ptr<ICaptureSession> session_{nullptr};
    exec::async_scope async_scope_;
    std::atomic_uint32_t image_ids_;
    TriggerId triggered_by_;
};
} // namespace Pbox
