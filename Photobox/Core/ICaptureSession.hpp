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

    //! @brief True if the live view images of the camera should be visible
    Q_PROPERTY(bool previewVisible READ isPreviewVisible NOTIFY previewVisibleChanged);
    //! @brief True if the countdown should be visible
    Q_PROPERTY(bool countdownVisible READ isCountdownVisible NOTIFY countdownVisibleChanged);
    //! @brief The current text to show in the countdown ui
    Q_PROPERTY(QString countdownText READ getCountdownText NOTIFY countdownTextChanged)

  public:
    PBOX_DISABLE_COPY_MOVE(ICaptureSession);
    ICaptureSession() = default;
    ~ICaptureSession() override = default;

    Q_INVOKABLE virtual void triggerCapture() = 0;

    /// vvv property methods
    virtual bool isPreviewVisible() const = 0;
    virtual bool isCountdownVisible() const = 0;
    virtual const QString &getCountdownText() const = 0;
    /// ^^^ property methods

  Q_SIGNALS:
    //! @brief Will be emitted if the session has captured all images and a new session can begin.
    void finished();

    /// vvv property signals
    void previewVisibleChanged();
    void countdownVisibleChanged();
    void countdownTextChanged();
    /// ^^^ property signals
};
} // namespace Pbox
