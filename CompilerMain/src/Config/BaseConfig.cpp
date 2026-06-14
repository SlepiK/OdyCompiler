#include "Config/BaseConfig.h"

namespace Sierra::OdyCompiler::v1::Config {
    Argument::IConfigArgument& BaseConfig::registerArgument(std::unique_ptr<Argument::IConfigArgument> argument) {
        auto key = argument->getKey();
        auto [it, _ ] = this->m_arguments.emplace(key, std::move(argument));
        return *it->second;
    }
} // Sierra::v1::Compiler::Config