#pragma once
#include <memory>
#include <gphoto2-list.h>

namespace Pbox
{
using gphoto_list_unique_ptr = std::unique_ptr<CameraList, decltype(&gp_list_free)>;

gphoto_list_unique_ptr make_camera_list();

} // namespace Pbox