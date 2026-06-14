#ifndef SIERRA_ODYCOMPILER_V1_CORE_CONFIG_ARGUMENT_BASECONFIGARGUMENT_H
#define SIERRA_ODYCOMPILER_V1_CORE_CONFIG_ARGUMENT_BASECONFIGARGUMENT_H

#include <string>
#include <typeindex>
#include <optional>
#include <expected>

#include "IConfigStaticArgument.h"

namespace Sierra::OdyCompiler::v1::Core::Config::Argument {
    template <typename T>
    class BaseConfigStaticArgument : public IConfigStaticArgument {
    public:
        BaseConfigStaticArgument(std::string key, T value, bool is_required = false)
            : m_is_required(is_required),
            m_key(std::move(key)),
            m_type(typeid(T)),
            m_value(value) {
        }

        BaseConfigStaticArgument(std::string key, bool is_required = false)
            : m_is_required(is_required),
            m_key(std::move(key)),
            m_type(typeid(T)),
            m_value(std::nullopt) {
        }

        ~BaseConfigStaticArgument() override = default;

        bool isAvailable() override {
            return this->m_value.has_value();
        }

        bool isRequired() override {
            return this->m_is_required;
        }

        std::type_index getType() override {
            return this->m_type;
        }

        std::string& getKey() override {
            return this->m_key;
        }

        std::expected<std::reference_wrapper<T>, std::string> getValue() {
            if (!this->m_value.has_value())
                return std::unexpected("Argument \"" +this->m_key +"\" has no value");
            return std::ref(*this->m_value);
        }

    protected:
        bool m_is_required;
        std::string m_key;
        std::type_index m_type;
        std::optional<T> m_value;
    };
} // Sierra::OdyCompiler::v1::Core::Config::Argument

#endif //SIERRA_ODYCOMPILER_V1_CORE_CONFIG_ARGUMENT_BASECONFIGARGUMENT_H