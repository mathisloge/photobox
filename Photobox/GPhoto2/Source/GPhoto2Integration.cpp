// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "GPhoto2Integration.hpp"
#include <QImage>
#include <Pbox/Logger.hpp>
#include <fmt/format.h>
#include "GPhoto2Context.hpp"
#include "Pbox/GPhoto2Exception.hpp"

DEFINE_LOGGER(gphoto2);

namespace Pbox::GPhoto2
{
namespace
{
std::optional<QImage> readImageFromFile(CameraFile *file);
bool failed(int gphoto2_return_value);

struct CFreeDeleter
{
    void operator()(void *ptr) const noexcept
    {
        std::free(ptr); // NOLINT(cppcoreguidelines-no-malloc,cppcoreguidelines-owning-memory)
    }
};
} // namespace

bool autodetectAndConnectCamera(Context &context)
{
    auto cam_list = makeUniqueCameraList();
    const auto autodetect_count = gp_camera_autodetect(cam_list.get(), context.context.get());
    if (failed(autodetect_count) or autodetect_count == 0)
    {
        LOG_DEBUG_LIMIT(std::chrono::seconds{1}, logger_gphoto2(), "got no possible cameras");
        return false;
    }
    LOG_DEBUG(logger_gphoto2(), "autoconnect count: {}", autodetect_count);

    const char *model_name = nullptr;
    const char *port_value = nullptr;
    gp_list_get_name(cam_list.get(), 0, &model_name);
    gp_list_get_value(cam_list.get(), 0, &port_value);
    LOG_INFO(logger_gphoto2(), "Got camera model: {} on port {}", model_name, port_value);

    const auto camera_index = gp_abilities_list_lookup_model(context.camera_abilities_list.get(), model_name);
    if (failed(camera_index))
    {
        LOG_ERROR(logger_gphoto2(), "Could not lookup camera abilities");
        return false;
    }

    const auto port_lookup_result = gp_port_info_list_lookup_path(context.port_list.get(), port_value);
    if (failed(port_lookup_result))
    {
        LOG_ERROR(logger_gphoto2(),
                  "Could not obtain specified port ({}). Failed with: {}",
                  port_value,
                  gp_port_result_as_string(port_lookup_result));
        return false;
    }

    GPPortInfo port_info{};
    if (failed(gp_port_info_list_get_info(context.port_list.get(), port_lookup_result, &port_info)))
    {
        LOG_ERROR(logger_gphoto2(), "Could not get port info");
        return false;
    }

    auto camera = makeUniqueCamera(context.context.get());
    if (camera == nullptr)
    {
        LOG_ERROR(logger_gphoto2(), "could not allocate camera");
        return false;
    }

    if (failed(gp_abilities_list_get_abilities(
            context.camera_abilities_list.get(), camera_index, &context.camera_abilities)))
    {
        LOG_ERROR(logger_gphoto2(), "Could not get camera abilities");
        return false;
    }
    if (failed(gp_camera_set_abilities(camera.get(), context.camera_abilities)))
    {
        LOG_ERROR(logger_gphoto2(), "Could not set camera abilities");
        return false;
    }

    if (failed(gp_camera_set_port_info(camera.get(), port_info)))
    {
        LOG_ERROR(logger_gphoto2(), "Could not set camera port info");
        return false;
    }

    context.camera = std::move(camera);
    return true;
}

std::vector<CameraEvent> pollCameraEvents(Context &context)
{
    std::vector<CameraEvent> events;

    // Drain the event queue until it's empty
    while (true)
    {
        CameraEventType event_type{};
        void *raw_event_data = nullptr;

        const auto wait_for_event_return =
            gp_camera_wait_for_event(context.camera.get(), 5, &event_type, &raw_event_data, context.context.get());

        std::unique_ptr<void, CFreeDeleter> event_data{raw_event_data};

        if (failed(wait_for_event_return))
        {
            throw GPhoto2Exception{"Failed to poll camera event"};
        }

        if (event_type == GP_EVENT_TIMEOUT)
        {
            break;
        }

        CameraEvent event{event_type};

        if (event_type == GP_EVENT_FILE_ADDED || event_type == GP_EVENT_FOLDER_ADDED)
        {
            if (event_data != nullptr)
            {
                auto *path = static_cast<CameraFilePath *>(event_data.get());
                event.camera_file_path = *path;
            }
        }

        events.push_back(std::move(event));
    }

    return events;
}

void triggerCapture(Context &context)
{
    // Triggers the camera to shoot. Returns immediately without waiting for the file.
    if (failed(gp_camera_trigger_capture(context.camera.get(), context.context.get())))
    {
        throw GPhoto2Exception{"Failed to trigger capture"};
    }
}

std::optional<QImage> capturePreviewImage(Context &context)
{
    auto file = makeUniqueCameraFile();
    if (failed(gp_camera_capture_preview(context.camera.get(), file.get(), context.context.get())))
    {
        return std::nullopt;
    }
    return readImageFromFile(file.get());
}

std::optional<QImage> downloadImage(Context &context, CameraFilePath camera_file_path, const bool delete_from_camera)
{
    auto file = makeUniqueCameraFile();
    const auto get_file_result = gp_camera_file_get(context.camera.get(),
                                                    camera_file_path.folder,
                                                    camera_file_path.name,
                                                    GP_FILE_TYPE_NORMAL,
                                                    file.get(),
                                                    context.context.get());
    if (failed(get_file_result))
    {
        LOG_ERROR(logger_gphoto2(), "could not download image from camera");
        return std::nullopt;
    }

    auto image = readImageFromFile(file.get());

    if (delete_from_camera)
    {
        if (failed(gp_camera_file_delete(
                context.camera.get(), camera_file_path.folder, camera_file_path.name, context.context.get())))
        {
            throw GPhoto2Exception{fmt::format("Unable to delete image (folder: {}, file: {}) from camera.",
                                               camera_file_path.folder,
                                               camera_file_path.name)};
        }
    }
    return image;
}

namespace
{
std::optional<QImage> readImageFromFile(CameraFile *file)
{
    const char *buffer{};
    unsigned long int size{};
    if (failed(gp_file_get_data_and_size(file, &buffer, &size)))
    {
        LOG_DEBUG(logger_gphoto2(), "could not get size of file.");
        return std::nullopt;
    }
    QImage img;
    if (!img.loadFromData(reinterpret_cast<const uchar *>(buffer), static_cast<int>(size)))
    {
        LOG_ERROR(logger_gphoto2(), "Could not create image from data");
        return std::nullopt;
    }
    return img;
}

bool failed(int gphoto2_return_value)
{
    return gphoto2_return_value < GP_OK;
}
} // namespace
} // namespace Pbox::GPhoto2
