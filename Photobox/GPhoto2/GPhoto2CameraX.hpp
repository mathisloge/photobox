#pragma once
#include <memory>
#include <qtclasshelpermacros.h>
#include "GPhoto2Context.hpp"
#include <gphoto2-abilities-list.h>
#include <gphoto2-camera.h>

namespace Pbox
{

using CameraAbilitiesListUniquePtr = std::unique_ptr<CameraAbilitiesList, decltype(&gp_abilities_list_free)>;
CameraAbilitiesListUniquePtr make_camera_abilities_list();

struct CameraDeleter
{
    GPhoto2ContextSharedPtr context;
    void operator()(Camera *c) const;
};
using CameraUniquePtr = std::unique_ptr<Camera, CameraDeleter>;
CameraUniquePtr make_gphoto2_camera();
using GPPortInfoListUniquePtr = std::unique_ptr<GPPortInfoList, decltype(&gp_port_info_list_free)>;
GPPortInfoListUniquePtr make_gphoto2_port_info_list();

class GPhoto2CameraX
{
  public:
    GPhoto2CameraX();
    ~GPhoto2CameraX();
    Q_DISABLE_COPY_MOVE(GPhoto2CameraX);

    GPContext *context();
    Camera *camera();
    bool autodetectAndConnectCamera();
    void closeCamera();

  private:
    GPhoto2ContextSharedPtr context_{make_gphoto2_context()};
    CameraAbilitiesListUniquePtr camera_abilities_list_{make_camera_abilities_list()};
    GPPortInfoListUniquePtr port_list_{make_gphoto2_port_info_list()};
    CameraUniquePtr camera_{nullptr, CameraDeleter{context_}};
    CameraAbilities camera_abilities_;
};
} // namespace Pbox