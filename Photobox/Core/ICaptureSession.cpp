#include "ICaptureSession.hpp"

namespace Pbox
{
bool ICaptureSession::isIdle() const
{
    return idle_;
}

void ICaptureSession::setIdle(bool idle)
{
    if (idle_ != idle)
    {
        idle_ = idle;
        Q_EMIT idleChanged();
    }
}
} // namespace Pbox
