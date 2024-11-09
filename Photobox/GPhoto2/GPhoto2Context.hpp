#pragma once
#include <memory>
#include "GPhoto2Wrapper.hpp"
#include <gphoto2-context.h>

namespace Pbox
{
using GPhoto2ContextSharedPtr = std::shared_ptr<GPContext>;

[[nodiscard]] GPhoto2ContextSharedPtr make_gphoto2_context();
}; // namespace Pbox

namespace Pbox::GPhoto2
{
struct Context
{
    ContextUniquePtr context{makeUniqueContext()};
    CameraAbilitiesListUniquePtr camera_abilities_list{makeUniqueCameraListAbilities()};
    PortInfoListUniquePtr port_list{makeUniquePortInfoList()};
    CameraUniquePtr camera{nullptr, CameraDeleter{context.get()}};
    CameraAbilities camera_abilities{};
};
} // namespace Pbox::GPhoto2
