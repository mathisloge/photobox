#include "GPhoto2Integration.hpp"
#include <Pbox/Logger.hpp>

DEFINE_LOGGER(gphoto2log);

namespace Pbox::GPhoto2
{
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
    switch (port_lookup_result)
    {
    case GP_ERROR_UNKNOWN_PORT:
        LOG_ERROR(gphoto2log, "The specified port ({}) can not be found.", port_value);
        break;
    default:
        break;
    }
    if (port_lookup_result < GP_OK)
    {
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
} // namespace Pbox::GPhoto2
