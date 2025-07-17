// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathis.opensource@tuta.io>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QObject>
#include <QtQmlIntegration>
#include <Pbox/DisableCopyMove.hpp>
#include "Countdown.hpp"
namespace Pbox
{
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
    enum class Status
    {
        Idle,      //!< The session waits for user input and does nothing currently
        Capturing, //!< The session is activly working on capturing the necessary images
        Busy //!< The session is currently busy and won't accept user inputs and won't capture any images, set only if
             //!< the user should know about the busy state, otherwise ude capturing
    };
    Q_ENUM(Status);

    enum class CaptureStatus
    {
        Idle,           //!< Nothing related to the camera currently happening
        BeforeCapture,  //!< The session is working towards a capture
        WaitForCapture, //!< The session is activly waiting for the completion of the capture
    };
    Q_ENUM(CaptureStatus);

  public:
    PBOX_DISABLE_COPY_MOVE(ICaptureSession);
    ICaptureSession(std::string name);
    ~ICaptureSession() override;

    //! @brief The name of the session.
    const std::string &name() const;

    virtual void triggerCapture() = 0;

    //! @brief Will be called if the camera has captured an image.
    virtual void imageCaptured(const QImage &captured_image, std::uint32_t image_id) = 0;
    //! @brief Will be called if the captured image has been saved to the disk
    virtual void imageSaved(const std::filesystem::path &captured_image_path) = 0;

    /// vvv property methods
    Status getStatus() const;
    CaptureStatus getCaptureStatus() const;
    bool isLiveViewVisible() const;
    Countdown *getCountdown();
    const QString &getPreviewImage() const;
    /// ^^^ property methods

  Q_SIGNALS:
    //! @brief Will be emitted if the session has captured all images and a new session can begin.
    void finished();
    //! @brief Should be emitted if the session want's to capture an image with the camera.
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
    std::string name_;
    Status status_{Status::Idle};
    CaptureStatus capture_status_{CaptureStatus::Idle};
    bool live_view_visible_{false};
    QString preview_image_;
};
} // namespace Pbox
