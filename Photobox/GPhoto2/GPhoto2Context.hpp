#pragma once
#include <memory>
#include <gphoto2-context.h>

namespace Pbox
{
using GPhoto2ContextSharedPtr = std::shared_ptr<GPContext>;

[[nodiscard]] GPhoto2ContextSharedPtr make_gphoto2_context();
}; // namespace Pbox