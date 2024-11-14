#include "Scheduler.hpp"
#include <QCoreApplication>

namespace Pbox
{
auto Scheduler::getQtEventLoopScheduler() -> QThreadScheduler
{
    return qThreadAsScheduler(QCoreApplication::instance()->thread());
}
} // namespace Pbox
