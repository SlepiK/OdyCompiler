#ifndef SIERRA_ODYCOMPILER_V1_CONFIG_ICONFIG_H
#define SIERRA_ODYCOMPILER_V1_CONFIG_ICONFIG_H

#include <memory>

#include "Argument/IConfigArgument.h"

namespace Sierra::OdyCompiler::v1::Config {
    class IConfig {
    public:
        virtual ~IConfig() = default;

        virtual void validate() = 0;

    protected:
        virtual Argument::IConfigArgument& registerArgument(std::unique_ptr<Argument::IConfigArgument> argument) = 0;

    };
} // Sierra::v1::Compiler::Config

#endif //SIERRA_ODYCOMPILER_V1_CONFIG_ICONFIG_H