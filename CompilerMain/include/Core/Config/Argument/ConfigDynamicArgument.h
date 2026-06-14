#ifndef SIERRA_ODYCOMPILER_V1_CORE_CONFIG_ARGUMENT_CONFIGCALLARGUMENT_H
#define SIERRA_ODYCOMPILER_V1_CORE_CONFIG_ARGUMENT_CONFIGCALLARGUMENT_H

#include "BaseConfigDynamicArgument.h"

namespace Sierra::OdyCompiler::v1::Core::Config::Argument {
    using IntConfigArgument = BaseConfigDynamicArgument<int>;
    using StringConfigArgument = BaseConfigDynamicArgument<std::string>;
    using BooleanConfigArgument = BaseConfigDynamicArgument<bool>;
} // Sierra::OdyCompiler::v1::Core::Config::Argument

#endif //SIERRA_ODYCOMPILER_V1_CORE_CONFIG_ARGUMENT_CONFIGCALLARGUMENT_H