#pragma once

#define PBOX_DISABLE_COPY(ClassName)                                                                                   \
    ClassName(const ClassName &) = delete;                                                                             \
    ClassName &operator=(const ClassName &) = delete;

#define PBOX_DISABLE_COPY_MOVE(ClassName)                                                                              \
    PBOX_DISABLE_COPY(ClassName)                                                                                       \
    ClassName(ClassName &&) = delete;                                                                                  \
    ClassName &operator=(ClassName &&) = delete;
