#ifndef SIERRA_ODYCOMPILER_V1_CONFIG_ARGUMENT_CONFIGARGUMENT_H
#define SIERRA_ODYCOMPILER_V1_CONFIG_ARGUMENT_CONFIGARGUMENT_H

#include <optional>

#include "BaseConfigArgument.h"

namespace Sierra::OdyCompiler::v1::Config::Argument {
    template <typename T>
    class ConfigArgument : public BaseConfigArgument {
    public:
        ConfigArgument(T value, std::string key, bool is_required = true);

        ~ConfigArgument() override = default;

        std::optional<T> getValue();

    private:
        std::optional<T> m_value;
    };
} // Sierra::v1::Compiler::Config::Argument

#endif //SIERRA_ODYCOMPILER_V1_CONFIG_ARGUMENT_CONFIGARGUMENT_H