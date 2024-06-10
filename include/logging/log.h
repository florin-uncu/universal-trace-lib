#pragma once
#include "EventBuilder.h"

// This is more a demo than a legit header file to be delivered with the library
// More likely an application would have extra information for each log event.
// E.g., source code filename, line, function name
#define LOG_DBG ::logging::EventBuilder{ logging::Severity::debug }
#define LOG_INF ::logging::EventBuilder{ logging::Severity::info }
#define LOG_WRN ::logging::EventBuilder{ logging::Severity::warning }
#define LOG_ERR ::logging::EventBuilder{ logging::Severity::error }
#define LOG_FAT ::logging::EventBuilder{ logging::Severity::fatal }

// Example:
#include <string>
static inline std::string generate_log_header(const char* file, int line)
{
    return file + std::to_string(line);
}

#define LOG_DEBUG (LOG_DBG << generate_log_header(__FILE__, __LINE__))
