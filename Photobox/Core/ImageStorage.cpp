// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ImageStorage.hpp"
#include <QImage>
#include <Pbox/Logger.hpp>
#include <fmt/format.h>

DEFINE_LOGGER(image_store)

using namespace Qt::Literals::StringLiterals;
namespace Pbox
{

ImageStorage::ImageStorage(std::filesystem::path storage_dir)
    : storage_dir_{std::move(storage_dir)}
{
    updateStorageDir(std::move(storage_dir));
}

void ImageStorage::updateStorageDir(std::filesystem::path storage_dir)
{
    if (storage_dir == storage_dir_)
    {
        return;
    }

    storage_dir_ = std::move(storage_dir);
    LOG_INFO(logger_image_store(), "Updated image store dir to '{}'", storage_dir_.string());
    if (std::filesystem::exists(storage_dir_))
    {
        auto current_file_count = 0;
        for (auto entry : std::filesystem::directory_iterator{storage_dir_})
        {
            if (entry.is_regular_file())
            {
                current_file_count++;
            }
        }
        image_counter_ = current_file_count + 1;
        LOG_DEBUG(logger_image_store(), "Updated image counter to '{}'", image_counter_.load());
    }
    else
    {
        try
        {
            std::filesystem::create_directories(storage_dir_);
            image_counter_ = 1;
        }
        catch (const std::exception &exception)
        {
            LOG_CRITICAL(logger_image_store(),
                         "Could not create directories to path '{}'. Failed with: '{}'",
                         storage_dir_.string(),
                         exception.what());
        }
    }
}

std::filesystem::path ImageStorage::saveImage(const QImage &image)
{
    const auto file_path = storage_dir_ / generateNewImageFilePath();
    const bool image_saved = image.save(QString::fromStdString(file_path));
    if (not image_saved)
    {
        throw std::runtime_error{fmt::format("Image could not be saved to '{}'", file_path.string())};
    }
    return file_path;
}

std::string ImageStorage::generateNewImageFilePath()
{
    constexpr std::string_view kNameTemplate = "capture";
    return fmt::format("{}_{}.png", kNameTemplate, image_counter_++);
}

const std::filesystem::path &ImageStorage::storageDir() const
{
    return storage_dir_;
}

} // namespace Pbox
