#ifndef SIERRA_ODYCOMPILER_V1_CORE_CONFIG_ARGUMENT_BASECONFIGDYNAMICARGUMENT_H
#define SIERRA_ODYCOMPILER_V1_CORE_CONFIG_ARGUMENT_BASECONFIGDYNAMICARGUMENT_H

#include <charconv>
#include <functional>

#include "BaseConfigStaticArgument.h"
#include "IConfigDynamicArgument.h"

namespace Sierra::OdyCompiler::v1::Core::Config::Argument {
    template <typename T>
    class BaseConfigDynamicArgument : public IConfigDynamicArgument, public BaseConfigStaticArgument<T>{
    public:
        BaseConfigDynamicArgument(std::string key, bool is_required = false)
            : BaseConfigStaticArgument<T>(std::move(key), is_required) {
        }

        ~BaseConfigDynamicArgument() override = default;

        bool isAvailable() override {
            return BaseConfigStaticArgument<T>::isAvailable();
        }

        bool isRequired() override {
            return BaseConfigStaticArgument<T>::isRequired();
        }

        std::type_index getType() override {
            return BaseConfigStaticArgument<T>::getType();
        }

        std::string& getKey() override {
            return BaseConfigStaticArgument<T>::getKey();
        }

        std::expected<void, std::string> parse(const std::string& raw) override {
            if (this->m_value.has_value()) {
                return std::unexpected("Argument \"" +this->m_key +"\" already parsed");
            }

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
    };
} // Sierra::OdyCompiler::v1::Core::Config::Argument

#endif //SIERRA_ODYCOMPILER_V1_CORE_CONFIG_ARGUMENT_BASECONFIGDYNAMICARGUMENT_H