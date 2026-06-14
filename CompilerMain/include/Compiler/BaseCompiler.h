#ifndef SIERRA_ODYCOMPILER_V1_COMPILER_BASECOMPILER_H
#define SIERRA_ODYCOMPILER_V1_COMPILER_BASECOMPILER_H

#include <memory>

#include "Compiler/ICompiler.h"
#include "Config/CompilerConfig.h"

namespace Sierra::OdyCompiler::v1::Compiler {
    class BaseCompiler : public ICompiler {
    public:
        BaseCompiler();
        ~BaseCompiler() override = default;

        Config::CompilerConfig& getConfig() override;

    protected:
        Config::CompilerConfig m_config;
    };
} // Sierra

#endif //SIERRA_ODYCOMPILER_V1_COMPILER_BASECOMPILER_H