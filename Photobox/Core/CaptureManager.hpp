// SPDX-FileCopyrightText: 2024 - 2026 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QObject>
#include <memory>
#include <Pbox/Instance.hpp>
#include <QtQmlIntegration/qqmlintegration.h>
#include "CapturePipeline.hpp"
#include "CaptureSessionCoordinator.hpp"
#include "ICamera.hpp"
#include "ICaptureSession.hpp"
#include "ImageProvider.hpp"
#include "SessionEffectController.hpp"
#include "TriggerController.hpp"

namespace Pbox
{
/**
 * @brief Facade for capture workflow coordination.
 *
 * CaptureManager is the QML-facing entry point for camera capture flows.
 * It delegates trigger handling, session switching, image capture processing,
 * and effect management to dedicated helper components.
 */
class CaptureManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Provided by ApplicationState")

    Q_PROPERTY(Pbox::ICaptureSession *session READ getSession NOTIFY sessionChanged FINAL);
    Q_PROPERTY(Pbox::ICamera *camera READ getCamera CONSTANT);

  public:
    PBOX_DISABLE_COPY_MOVE(CaptureManager);
    /**
     * @brief Constructs a CaptureManager.
     * @param scheduler Scheduler used for background work.
     * @param image_storage Image storage used to persist captured images.
     * @param camera Camera interface used to request captures.
     * @param trigger_manager Trigger manager handling remote triggers.
     * @param camera_led Camera LED controller for capture feedback.
     * @param capture_session_manager Session manager that creates capture sessions.
     */
    explicit CaptureManager(Instance<Scheduler> scheduler,
                            Instance<ImageStorage> image_storage,
                            Instance<ICamera> camera,
                            Instance<TriggerManager> trigger_manager,
                            Instance<CameraLed> camera_led,
                            Instance<CaptureSessionManager> capture_session_manager);
    /**
     * @brief Destroys the CaptureManager and its helper components.
     */
    ~CaptureManager() override;
    /**
     * @brief Starts capture flow for a trigger button.
     * @param trigger_id Identifier of the triggered remote.
     */
    Q_INVOKABLE void triggerButtonPressed(const QString &trigger_id);
    /**
     * @brief Starts capture flow for a session button.
     * @param session_id Identifier of the selected capture session.
     */
    Q_INVOKABLE void sessionButtonPressed(const QString &session_id);

    /**
     * @brief Creates a QML image provider for preview images.
     * @return A newly allocated ImageProvider.
     */
    ImageProvider *createImageProvider() const;

    /// vvv property methods
    Pbox::ICaptureSession *getSession();
    ICamera *getCamera();
    /// ^^^ property methods

  Q_SIGNALS:
    /// vvv property signals
    void sessionChanged();
    /// ^^^ property signals
    /**
     * @brief Emitted when a new image is available for preview.
     * @param image Captured image.
     * @param image_id Sequential image identifier.
     */
    void imageCaptured(const QImage &image, std::uint32_t image_id);
    /**
     * @brief Emitted when the current image preview cache should be cleared.
     */
    void resetImages();

  private:
    /**
     * @brief Responds to the completion of the current capture session.
     */
    void sessionFinished();

  private:
    Instance<ICamera> camera_;                          /**< Camera interface used for live capture requests. */
    std::unique_ptr<CapturePipeline> capture_pipeline_; /**< Handles image capture and saving pipeline. */
    std::unique_ptr<CaptureSessionCoordinator> session_coordinator_; /**< Manages capture session lifecycle. */
    std::unique_ptr<TriggerController> trigger_controller_;          /**< Handles trigger and session button events. */
    std::unique_ptr<SessionEffectController> session_effect_controller_; /**< Manages LED and trigger effects. */
};
} // namespace Pbox
