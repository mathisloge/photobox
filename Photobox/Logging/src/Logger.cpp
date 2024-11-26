// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Pbox/Logger.hpp"
#include <quill/Backend.h>
#include <quill/Frontend.h>
#include <quill/sinks/ConsoleSink.h>
#include "Pbox/SetupLogging.hpp"

namespace Pbox
{
quill::Logger *createLogger(const std::string &logger_name)
{
    auto console_sink = quill::Frontend::create_or_get_sink<quill::ConsoleSink>("console_sink");

    auto &&logger = quill::Frontend::create_or_get_logger(logger_name, std::move(console_sink));

    logger->set_log_level(quill::LogLevel::Debug);

    return logger;
}

void setupLogging()
{
    quill::Backend::start();
    quill::Frontend::preallocate();

    createLogger(kRootLogger);
}

} // namespace Pbox
