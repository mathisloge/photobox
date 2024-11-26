// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Scheduler.hpp"
#include <QCoreApplication>

namespace Pbox
{
auto Scheduler::getQtEventLoopScheduler() -> QThreadScheduler
{
    return qThreadAsScheduler(QCoreApplication::instance()->thread());
}
} // namespace Pbox
