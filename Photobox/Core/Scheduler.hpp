#pragma once
#include <Pbox/QStdexec.hpp>
#include <exec/single_thread_context.hpp>
#include <exec/static_thread_pool.hpp>
namespace Pbox
{
struct Scheduler
{
    /**
     * @brief General scheduler to dispatch any workload.
     * @todo Sooner or later asio is required (at least when adding the native esphome api). Therefore just switch to
     * the exec::asio_thread_pool. Then any timed workload can be dispatched to. e.g. the libgphoto2 integration now
     * takes the whole workload as it just runs blocking with a busy poll.
     */
    auto getWorkScheduler()
    {
        return compute_thread_pool_.get_scheduler();
    }

    /**
     * @brief The svg libs uses a lot of thread local storage (e.g. for fonts etc.). Therefore any rendering
     * of the svg should happen with the svg scheduler. The update can happen anywhere.
     */
    auto getSvgRenderScheduler()
    {
        return svg_thread_pool_.get_scheduler();
    }

    QThreadScheduler getQtEventLoopScheduler();

    exec::static_thread_pool compute_thread_pool_{2};
    exec::single_thread_context svg_thread_pool_;
};
} // namespace Pbox
