#ifndef SIERRA_ODYCOMPILER_V1_COMPILER_CONFIG_COMPILERCONFIG_H
#define SIERRA_ODYCOMPILER_V1_COMPILER_CONFIG_COMPILERCONFIG_H

#include "Core/Config/BaseConfig.h"

namespace Sierra::OdyCompiler::v1::Compiler::Config {
    class CompilerConfig : public Core::Config::BaseConfig {
    public:
        CompilerConfig();

        ~CompilerConfig() override = default;
    };
}

#endif //SIERRA_ODYCOMPILER_V1_COMPILER_CONFIG_COMPILERCONFIG_H