#include "GPhoto2Context.hpp"
#include "GPhoto2Exeption.hpp"
namespace Pbox::GPhoto2
{
Context::Context()
{
    if (gp_abilities_list_load(camera_abilities_list.get(), context.get()) < GP_OK)
    {
        throw GPhoto2Exception{"Could not load abilities"};
    }
}
} // namespace Pbox::GPhoto2
