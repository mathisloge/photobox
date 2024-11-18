#include "GPhoto2Integration.hpp"
#include <QImage>
#include <Pbox/Logger.hpp>
#include "GPhoto2Context.hpp"

DEFINE_LOGGER(gphoto2log);

namespace Pbox::GPhoto2
{
namespace
{
std::optional<QImage> readImageFromFile(CameraFile *file);
}

bool autodetectAndConnectCamera(Context &context)
{
    auto cam_list = makeUniqueCameraList();
    const auto autodetect_count = gp_camera_autodetect(cam_list.get(), context.context.get());
    if (autodetect_count <= GP_OK)
    {
        LOG_DEBUG(gphoto2log, "got no possible cameras");
        return false;
    }
    LOG_DEBUG(gphoto2log, "autoconnect count: {}", autodetect_count);

    const char *model_name = nullptr;
    const char *port_value = nullptr;
    gp_list_get_name(cam_list.get(), 0, &model_name);
    gp_list_get_value(cam_list.get(), 0, &port_value);
    LOG_INFO(gphoto2log, "Got camera model: {} on port {}", model_name, port_value);

    const auto camera_index = gp_abilities_list_lookup_model(context.camera_abilities_list.get(), model_name);
    if (camera_index < GP_OK)
    {
        LOG_ERROR(gphoto2log, "Could not lookup camera abilities");
        return false;
    }

    const auto port_lookup_result = gp_port_info_list_lookup_path(context.port_list.get(), port_value);
    if (port_lookup_result < GP_OK)
    {
        LOG_ERROR(gphoto2log,
                  "Could not obtain specified port ({}). Failed with: {}",
                  port_value,
                  gp_port_result_as_string(port_lookup_result));
        return false;
    }

    GPPortInfo port_info{};
    if (gp_port_info_list_get_info(context.port_list.get(), port_lookup_result, &port_info) < GP_OK)
    {
        LOG_ERROR(gphoto2log, "Could not get port info");
        return false;
    }

    auto camera = makeUniqueCamera(context.context.get());
    if (camera == nullptr)
    {
        LOG_ERROR(gphoto2log, "could not allocate camera");
        return false;
    }

    if (gp_abilities_list_get_abilities(context.camera_abilities_list.get(), camera_index, &context.camera_abilities) <
        GP_OK)
    {
        LOG_ERROR(gphoto2log, "Could not get camera abilities");
        return false;
    }
    if (gp_camera_set_abilities(camera.get(), context.camera_abilities) < GP_OK)
    {
        LOG_ERROR(gphoto2log, "Could not set camera abilities");
        return false;
    }

    if (gp_camera_set_port_info(camera.get(), port_info) < GP_OK)
    {
        LOG_ERROR(gphoto2log, "Could not set camera port info");
        return false;
    }

    context.camera = std::move(camera);
    return true;
}

std::optional<QImage> capturePreviewImage(Context &context)
{
    auto file = makeUniqueCameraFile();
    const auto ret_val = gp_camera_capture_preview(context.camera.get(), file.get(), context.context.get());
    if (ret_val < GP_OK)
    {
        return std::nullopt;
    }
    return readImageFromFile(file.get());
}

std::optional<QImage> captureImage(Context &context)
{
    CameraFilePath camera_file_path;
    /* NOP: This gets overridden in the library to /capt0000.jpg */
    strcpy(camera_file_path.folder, "/");
    strcpy(camera_file_path.name, "xxx.jpg");

    auto file = makeUniqueCameraFile();
    const auto ret_val =
        gp_camera_capture(context.camera.get(), GP_CAPTURE_IMAGE, &camera_file_path, context.context.get());
    if (ret_val < GP_OK)
    {
        LOG_ERROR(gphoto2log, "could invoke gphoto2 capture");
        return std::nullopt;
    }
    gp_camera_file_get(context.camera.get(),
                       camera_file_path.folder,
                       camera_file_path.name,
                       GP_FILE_TYPE_NORMAL,
                       file.get(),
                       context.context.get());

    return readImageFromFile(file.get());
}

namespace
{
std::optional<QImage> readImageFromFile(CameraFile *file)
{
    const char *buffer{};
    unsigned long int size{};
    const auto size_result = gp_file_get_data_and_size(file, &buffer, &size);
    if (size_result < GP_OK)
    {
        LOG_DEBUG(gphoto2log, "could not get size of file. result code: {}", size_result);
        return std::nullopt;
    }
    QImage img;
    if (!img.loadFromData(reinterpret_cast<const uchar *>(buffer), static_cast<int>(size)))
    {
        LOG_ERROR(gphoto2log, "Could not create image from data");
        return std::nullopt;
    }
    return img;
}
} // namespace
} // namespace Pbox::GPhoto2
