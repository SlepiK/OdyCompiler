
#include <memory>
#include "Log/Logger.h"
#include "Log/ILog.h"

namespace sierra::v1::compiler::log {
    void Logger::setLog(std::unique_ptr<ILog> newLog) {
        log_sinks.push_back(std::move(newLog));
    }

    void Logger::debug(std::string_view message, std::source_location current_location) const {
        log(LogLevel::DEBUG, message, current_location);
    }

    void Logger::info(std::string_view message, std::source_location current_location) const {
        log(LogLevel::INFORMATION, message, current_location);
    }

    void Logger::warn(std::string_view message, std::source_location current_location) const {
        log(LogLevel::WARNING, message, current_location);
    }

    void Logger::error(std::string_view message, std::source_location current_location) const {
        log(LogLevel::ERROR, message, current_location);
    }

    void Logger::log(LogLevel log_level, std::string_view log_message, const std::source_location current_location) const {
        time_t tTime;
        time(&tTime);
        tm* lTime = localtime(&tTime);

        const LogEvent log_event {
            log_level,
            std::string(log_message),
            current_location,
            std::chrono::system_clock::now(),
        };

        for (const auto& log_sink : log_sinks ) {
            log_sink->log(log_event);
        }
    }

} // sierra::v1::compiler::log