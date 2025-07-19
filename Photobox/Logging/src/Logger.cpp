// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathis.opensource@tuta.io>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Pbox/Logger.hpp"
#include <QString>
#include <QtLogging>
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

namespace
{
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static auto *logger = quill::Frontend::get_logger(kRootLogger);

    // map log level
    quill::LogLevel lvl{};
    switch (type)
    {
    case QtDebugMsg:
        lvl = quill::LogLevel::Debug;
        break;
    case QtInfoMsg:
        lvl = quill::LogLevel::Info;
        break;
    case QtWarningMsg:
        lvl = quill::LogLevel::Warning;
        break;
    case QtCriticalMsg:
        lvl = quill::LogLevel::Error;
        break;
    case QtFatalMsg:
        lvl = quill::LogLevel::Critical;
        break;
    default:
        lvl = quill::LogLevel::TraceL3;
    }

    // log
    LOG_RUNTIME_METADATA(logger, lvl, context.file, context.line, context.function, "{}", msg.toStdString());
}
} // namespace
void installQtMessageHandler()
{
    qInstallMessageHandler(messageHandler);
}

} // namespace Pbox
