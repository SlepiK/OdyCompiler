#ifndef SIERRA_ODYCOMPILER_V1_CONFIG_CONFIG_H
#define SIERRA_ODYCOMPILER_V1_CONFIG_CONFIG_H

#include "BaseConfig.h"

namespace Sierra::OdyCompiler::v1::Config {
    class Config : public BaseConfig {
    public:
        Config();

        ~Config() override = default;

        void validate() override;

        template <typename T>
        T get(const std::string& key) const;
    };
} // Sierra::v1::Compiler::Config

#endif //SIERRA_ODYCOMPILER_V1_CONFIG_CONFIG_H