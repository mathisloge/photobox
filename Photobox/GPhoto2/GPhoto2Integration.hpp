#pragma once
#include <optional>

class QImage;
namespace Pbox::GPhoto2
{
struct Context;

bool autodetectAndConnectCamera(Context &context);

std::optional<QImage> capturePreviewImage(Context &context);

std::optional<QImage> captureImage(Context &context);
} // namespace Pbox::GPhoto2
