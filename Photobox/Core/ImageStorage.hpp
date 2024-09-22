#pragma once
#include <QObject>
#include <filesystem>

namespace Pbox
{
class ImageStorage : public QObject
{
    Q_OBJECT
  public:
    explicit ImageStorage(std::filesystem::path storage_dir);
    void onImageCaptured(const QImage &captured_image);
  Q_SIGNALS:
    void imageSaved(const std::filesystem::path &file_path);

  private:
    std::filesystem::path storage_dir_;
    std::uint32_t image_counter_{0};
};
} // namespace Pbox