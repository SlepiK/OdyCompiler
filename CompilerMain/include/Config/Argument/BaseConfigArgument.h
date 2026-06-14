#ifndef SIERRA_ODYCOMPILER_V1_CONFIG_ARGUMENT_BASECONFIGARGUMENT_H
#define SIERRA_ODYCOMPILER_V1_CONFIG_ARGUMENT_BASECONFIGARGUMENT_H

#include <string>
#include <typeindex>

#include "IConfigArgument.h"

namespace Sierra::OdyCompiler::v1::Config::Argument {
    class BaseConfigArgument : public IConfigArgument {
    public:
        BaseConfigArgument(bool is_required, std::string key, std::type_index type);

        ~BaseConfigArgument() override = default;

        bool isRequired() override;

        std::type_index getType() override;

    protected:
        bool m_is_required;
        std::string m_key;
        std::type_index m_type;
    };
} // Sierra::v1::Compiler::Config

#endif //SIERRA_ODYCOMPILER_V1_CONFIG_ARGUMENT_BASECONFIGARGUMENT_H