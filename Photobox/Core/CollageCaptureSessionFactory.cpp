#include "CollageCaptureSessionFactory.hpp"
#include "CollageCaptureSession.hpp"

namespace Pbox
{
CaptureSessionPtr CollageCaptureSessionFactory::create() const
{
    return make_unique_object_ptr_as<ICaptureSession, CollageCaptureSession>(
        image_storage_, renderer_, scheduler_, collage_settings_);
}
} // namespace Pbox
