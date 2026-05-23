
#ifndef SIERRA_V1_COMPILER_LOGGER_H
#define SIERRA_V1_COMPILER_LOGGER_H

#include <memory>
#include <vector>
#include <source_location>
#include <chrono>

#include "LogEvent.h"
#include "Log/ILog.h"
#include "Log/LogLevel.h"

namespace sierra::v1::compiler::log {
    class Logger {
    public:
        void setLog(std::unique_ptr<ILog> newLog);

        void debug(std::string_view message, std::source_location current_location = std::source_location::current()) const;
        void info(std::string_view message, std::source_location current_location = std::source_location::current()) const;
        void warn(std::string_view message, std::source_location current_location = std::source_location::current()) const;
        void error(std::string_view message, std::source_location current_location = std::source_location::current()) const;
        void log(LogLevel log_level, std::string_view log_message, std::source_location current_location = std::source_location::current()) const;

    private:
        std::vector<std::unique_ptr<ILog>> log_sinks;
    };
} // sierra::v1::compiler::log

#endif //SIERRA_V1_COMPILER_LOGGER_H