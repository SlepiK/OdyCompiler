#include "Config/BaseConfig.h"

namespace Sierra::OdyCompiler::v1::Config {
    Argument::IConfigArgument& BaseConfig::registerArgument(std::unique_ptr<Argument::IConfigArgument> argument) {
        auto& ref = *argument;
        this->m_arguments.emplace(argument->getKey(), std::move(argument));
        return ref;
    }
} // Sierra::v1::Compiler::Config