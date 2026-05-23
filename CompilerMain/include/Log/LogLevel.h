
#ifndef SIERRA_V1_COMPILER_LOGLEVEL_H
#define SIERRA_V1_COMPILER_LOGLEVEL_H
#include <array>
#include <ostream>

namespace sierra::v1::compiler::log {
    enum class LogLevel : int {
        TRACE,
        DEBUG,
        INFORMATION,
        WARNING,
        ERROR,
        CRITICAL,
        FATAL,
        _COUNT
    };

    constexpr std::array<std::string_view,7> LogLevelNames {
        "trace",
        "debug",
        "info",
        "warning",
        "error",
        "critical",
        "fatal"
    };

    static_assert(LogLevelNames.size() == static_cast<size_t>(LogLevel::_COUNT));

    constexpr size_t index(LogLevel log_level) {
        return static_cast<size_t>(log_level);
    }

    inline std::ostream& operator<< (std::ostream& ostream, LogLevel log_level) {
        if (const auto idx = index(log_level); idx >= LogLevelNames.size()) {
            ostream << "unknown";
        } else {
            ostream << LogLevelNames[idx];
        }

        return ostream;
    }

    constexpr std::string_view to_string(const LogLevel& log_level) {
        const auto idx = index(log_level);

        if (idx >= LogLevelNames.size()) {
            return "unknown";
        }

        return LogLevelNames[idx];
    }

} // sierra::v1::compiler::log

#endif //SIERRA_V1_COMPILER_LOGLEVEL_H