// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

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
    void setStorageDir(std::filesystem::path storage_dir);

  private:
    std::filesystem::path storage_dir_;
    std::atomic_uint32_t image_counter_{0};
};
} // namespace Pbox
