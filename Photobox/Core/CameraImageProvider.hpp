#pragma once
#include <QQuickImageProvider>

namespace Pbox
{
class CameraImageProvider : public QQuickImageProvider
{
    Q_OBJECT
  public:
    CameraImageProvider();
    void addImage(const QImage &image, QString id);
    void resetCache();
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;

  private:
    std::unordered_map<QString, QPixmap> captured_images_;
};
} // namespace Pbox
