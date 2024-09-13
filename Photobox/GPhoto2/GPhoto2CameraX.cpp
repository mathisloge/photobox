#include "GPhoto2CameraX.hpp"
#include <QDebug>
#include "GPhoto2List.hpp"
namespace Pbox
{

GPhoto2CameraX::GPhoto2CameraX()
{
    Q_ASSERT(context_ != nullptr);
    Q_ASSERT(camera_abilities_list_ != nullptr);
    Q_ASSERT(port_list_ != nullptr);
    if (gp_abilities_list_load(camera_abilities_list_.get(), context_.get()) < GP_OK)
    {
        qCritical() << "Could not load all camera abilities list";
    }
}

GPhoto2CameraX::~GPhoto2CameraX() = default;

bool GPhoto2CameraX::autodetectAndConnectCamera()
{
    auto cam_list = make_camera_list();
    const auto auto_detect_count = gp_camera_autodetect(cam_list.get(), context_.get());
    if (auto_detect_count <= GP_OK)
    {
        qDebug() << "got no possible cameras";
        return false;
    }
    qDebug() << "auto connect result" << auto_detect_count;
    const char *model_name;
    const char *port_value;
    gp_list_get_name(cam_list.get(), 0, &model_name);
    gp_list_get_value(cam_list.get(), 0, &port_value);
    qDebug() << "model_name=" << model_name << port_value;

    const auto camera_index = gp_abilities_list_lookup_model(camera_abilities_list_.get(), model_name);
    if (camera_index < GP_OK)
    {
        qCritical() << "Could not lookup camera abilities";
        return false;
    }

    const auto port_lookup_result = gp_port_info_list_lookup_path(port_list_.get(), port_value);
    switch (port_lookup_result)
    {
    case GP_ERROR_UNKNOWN_PORT:
        qCritical() << "The port you specified "
                       "('"
                    << port_value
                    << "') can not be found. Please "
                       "specify one of the ports found by "
                       "'gphoto2 --list-ports' and make "
                       "sure the spelling is correct "
                       "(i.e. with prefix 'serial:' or 'usb:').";
        break;
    default:
        break;
    }
    if (port_lookup_result < GP_OK)
    {
        return false;
    }

    GPPortInfo port_info{};
    if (gp_port_info_list_get_info(port_list_.get(), port_lookup_result, &port_info) < GP_OK)
    {
        qCritical() << "Could not get port info";
        return false;
    }

    auto camera = make_gphoto2_camera();
    Q_ASSERT(camera != nullptr);

    if (gp_abilities_list_get_abilities(camera_abilities_list_.get(), camera_index, &camera_abilities_) < GP_OK)
    {
        qCritical() << "Could not get camera abilities";
        return false;
    }
    if (gp_camera_set_abilities(camera.get(), camera_abilities_) < GP_OK)
    {
        qCritical() << "Could not set camera abilities";
        return false;
    }

    if (gp_camera_set_port_info(camera.get(), port_info) < GP_OK)
    {
        qCritical() << "Could not set camera port info";
        return false;
    }

    camera_ = std::move(camera);
    return true;
}

void GPhoto2CameraX::closeCamera()
{
    camera_ = nullptr;
}

GPContext *GPhoto2CameraX::context()
{
    return context_.get();
}

Camera *GPhoto2CameraX::camera()
{
    return camera_.get();
}

CameraAbilitiesListUniquePtr make_camera_abilities_list()
{
    CameraAbilitiesListUniquePtr list{nullptr, gp_abilities_list_free};
    CameraAbilitiesList *abilities{nullptr};
    if (gp_abilities_list_new(&abilities) < GP_OK)
    {
        qCritical() << "Could not allocated gp_abilities_list_new";
        return list;
    }
    list.reset(abilities);
    return list;
}

CameraUniquePtr make_gphoto2_camera()
{
    CameraUniquePtr cam_ptr{nullptr, CameraDeleter{}};
    Camera *camera{nullptr};
    if (gp_camera_new(&camera) < GP_OK)
    {
        qCritical() << "could not allocate camera";
        return cam_ptr;
    }
    cam_ptr.reset(camera);
    return cam_ptr;
}

GPPortInfoListUniquePtr make_gphoto2_port_info_list()
{
    GPPortInfoListUniquePtr ptr{nullptr, gp_port_info_list_free};
    GPPortInfoList *port_info_list{nullptr};
    if (gp_port_info_list_new(&port_info_list) < GP_OK)
    {
        qCritical() << "could not allocate GPPortInfoList";
        return ptr;
    }
    ptr.reset(port_info_list);

    if (gp_port_info_list_load(port_info_list) < GP_OK)
    {
        qCritical() << "could not load GPPortInfoList";
        return {nullptr, gp_port_info_list_free};
    }
    if (gp_port_info_list_count(port_info_list) < GP_OK)
    {
        qCritical() << "could not count GPPortInfoList";
        return {nullptr, gp_port_info_list_free};
    }
    return ptr;
}

void CameraDeleter::operator()(Camera *c) const
{
    if (c != nullptr)
    {
        qDebug() << "camera exit";
        gp_camera_exit(c, context.get());
    }
    gp_camera_free(c);
}
} // namespace Pbox