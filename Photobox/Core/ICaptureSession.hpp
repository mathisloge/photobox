// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QObject>
#include <filesystem>
#include <Pbox/DisableCopyMove.hpp>
#include <QtQmlIntegration/qqmlintegration.h>
#include "Countdown.hpp"
namespace Pbox
{
/**
 * @brief Interface for capture sessions.
 *
 * This class defines the interface for managing capture sessions, including status, countdown, and image handling.
 */
class ICaptureSession : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Will be provided as a concrete instance by the c++ API");

    //! @brief Holds the status of the session
    Q_PROPERTY(Pbox::ICaptureSession::Status status READ getStatus NOTIFY statusChanged FINAL);
    //! @brief True if the live view images of the camera should be visible
    Q_PROPERTY(bool liveViewVisible READ isLiveViewVisible NOTIFY liveViewVisibleChanged FINAL);
    //! @brief The countdown for the session
    Q_PROPERTY(Countdown *countdown READ getCountdown CONSTANT FINAL);
    //! @brief A string containing the preview image or empty if nothing should be shown
    Q_PROPERTY(QString previewImage READ getPreviewImage NOTIFY previewImageChanged FINAL)

  public:
    /**
     * @brief Enumeration of session statuses.
     */
    enum class Status
    {
        Idle,      // The session waits for user input and does nothing currently
        Capturing, // The session is actively working on capturing the necessary images
        Busy // The session is currently busy and won't accept user inputs and won't capture any images, set only if the
             // user should know about the busy state, otherwise use Capturing
    };
    Q_ENUM(Status);

    /**
     * @brief Enumeration of capture statuses.
     */
    enum class CaptureStatus
    {
        Idle,           // Nothing related to the camera currently happening
        BeforeCapture,  // The session is working towards a capture
        WaitForCapture, // The session is actively waiting for the completion of the capture
    };
    Q_ENUM(CaptureStatus);

  public:
    PBOX_DISABLE_COPY_MOVE(ICaptureSession);
    /**
     * @brief Constructs an ICaptureSession.
     * @param session_id_ The ID of the session.
     */
    ICaptureSession(std::string session_id_);
    /**
     * @brief Destructs the ICaptureSession.
     */
    ~ICaptureSession() override;

    /**
     * @brief Gets the session ID.
     * @return The session ID.
     */
    const std::string &sessionId() const;

    /**
     * @brief Triggers a capture.
     */
    virtual void triggerCapture() = 0;

    /**
     * @brief Called when an image is captured.
     * @param captured_image The captured image.
     * @param image_id The ID of the image.
     */
    virtual void imageCaptured(const QImage &captured_image, std::uint32_t image_id) = 0;
    /**
     * @brief Called when an image is saved.
     * @param captured_image_path The path to the saved image.
     */
    virtual void imageSaved(const std::filesystem::path &captured_image_path) = 0;

    /// vvv property methods
    /**
     * @brief Gets the current status.
     * @return The status.
     */
    Status getStatus() const;
    /**
     * @brief Gets the current capture status.
     * @return The capture status.
     */
    CaptureStatus getCaptureStatus() const;
    /**
     * @brief Checks if live view is visible.
     * @return True if visible.
     */
    bool isLiveViewVisible() const;
    /**
     * @brief Gets the countdown object.
     * @return Pointer to the countdown.
     */
    Countdown *getCountdown();
    /**
     * @brief Gets the preview image.
     * @return The preview image string.
     */
    const QString &getPreviewImage() const;
    /// ^^^ property methods

  Q_SIGNALS:
    /**
     * @brief Emitted when the session is finished.
     */
    void finished();
    /**
     * @brief Emitted when an image capture is requested.
     */
    void requestedImageCapture();

    /// vvv property signals
    void statusChanged();
    void captureStatusChanged();
    void liveViewVisibleChanged();
    void previewImageChanged();
    /// ^^^ property signals

  protected:
    void setStatus(ICaptureSession::Status status);
    void setCaptureStatus(ICaptureSession::CaptureStatus capture_status);
    void setLiveViewVisible(bool visible);
    void setPreviewImage(QString preview_image);

  private:
    Countdown countdown_;
    std::string session_id_;
    Status status_{Status::Idle};
    CaptureStatus capture_status_{CaptureStatus::Idle};
    bool live_view_visible_{false};
    QString preview_image_;
};
} // namespace Pbox
