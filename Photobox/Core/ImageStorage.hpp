// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <atomic>
#include <filesystem>

class QImage;
namespace Pbox
{
/**
 * @brief Manages persistent storage of captured images.
 *
 * This class handles saving captured images to the filesystem with
 * automatic sequential naming and directory management.
 */
class ImageStorage
{
  public:
    /**
     * @brief Constructs an ImageStorage with the specified directory.
     * @param storage_dir The directory for storing images.
     */
    explicit ImageStorage(std::filesystem::path storage_dir);
    /**
     * @brief Saves an image to disk.
     * @param image The image to save.
     * @return The path to the saved image.
     */
    std::filesystem::path saveImage(const QImage &image);
    /**
     * @brief Gets the current storage directory.
     * @return The storage directory path.
     */
    const std::filesystem::path &storageDir() const;
    /**
     * @brief Generates a new unique filename for an image.
     * @return The filename string.
     */
    std::string generateNewImageFilePath();
    /**
     * @brief Updates the storage directory and rescans existing files.
     * @param storage_dir The new storage directory.
     */
    void updateStorageDir(std::filesystem::path storage_dir);

  private:
    std::filesystem::path storage_dir_;     /**< The storage directory. */
    std::atomic_uint32_t image_counter_{0}; /**< Counter for unique filenames. */
};
} // namespace Pbox
