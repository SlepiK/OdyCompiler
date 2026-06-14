#ifndef SIERRA_ODYCOMPILER_V1_CONFIG_BASECONFIG_H
#define SIERRA_ODYCOMPILER_V1_CONFIG_BASECONFIG_H

#include <unordered_map>
#include <memory>
#include <string>

#include "Config/IConfig.h"
#include "Config/Argument/IConfigArgument.h"

namespace Sierra::OdyCompiler::v1::Config {
    class BaseConfig : public IConfig {
    public:
        BaseConfig();

        ~BaseConfig() override = default;

        template <Argument::IConfigArgument T>
        T get(const std::string& key) const {
            auto& arg = this->m_arguments.at(key);
            if (arg->getType() != typeid(T))
                throw std::runtime_error("Type mismatch for key [" + key + "]");
            return static_cast<Argument::IConfigArgument<T>*>(arg.get()).getValue();
        }


    protected:
        std::unordered_map<std::string, std::unique_ptr<Argument::IConfigArgument>> m_arguments;
    };
} // Sierra::v1::Compiler::Config

#endif //SIERRA_ODYCOMPILER_V1_CONFIG_BASECONFIG_H