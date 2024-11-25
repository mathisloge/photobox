#pragma once
#include <atomic>
#include <filesystem>

class QImage;
namespace Pbox
{
class ImageStorage
{
  public:
    explicit ImageStorage(std::filesystem::path storage_dir);
    std::filesystem::path saveImage(const QImage &image);
    const std::filesystem::path &storageDir() const;
    std::string generateNewImageFilePath();

  private:
    std::filesystem::path storage_dir_;
    std::atomic_uint32_t image_counter_{0};
};
} // namespace Pbox
