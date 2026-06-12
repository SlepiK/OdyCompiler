#include "Log/Standard/StandardLogFormatter.h"

namespace sierra::v1::compiler::log::standard {
    std::string LogFormatter::format(const LogEvent& log_event) {
        std::string result;
        format(log_event, result);
        return result;
    }

    void LogFormatter::format(const LogEvent& log_event, std::string& buffer) {
        buffer.clear();
        buffer.reserve(128);

        buffer.append("[");
        buffer.append(std::format("{:%H:%M:%S %d-%m-%Y}", log_event.timestamp));
        buffer.append("]");

        buffer.append("[");
        buffer.append(log::to_string(log_event.level));
        buffer.append("]");

        buffer.append("[");
        buffer.append(log_event.location.file_name());
        buffer.append(":");
        buffer.append(std::to_string(log_event.location.line()));
        buffer.append("]: ");

        buffer.append(log_event.message());
    }
} // sierra::v1::compiler::log::standard