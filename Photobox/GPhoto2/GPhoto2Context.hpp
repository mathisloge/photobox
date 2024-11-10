#pragma once
#include "GPhoto2Wrapper.hpp"
#include <gphoto2-context.h>

namespace Pbox::GPhoto2
{
struct Context
{
    Context();
    ContextUniquePtr context{makeUniqueContext()};
    CameraAbilitiesListUniquePtr camera_abilities_list{makeUniqueCameraListAbilities()};
    PortInfoListUniquePtr port_list{makeUniquePortInfoList()};
    CameraUniquePtr camera{nullptr, CameraDeleter{context.get()}};
    CameraAbilities camera_abilities{};
};
} // namespace Pbox::GPhoto2
