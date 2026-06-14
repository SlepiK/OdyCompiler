#ifndef SIERRA_ODYCOMPILER_V1_CORE_CONFIG_ICONFIG_H
#define SIERRA_ODYCOMPILER_V1_CORE_CONFIG_ICONFIG_H

#include <memory>
#include <expected>

#include "Core/Config/Argument/IConfigArgument.h"

namespace Sierra::OdyCompiler::v1::Core::Config {
    class IConfig {
    public:
        virtual ~IConfig() = default;

        virtual std::expected<void, std::string> validate() = 0;

        virtual std::expected<void, std::string> parse(int argc, char* argv[]) = 0;

    protected:
        virtual Argument::IConfigArgument& registerArgument(std::unique_ptr<Argument::IConfigArgument> argument) = 0;

    };
} // Sierra::OdyCompiler::v1::Core::Config

#endif //SIERRA_ODYCOMPILER_V1_CORE_CONFIG_ICONFIG_H