#include "Log/Standard/StandardConsoleLog.h"

namespace sierra::v1::compiler::log::standard {
    void ConsoleLog::log(const LogEvent& log_event) {
        std::cout << this->formatter->format(log_event) << std::endl;
    }
} // sierra::v1::compiler::log::standard