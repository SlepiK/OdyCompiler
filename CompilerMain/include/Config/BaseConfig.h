#ifndef SIERRA_ODYCOMPILER_V1_CONFIG_BASECONFIG_H
#define SIERRA_ODYCOMPILER_V1_CONFIG_BASECONFIG_H

#include <unordered_map>
#include <memory>
#include <string>

#include "Config/IConfig.h"
#include "Config/Argument/IConfigArgument.h"
#include "Config/Argument/ConfigArgument.h"

namespace Sierra::OdyCompiler::v1::Config {
    class BaseConfig : public IConfig {
    public:
        BaseConfig();

        ~BaseConfig() override = default;

        Argument::IConfigArgument& getArgument(const std::string& key) override;

    protected:
        std::unordered_map<std::string, std::unique_ptr<Argument::IConfigArgument>> m_arguments;

        Argument::IConfigArgument& registerArgument(std::unique_ptr<Argument::IConfigArgument> argument) override;
    };
} // Sierra::v1::Compiler::Config

#endif //SIERRA_ODYCOMPILER_V1_CONFIG_BASECONFIG_H