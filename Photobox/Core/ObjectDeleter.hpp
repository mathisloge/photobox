// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QObject>
#include <QThread>
#include <type_traits>

namespace Pbox::Details
{
/**
 * @brief Qt-aware deleter: uses deleteLater() if the QObject
 * lives in a running QThread (so its event loop can process
 * the deletion), else deletes immediately.
 */
template <typename T>
struct ObjectDeleter
{
    ObjectDeleter() noexcept = default;
    // Allow conversion between compatible deleter types
    template <typename U, std::enable_if_t<std::is_base_of_v<T, U>, int> = 0>
    ObjectDeleter(const ObjectDeleter<U> & /*other*/) noexcept
    {}

    void operator()(T *ptr) const noexcept
    {
        if (ptr == nullptr)
        {
            return;
        }

        // If it's not a QObject-derived type, just delete.
        if constexpr (!std::is_base_of_v<QObject, T>)
        {
            delete ptr; // NOLINT(cppcoreguidelines-owning-memory)
            return;
        }

        // QObject-specific path:
        QThread *thr = ptr->thread();
        if ((thr != nullptr) and thr->isRunning())
        {
            QMetaObject::invokeMethod(ptr, &QObject::deleteLater);
        }
        else
        {
            // No running event loop to handle deleteLater()
            delete ptr; // NOLINT(cppcoreguidelines-owning-memory)
        }
    }
};
} // namespace Pbox::Details
