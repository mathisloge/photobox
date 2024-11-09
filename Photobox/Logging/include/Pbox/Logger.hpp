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
    static quill::Logger *(name) = ::Pbox::createLogger(#name);                                                        \
    }

#define DEFINE_ROOT_LOGGER(name)                                                                                       \
    namespace                                                                                                          \
    {                                                                                                                  \
    static quill::Logger *(name) = ::Pbox::createLogger(::Pbox::kRootLogger);                                          \
    }
