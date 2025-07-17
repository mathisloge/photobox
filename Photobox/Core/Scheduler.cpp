// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathis.opensource@tuta.io>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Scheduler.hpp"
#include <QCoreApplication>

namespace Pbox
{
auto Scheduler::getQtEventLoopScheduler() -> QThreadScheduler
{
    return qthread_as_scheduler(QCoreApplication::instance()->thread());
}
} // namespace Pbox
