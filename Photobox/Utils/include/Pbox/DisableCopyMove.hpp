// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#define PBOX_DISABLE_COPY(ClassName)                                                                                   \
    ClassName(const ClassName &) = delete;                                                                             \
    ClassName &operator=(const ClassName &) = delete;

#define PBOX_DISABLE_COPY_MOVE(ClassName)                                                                              \
    PBOX_DISABLE_COPY(ClassName)                                                                                       \
    ClassName(ClassName &&) = delete;                                                                                  \
    ClassName &operator=(ClassName &&) = delete;
