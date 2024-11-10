#include "GPhoto2Context.hpp"
#include <Pbox/Logger.hpp>

DEFINE_LOGGER(gphoto2log);

namespace Pbox::GPhoto2
{
Context::Context()
{
    if (gp_abilities_list_load(camera_abilities_list.get(), context.get()) < GP_OK)
    {
        LOG_ERROR(gphoto2log, "Could not load abilities");
    }
}
} // namespace Pbox::GPhoto2
