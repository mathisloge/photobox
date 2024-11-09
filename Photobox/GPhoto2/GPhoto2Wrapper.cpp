#include "GPhoto2Wrapper.hpp"
#include <Pbox/Logger.hpp>
#include <gphoto2-port-result.h>

DEFINE_LOGGER(gphoto2log);

namespace Pbox::GPhoto2
{
namespace
{
void handleGPhoto2Error(GPContext * /*context*/, const char *str, void * /*data*/)
{
    LOG_ERROR(gphoto2log, "{}", str);
}

void handleGPhoto2Status(GPContext * /*context*/, const char *str, void * /*data*/)
{
    LOG_INFO(gphoto2log, "{}", str);
}
} // namespace

ContextUniquePtr makeUniqueContext()
{
    ContextUniquePtr ctx{gp_context_new(), &gp_context_unref};

    gp_context_set_error_func(ctx.get(), handleGPhoto2Error, NULL);
    gp_context_set_status_func(ctx.get(), handleGPhoto2Status, NULL);

    return ctx;
}

CameraUniquePtr makeUniqueCamera(GPContext *context)
{
    CameraUniquePtr cam_ptr{nullptr, CameraDeleter{.context = context}};
    Camera *camera{nullptr};
    if (gp_camera_new(&camera) < GP_OK)
    {
        LOG_ERROR(gphoto2log, "could not allocate camera");
        return cam_ptr;
    }
    cam_ptr.reset(camera);
    return cam_ptr;
}

CameraListUniquePtr makeUniqueCameraList()
{
    CameraListUniquePtr cam_list{nullptr, gp_list_free};

    CameraList *list{nullptr};
    if (gp_list_new(&list) < GP_OK)
    {
        LOG_ERROR(gphoto2log, "could not allocate list");
    }
    cam_list.reset(list);

    return cam_list;
}

CameraAbilitiesListUniquePtr makeUniqueCameraListAbilities()
{
    CameraAbilitiesListUniquePtr list{nullptr, gp_abilities_list_free};
    CameraAbilitiesList *abilities{nullptr};
    if (gp_abilities_list_new(&abilities) < GP_OK)
    {
        LOG_ERROR(gphoto2log, "Could not allocated gp_abilities_list_new");
        return list;
    }
    list.reset(abilities);
    return list;
}

PortInfoListUniquePtr makeUniquePortInfoList()
{
    PortInfoListUniquePtr ptr{nullptr, gp_port_info_list_free};
    GPPortInfoList *port_info_list{nullptr};
    if (gp_port_info_list_new(&port_info_list) < GP_OK)
    {
        LOG_ERROR(gphoto2log, "could not allocate GPPortInfoList");
        return ptr;
    }
    ptr.reset(port_info_list);

    if (gp_port_info_list_load(port_info_list) < GP_OK)
    {
        LOG_ERROR(gphoto2log, "could not load GPPortInfoList");
        return {nullptr, gp_port_info_list_free};
    }
    if (gp_port_info_list_count(port_info_list) < GP_OK)
    {
        LOG_ERROR(gphoto2log, "could not count GPPortInfoList");
        return {nullptr, gp_port_info_list_free};
    }
    return ptr;
}

void CameraDeleter::operator()(Camera *c) const
{
    if (c != nullptr and context != nullptr)
    {
        LOG_INFO(gphoto2log, "Camera will exit now.");
        gp_camera_exit(c, context);
    }
    gp_camera_free(c);
}

} // namespace Pbox::GPhoto2
