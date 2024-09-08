#pragma once
#include <QQuickImageProvider>
#include <memory>

namespace Pbox
{
class ICamera;

class CameraImageProvider : public QQuickImageProvider
{

  public:
    explicit CameraImageProvider(std::shared_ptr<ICamera> camera);
    ~CameraImageProvider() override;
    Q_DISABLE_COPY_MOVE(CameraImageProvider);

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;

  private:
    std::shared_ptr<ICamera> camera_;
    QPixmap last_captured_image_;
};
} // namespace Pbox
