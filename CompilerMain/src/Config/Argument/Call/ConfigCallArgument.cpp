#include "Config/Argument/Call/ConfigCallArgument.h"

namespace Sierra::OdyCompiler::v1::Config::Argument::Call {
    template<typename T>
    ConfigCallArgument<T>::ConfigCallArgument()
        : BaseConfigArgument<T>(std::nullopt) {
    }

    template<typename T>
    std::optional<T> ConfigCallArgument<T>::parse() {
        return std::nullopt;
    }
} // Sierra::v1::Compiler::Config::Call