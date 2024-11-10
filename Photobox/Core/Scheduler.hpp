#pragma once
#include <exec/static_thread_pool.hpp>

namespace Pbox
{
struct Scheduler
{
    auto getScheduler()
    {
        return thread_pool_.get_scheduler();
    }

    exec::static_thread_pool thread_pool_{4};
};
} // namespace Pbox
