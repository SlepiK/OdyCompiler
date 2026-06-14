#ifndef SIERRA_ODYCOMPILER_V1_CORE_CONFIG_BASECONFIG_H
#define SIERRA_ODYCOMPILER_V1_CORE_CONFIG_BASECONFIG_H

#include <unordered_map>
#include <memory>
#include <string>

#include "Core/Config/IConfig.h"
#include "Core/Config/Argument/IConfigArgument.h"
#include "Core/Config/Argument/BaseConfigArgument.h"

namespace Sierra::OdyCompiler::v1::Core::Config {
    class BaseConfig : public IConfig {
    public:
        BaseConfig() = default;
        ~BaseConfig() override = default;

        std::expected<void, std::string> validate() override;

        std::expected<void, std::string> parse(int argc, char* argv[]) override;

        template <typename T>
        std::expected<std::reference_wrapper<Argument::BaseConfigArgument<T>>, std::string> getArgument(const std::string& key) {
            const auto it = this->m_arguments.find(key);
            if (it == this->m_arguments.end()) {
                return std::unexpected("Unknown static argument \"" +key +"\"");
            }

            if (it->second->getType() != typeid(T)) {
                return std::unexpected("Type mismatch for static argument \"" +key +"\"");
            }

            return std::ref(static_cast<Argument::BaseConfigArgument<T>&>(*it->second));
        }

    protected:
        Argument::IConfigArgument& registerArgument(std::unique_ptr<Argument::IConfigArgument> argument) override;

        template <typename T>
        Argument::BaseConfigArgument<T>& registerArgument(std::string key, T value, bool is_required = false) {
            auto argument = std::make_unique<Argument::BaseConfigArgument<T>>(std::move(key), std::move(value), is_required);
            auto& ref = static_cast<Argument::BaseConfigArgument<T>&>(this->registerArgument(std::move(argument)));
            return ref;
        }

    private:
        std::unordered_map<std::string, std::unique_ptr<Argument::IConfigArgument>> m_arguments;
    };
} // Sierra::OdyCompiler::v1::Core::Config

#endif //SIERRA_ODYCOMPILER_V1_CORE_CONFIG_BASECONFIG_H