
#ifndef SIERRA_V1_COMPILER_LOGEVENT_H
#define SIERRA_V1_COMPILER_LOGEVENT_H
#include <source_location>
#include <string>
#include <string_view>
#include <chrono>

#include "Log/LogLevel.h"

namespace sierra::v1::compiler::log {
    struct LogEvent {
        LogLevel level;
        std::string message_storage;
        std::source_location location;
        std::chrono::system_clock::time_point timestamp;

        [[nodiscard]] std::string_view message() const {
            return message_storage;
        }
    };
} // sierra::v1::compiler::log

#endif //SIERRA_V1_COMPILER_LOGEVENT_H