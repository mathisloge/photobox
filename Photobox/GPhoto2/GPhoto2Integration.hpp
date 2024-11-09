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

inline auto flowAutoconnect()
{
    return stdexec::then([]() {
               Context context{};
               if (gp_abilities_list_load(context.camera_abilities_list.get(), context.context.get()) < GP_OK)
               {
                   // throw error?
               }
               return context;
           }) |
           stdexec::then([](auto &&context) {
               while (not autodetectAndConnectCamera(context))
               {
               }
               return context;
           });
}
} // namespace Pbox::GPhoto2
