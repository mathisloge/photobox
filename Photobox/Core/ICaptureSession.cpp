#include "ICaptureSession.hpp"

namespace Pbox
{
ICaptureSession::Status ICaptureSession::getStatus() const
{
    return status_;
}

void ICaptureSession::setStatus(ICaptureSession::Status status)
{
    if (status_ != status)
    {
        status_ = status;
        Q_EMIT statusChanged();
    }
}
} // namespace Pbox
