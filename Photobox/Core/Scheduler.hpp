// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <Pbox/QStdexec.hpp>
#include <execpools/asio/asio_thread_pool.hpp>
#include "Pbox/DisableCopyMove.hpp"
namespace Pbox
{
class Scheduler final
{
  public:
    PBOX_DISABLE_COPY_MOVE(Scheduler);
    Scheduler() = default;
    ~Scheduler() = default;

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

    auto getWorkExecutor() const
    {
        return compute_thread_pool_.get_executor();
    }

    /**
     * @brief The svg libs uses a lot of thread local storage (e.g. for fonts etc.). Therefore any rendering
     * of the svg should happen with the svg scheduler. The update can happen anywhere.
     */
    auto getSvgRenderScheduler()
    {
        return compute_thread_pool_.get_scheduler();
    }

    /**
     * @brief All interaction with the qt event loop should happen in this context.
     */
    QThreadScheduler getQtEventLoopScheduler();

  private:
    execpools::asio_thread_pool compute_thread_pool_{4};
};
} // namespace Pbox
