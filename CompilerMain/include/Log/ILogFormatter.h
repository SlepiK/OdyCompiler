
#ifndef SIERRA_V1_COMPILER_ILOGFORMATTER_H
#define SIERRA_V1_COMPILER_ILOGFORMATTER_H
#include <string>
#include <format>

#include "LogEvent.h"

namespace sierra::v1::compiler::log {
    class ILogFormatter {
    public:
        virtual ~ILogFormatter() = default;

        virtual std::string format(const LogEvent& log_event) = 0;

        virtual void format(const LogEvent& log_event, std::string& buffer) = 0;
    };
} // sierra::v1::compiler::log

#endif //SIERRA_V1_COMPILER_ILOGFORMATTER_H