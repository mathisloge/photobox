#pragma once
#include <QImage>
#include <QObject>
#include <Pbox/DisableCopyMove.hpp>
#include "CollageSettings.hpp"
#include "ICaptureSession.hpp"

namespace Pbox
{
class CollageContext;
class CollageCaptureSession : public ICaptureSession
{
    Q_OBJECT
  public:
    PBOX_DISABLE_COPY_MOVE(CollageCaptureSession);
    explicit CollageCaptureSession(CollageContext &context);
    ~CollageCaptureSession() override;
    void triggerCapture() override;
    void imageCaptured(const QImage &captured_image) override;
    void imageSaved(const std::filesystem::path &captured_image_path) override;

    /// vvv property methods
    bool isPreviewVisible() const override;
    bool isCountdownVisible() const override;
    const QString &getCountdownText() const override;
    /// ^^^ property methods

  Q_SIGNALS:
    void finished();

  private:
    void startCountdownOrFinish();
    void handleCountdown();
    void setPreviewVisible(bool visible);

  private:
    CollageContext &context_;
    int current_capture_{0};
    bool preview_visible_{true};
    int countdown_counter_{0};
    QString final_countdown_text_{QStringLiteral("Smile!")};
    QString current_countdown_text_;
    QTimer countdown_timer_;
};
} // namespace Pbox
