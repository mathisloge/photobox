#include "GPhoto2Context.hpp"
#include <QDebug>

namespace
{
void handleGPhoto2Error(GPContext * /*context*/, const char *str, void * /*data*/)
{
    qDebug() << "gphoto2 error:" << str;
}

void handleGPhoto2Status(GPContext * /*context*/, const char *str, void * /*data*/)
{
    qDebug() << "gphoto2 status:" << str;
}
} // namespace
namespace Pbox
{
GPhoto2ContextSharedPtr make_gphoto2_context()
{
    GPhoto2ContextSharedPtr ctx{gp_context_new(), &gp_context_unref};

    gp_context_set_error_func(ctx.get(), handleGPhoto2Error, NULL);
    gp_context_set_status_func(ctx.get(), handleGPhoto2Status, NULL);

    return ctx;
}
}; // namespace Pbox