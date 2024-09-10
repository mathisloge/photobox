#include "GPhoto2List.hpp"
#include <QDebug>
#include <gphoto2-port-result.h>

namespace Pbox
{
gphoto_list_unique_ptr make_camera_list()
{
    gphoto_list_unique_ptr cam_list{nullptr, gp_list_free};

    CameraList *list{nullptr};
    if (gp_list_new(&list) < GP_OK)
    {
        qDebug() << "could not allocate list";
    }
    cam_list.reset(list);

    return cam_list;
}
} // namespace Pbox