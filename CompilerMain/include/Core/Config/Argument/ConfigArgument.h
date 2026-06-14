#ifndef SIERRA_ODYCOMPILER_V1_CORE_CONFIG_ARGUMENT_CONFIGARGUMENT_H
#define SIERRA_ODYCOMPILER_V1_CORE_CONFIG_ARGUMENT_CONFIGARGUMENT_H

#include <optional>

#include "BaseConfigArgument.h"

namespace Sierra::OdyCompiler::v1::Core::Config::Argument {
    using IntConfigArgument = BaseConfigArgument<int>;
    using StringConfigArgument = BaseConfigArgument<std::string>;
    using BooleanConfigArgument = BaseConfigArgument<bool>;
} // Sierra::OdyCompiler::v1::Core::Config::Argument

#endif //SIERRA_ODYCOMPILER_V1_CORE_CONFIG_ARGUMENT_CONFIGARGUMENT_H