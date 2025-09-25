// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <quill/LogMacros.h>
#include <quill/Logger.h>
#include "LoggingExport.hpp"

namespace Pbox
{
constexpr std::string kRootLogger{"general"};

PHOTOBOXLOGGING_EXPORT quill::Logger *createLogger(const std::string &logger_name);
} // namespace Pbox

#define DEFINE_LOGGER(name)                                                                                            \
    namespace                                                                                                          \
    {                                                                                                                  \
    quill::Logger *logger_##name()                                                                                     \
    {                                                                                                                  \
        static auto *name_logger = ::Pbox::createLogger(#name);                                                        \
        return name_logger;                                                                                            \
    }                                                                                                                  \
    }

#define DEFINE_ROOT_LOGGER(name)                                                                                       \
    namespace                                                                                                          \
    {                                                                                                                  \
    quill::Logger *logger_##name()                                                                                     \
    {                                                                                                                  \
        static auto *name_logger = ::Pbox::createLogger(::Pbox::kRootLogger);                                          \
        return name_logger;                                                                                            \
    }                                                                                                                  \
    }
