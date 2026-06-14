#ifndef SIERRA_ODYCOMPILER_V1_CORE_CONFIG_ARGUMENT_ICONFIGDYNAMICARGUMENT_H
#define SIERRA_ODYCOMPILER_V1_CORE_CONFIG_ARGUMENT_ICONFIGDYNAMICARGUMENT_H

#include <expected>
#include <string>

#include "IConfigStaticArgument.h"

namespace Sierra::OdyCompiler::v1::Core::Config::Argument {
    class IConfigDynamicArgument : public IConfigStaticArgument {
    public:
        ~IConfigDynamicArgument() override = default;

        virtual std::expected<void, std::string> parse(const std::string& raw) = 0;
    };
} // Sierra::OdyCompiler::v1::Core::Config::Argument

#endif //SIERRA_ODYCOMPILER_V1_CORE_CONFIG_ARGUMENT_ICONFIGDYNAMICARGUMENT_H