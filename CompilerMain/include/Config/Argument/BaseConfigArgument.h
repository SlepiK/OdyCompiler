#ifndef SIERRA_ODYCOMPILER_V1_CONFIG_ARGUMENT_BASECONFIGARGUMENT_H
#define SIERRA_ODYCOMPILER_V1_CONFIG_ARGUMENT_BASECONFIGARGUMENT_H

#include <string>
#include <typeindex>
#include <optional>
#include <expected>

#include "IConfigArgument.h"

namespace Sierra::OdyCompiler::v1::Config::Argument {
    template <typename T>
    class BaseConfigArgument : public IConfigArgument {
    public:
        BaseConfigArgument(std::string key, T value, bool is_required = false);

        ~BaseConfigArgument() override = default;

        bool isRequired() override;

        std::type_index getType() override;

        std::string& getKey() override;

        std::expected<std::reference_wrapper<T>, std::string> getValue();

    protected:
        bool m_is_required;
        std::string m_key;
        std::type_index m_type;

    private:
        std::optional<T> m_value;
    };
} // Sierra::v1::Compiler::Config

#endif //SIERRA_ODYCOMPILER_V1_CONFIG_ARGUMENT_BASECONFIGARGUMENT_H