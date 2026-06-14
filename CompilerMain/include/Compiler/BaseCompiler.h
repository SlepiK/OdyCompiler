#ifndef SIERRA_ODYCOMPILER_V1_COMPILER_BASECOMPILER_H
#define SIERRA_ODYCOMPILER_V1_COMPILER_BASECOMPILER_H

#include <memory>

#include "Compiler/ICompiler.h"
#include "Config/IConfig.h"

namespace Sierra::OdyCompiler::v1::Compiler {
    class BaseCompiler : public ICompiler {
    public:
        explicit BaseCompiler() {}
        explicit BaseCompiler(std::unique_ptr<Config::IConfig> config) : m_config(std::move(config)) {}
        ~BaseCompiler() override = default;
    protected:
        std::unique_ptr<Config::IConfig> m_config;
    };
} // Sierra

#endif //SIERRA_ODYCOMPILER_V1_COMPILER_BASECOMPILER_H