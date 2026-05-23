#ifndef SIERRA_V1_COMPILER_LOG_STANDARD_CONSOLELOG_H
#define SIERRA_V1_COMPILER_LOG_STANDARD_CONSOLELOG_H
#include <iostream>
#include <memory>
#include "Log/ILog.h"
#include "StandardLogFormatter.h"

namespace sierra::v1::compiler::log::standard {
    class ConsoleLog : public ILog {
    public:
        ConsoleLog() = default;

        ~ConsoleLog() override = default;

        void log(const LogEvent& log_event) final;

    private:
        std::unique_ptr<standard::LogFormatter> formatter;

    };
} // sierra::v1::compiler::log::standard

#endif //SIERRA_V1_COMPILER_LOG_STANDARD_CONSOLELOG_H