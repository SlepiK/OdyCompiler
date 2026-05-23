
#ifndef SIERRA_V1_COMPILER_ILOG_H
#define SIERRA_V1_COMPILER_ILOG_H
#include "Log/LogEvent.h"

namespace sierra::v1::compiler::log {
    class ILog {
        public:
            virtual ~ILog() = default;

            virtual void log(const LogEvent& log_event) = 0;
    };
} // sierra::v1::compiler::log

#endif //SIERRA_V1_COMPILER_ILOG_H