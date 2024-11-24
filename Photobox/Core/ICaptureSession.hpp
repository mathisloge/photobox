#pragma once
#include <QObject>
#include <QtQmlIntegration>
#include <Pbox/DisableCopyMove.hpp>
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
    Q_PROPERTY(bool previewVisible READ isPreviewVisible NOTIFY previewVisibleChanged FINAL);
    //! @brief True if the countdown should be visible
    Q_PROPERTY(bool countdownVisible READ isCountdownVisible NOTIFY countdownVisibleChanged FINAL);
    //! @brief The current text to show in the countdown ui
    Q_PROPERTY(QString countdownText READ getCountdownText NOTIFY countdownTextChanged FINAL)

  public:
    enum class Status
    {
        Idle,      //!< The session waits for user input and does nothing currently
        Capturing, //!< The session is activly working on capturing the necessary images
        Busy //!< The session is currently busy and won't accept user inputs and won't capture any images, set only if
             //!< the user should know about the busy state, otherwise ude capturing
    };
    Q_ENUM(Status);

  public:
    PBOX_DISABLE_COPY_MOVE(ICaptureSession);
    ICaptureSession() = default;
    ~ICaptureSession() override = default;

    Q_INVOKABLE virtual void triggerCapture() = 0;

    //! @brief Will be called if the camera has captured an image.
    virtual void imageCaptured(const QImage &captured_image) = 0;
    //! @brief Will be called if the captured image has been saved to the disk
    virtual void imageSaved(const std::filesystem::path &captured_image_path) = 0;

    /// vvv property methods
    Status getStatus() const;
    virtual bool isPreviewVisible() const = 0;
    virtual bool isCountdownVisible() const = 0;
    virtual const QString &getCountdownText() const = 0;
    /// ^^^ property methods

  Q_SIGNALS:
    //! @brief Will be emitted if the session has captured all images and a new session can begin.
    void finished();
    //! @brief Should be emitted if the session want's to capture an image with the camera.
    void requestedImageCapture();

    /// vvv property signals
    void statusChanged();
    void previewVisibleChanged();
    void countdownVisibleChanged();
    void countdownTextChanged();
    /// ^^^ property signals

  protected:
    void setStatus(ICaptureSession::Status status);

  private:
    Status status_{Status::Idle};
};
} // namespace Pbox
