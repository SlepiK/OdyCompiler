#ifndef SIERRA_V1_COMPILER_LOG_STANDARD_LOGFORMATTER_H
#define SIERRA_V1_COMPILER_LOG_STANDARD_LOGFORMATTER_H
#include <string>

#include "Log/ILogFormatter.h"
#include "Log/LogEvent.h"
#include "Config/Config.h"

namespace sierra::v1::compiler::log::standard {
    class LogFormatter : public ILogFormatter {
    public:
        LogFormatter() = default;

        ~LogFormatter() override = default;

        std::string format(const LogEvent& log_event) final;

        void format(const LogEvent& log_event, std::string& buffer) final;

    };
} // sierra::v1::compiler::log::standard

#endif //SIERRA_V1_COMPILER_LOG_STANDARD_LOGFORMATTER_H