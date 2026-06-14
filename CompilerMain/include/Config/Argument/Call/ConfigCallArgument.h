#ifndef SIERRA_ODYCOMPILER_V1_CONFIG_ARGUMENT_CALL_CONFIGCALLARGUMENT_H
#define SIERRA_ODYCOMPILER_V1_CONFIG_ARGUMENT_CALL_CONFIGCALLARGUMENT_H

#include <string>

#include "../ConfigArgument.h"

namespace Sierra::OdyCompiler::v1::Config::Argument::Call {
    template <typename T>
    class ConfigCallArgument : public ConfigArgument<T>{
    public:
        explicit ConfigCallArgument();

    private:
        std::optional<T> parse();

    };
} // Sierra::v1::Compiler::Config::Call::Argument

#endif //SIERRA_ODYCOMPILER_V1_CONFIG_ARGUMENT_CALL_CONFIGCALLARGUMENT_H