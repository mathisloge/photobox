// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <memory>
#include "ObjectDeleter.hpp"

namespace Pbox
{
/**
 * @brief Helper alias for unique_ptr with QtDeleter.
 */
template <typename T>
using unique_object_ptr = std::unique_ptr<T, Details::ObjectDeleter<T>>;

/*!
 * @brief Factory, akin to std::make_unique, that returns a unique_object_ptr<T> using a Details::ObjectDeleter<T>.
 */
template <typename T, typename... Args>
unique_object_ptr<T> make_unique_object_ptr(Args &&...args)
{
    // allocate with new, wrap in unique_ptr+deleter
    return unique_object_ptr<T>(new T{std::forward<Args>(args)...}, Details::ObjectDeleter<T>{});
}

template <typename Base, typename Derived, typename... Args>
std::unique_ptr<Base, Details::ObjectDeleter<Base>> make_unique_object_ptr_as(Args &&...args)
{
    static_assert(std::is_base_of_v<Base, Derived>, "Derived must inherit from Base");

    return std::unique_ptr<Base, Details::ObjectDeleter<Base>>{new Derived(std::forward<Args>(args)...),
                                                               Details::ObjectDeleter<Base>{}};
}
} // namespace Pbox
