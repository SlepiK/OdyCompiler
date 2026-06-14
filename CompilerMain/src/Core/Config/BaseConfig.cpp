#include "Core/Config/BaseConfig.h"

namespace Sierra::OdyCompiler::v1::Core::Config {
    std::expected<void, std::string> BaseConfig::validate() {
        for (auto& [key, arg ] : this->m_static_arguments) {
            if (arg->isRequired() && !arg->isAvailable()) {
                return std::unexpected("Required static argument \"" +key +"\" is missing");
            }
        }

        for (auto& [key, arg ] : this->m_dynamic_arguments) {
            if (arg->isRequired() && !arg->isAvailable()) {
                return std::unexpected("Required dynamic argument \"" +key +"\" is missing");
            }
        }

        return {};
    }

    std::expected<void, std::string> BaseConfig::parseDynamicArguments(const int argc, char* argv[]) {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (!arg.starts_with("--")) {
                continue;
            }

            auto pos = arg.find('=');
            std::string key = arg.substr(2, pos - 2);
            std::string val = (pos != std::string::npos) ? arg.substr(pos + 1) : argv[++i];

            auto it = this->m_dynamic_arguments.find(key);
            if (it == this->m_dynamic_arguments.end()) {
                return std::unexpected("Unknown dynamic argument \"" + key + "\"");
            }

            if (auto result = it->second->parse(val); !result) {
                return std::unexpected(result.error());
            }
        }
        return {};
    }

    Argument::IConfigStaticArgument& BaseConfig::registerStaticArgument(std::unique_ptr<Argument::IConfigStaticArgument> argument) {
        std::string key = argument->getKey();
        auto [it, _] = this->m_static_arguments.emplace(std::move(key), std::move(argument));
        return *it->second;
    }

    Argument::IConfigDynamicArgument& BaseConfig::registerDynamicArgument(std::unique_ptr<Argument::IConfigDynamicArgument> argument) {
        std::string key = argument->getKey();
        auto [it, _] = this->m_dynamic_arguments.emplace(std::move(key), std::move(argument));
        return *it->second;
    }
} // Sierra::OdyCompiler::v1::Core::Config