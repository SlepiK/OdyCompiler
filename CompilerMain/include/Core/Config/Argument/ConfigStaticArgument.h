#ifndef SIERRA_ODYCOMPILER_V1_CORE_CONFIG_ARGUMENT_CONFIGARGUMENT_H
#define SIERRA_ODYCOMPILER_V1_CORE_CONFIG_ARGUMENT_CONFIGARGUMENT_H

#include <optional>

#include "BaseConfigStaticArgument.h"

namespace Sierra::OdyCompiler::v1::Core::Config::Argument {
    using IntConfigArgument = BaseConfigStaticArgument<int>;
    using StringConfigArgument = BaseConfigStaticArgument<std::string>;
    using BooleanConfigArgument = BaseConfigStaticArgument<bool>;
} // Sierra::OdyCompiler::v1::Core::Config::Argument

#endif //SIERRA_ODYCOMPILER_V1_CORE_CONFIG_ARGUMENT_CONFIGARGUMENT_H