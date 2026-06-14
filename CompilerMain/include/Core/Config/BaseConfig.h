#ifndef SIERRA_ODYCOMPILER_V1_CORE_CONFIG_BASECONFIG_H
#define SIERRA_ODYCOMPILER_V1_CORE_CONFIG_BASECONFIG_H

#include <unordered_map>
#include <memory>
#include <string>

#include "Core/Config/IConfig.h"
#include "Core/Config/Argument/IConfigStaticArgument.h"
#include "Core/Config/Argument/BaseConfigDynamicArgument.h"
#include "Core/Config/Argument/BaseConfigStaticArgument.h"

namespace Sierra::OdyCompiler::v1::Core::Config {
    class BaseConfig : public IConfig {
    public:
        BaseConfig() = default;
        ~BaseConfig() override = default;

        std::expected<void, std::string> validate() override;

        std::expected<void, std::string> parseDynamicArguments(int argc, char* argv[]) override;

        template <typename T>
        std::expected<std::reference_wrapper<Argument::BaseConfigStaticArgument<T>>, std::string> getStaticArgument(const std::string& key) {
            const auto it = this->m_static_arguments.find(key);
            if (it == this->m_static_arguments.end()) {
                return std::unexpected("Unknown static argument \"" +key +"\"");
            }

            if (it->second->getType() != typeid(T)) {
                return std::unexpected("Type mismatch for static argument \"" +key +"\"");
            }

            return std::ref(static_cast<Argument::BaseConfigStaticArgument<T>&>(*it->second));
        }

        template <typename T>
        std::expected<std::reference_wrapper<Argument::BaseConfigDynamicArgument<T>>, std::string> getDynamicArgument(const std::string& key) {
            const auto it = this->m_dynamic_arguments.find(key);
            if (it == this->m_dynamic_arguments.end()) {
                return std::unexpected("Unknown dynamic argument \"" +key +"\"");
            }

            if (it->second->getType() != typeid(T)) {
                return std::unexpected("Type mismatch for dynamic argument \"" +key +"\"");
            }

            return std::ref(static_cast<Argument::BaseConfigDynamicArgument<T>&>(*it->second));
        }

    protected:
        Argument::IConfigStaticArgument& registerStaticArgument(std::unique_ptr<Argument::IConfigStaticArgument> argument) override;

        Argument::IConfigDynamicArgument& registerDynamicArgument(std::unique_ptr<Argument::IConfigDynamicArgument> argument) override;

        template <typename T>
        Argument::BaseConfigStaticArgument<T>& registerStaticArgument(std::string key, T value, bool is_required = false) {
            auto argument = std::make_unique<Argument::BaseConfigStaticArgument<T>>(std::move(key), std::move(value), is_required);
            auto& ref = static_cast<Argument::BaseConfigStaticArgument<T>&>(this->registerStaticArgument(std::move(argument)));
            return ref;
        }

        template <typename T>
        Argument::BaseConfigDynamicArgument<T>& registerDynamicArgument(std::string key, bool is_required = false) {
            auto argument = std::make_unique<Argument::BaseConfigDynamicArgument<T>>(std::move(key), is_required);
            auto& ref = static_cast<Argument::BaseConfigDynamicArgument<T>&>(this->registerDynamicArgument(std::move(argument)));
            return ref;
        }

    private:
        std::unordered_map<std::string, std::unique_ptr<Argument::IConfigStaticArgument>> m_static_arguments;

        std::unordered_map<std::string, std::unique_ptr<Argument::IConfigDynamicArgument>> m_dynamic_arguments;
    };
} // Sierra::OdyCompiler::v1::Core::Config

#endif //SIERRA_ODYCOMPILER_V1_CORE_CONFIG_BASECONFIG_H