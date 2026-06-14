#ifndef SIERRA_ODYCOMPILER_V1_CORE_CONFIG_ARGUMENT_BASECONFIGARGUMENT_H
#define SIERRA_ODYCOMPILER_V1_CORE_CONFIG_ARGUMENT_BASECONFIGARGUMENT_H

#include <charconv>
#include <string>
#include <typeindex>
#include <optional>
#include <expected>

#include "IConfigArgument.h"

namespace Sierra::OdyCompiler::v1::Core::Config::Argument {
    template <typename T>
    class BaseConfigArgument : public IConfigArgument {
    public:
        BaseConfigArgument(std::string key, T default_value, bool is_required = false)
            : m_is_required(is_required),
            m_key(std::move(key)),
            m_type(typeid(T)),
            m_value(default_value) {
        }

        ~BaseConfigArgument() override = default;

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

        std::expected<void, std::string> parse(const std::string& raw) override {
            if constexpr (std::is_same_v<T, std::string>) {
                this->m_value = raw;
            } else if constexpr (std::is_same_v<T, bool>) {
                if (raw == "true" || raw == "1") this->m_value = true;
                else if (raw == "false" || raw == "0") this->m_value = false;
                else return std::unexpected("Cannot parse \"" +raw +"\" as bool");
            } else {
                T value;
                auto [ptr, ec] = std::from_chars(raw.data(), raw.data() + raw.size(), value);
                if (ec == std::errc::invalid_argument) {
                    return std::unexpected("Cannot parse \"" +raw +"\"");
                }
                if (ec == std::errc::result_out_of_range) {
                    return std::unexpected("Value \"" +raw +"\" is out of range");
                }
                this->m_value = value;
            }
            return {};
        }

    protected:
        bool m_is_required;
        std::string m_key;
        std::type_index m_type;
        std::optional<T> m_value;
    };
} // Sierra::OdyCompiler::v1::Core::Config::Argument

#endif //SIERRA_ODYCOMPILER_V1_CORE_CONFIG_ARGUMENT_BASECONFIGARGUMENT_H