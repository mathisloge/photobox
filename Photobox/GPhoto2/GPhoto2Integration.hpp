#pragma once
#include <QImage>
#include <exec/repeat_effect_until.hpp>
#include <exec/repeat_n.hpp>
#include <stdexec/execution.hpp>
#include <stdexec/functional.hpp>
#include "GPhoto2Context.hpp"
namespace Pbox::GPhoto2
{

bool autodetectAndConnectCamera(Context &context);

std::optional<QImage> capturePreviewImage(Context &context);

std::optional<QImage> captureImage(Context &context);

inline auto flowAutoconnect()
{
    return stdexec::then([]() { return Context{}; }) | //
           stdexec::then([](auto &&context) {
               while (not autodetectAndConnectCamera(context))
               {
               }
               return context;
           });
}

inline auto flowCapturePreview(Pbox::GPhoto2::Context &context)
{
    return stdexec::then([&context]() -> QImage {
        std::optional<QImage> image;
        int error_count{0};
        while (not image.has_value() and error_count < 5)
        {
            image = Pbox::GPhoto2::capturePreviewImage(context);
            error_count++;
        }
        if (not image.has_value())
        {
            throw std::runtime_error("Could not take preview image");
        }

        return *image;
    });
}

} // namespace Pbox::GPhoto2
