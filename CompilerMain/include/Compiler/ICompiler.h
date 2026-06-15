#ifndef SIERRA_ODYCOMPILER_V1_COMPILER_ICOMPILER_H
#define SIERRA_ODYCOMPILER_V1_COMPILER_ICOMPILER_H

#include <CLI/CLI.hpp>

namespace Sierra::OdyCompiler::v1::Compiler {
    class ICompiler {
    public:
        virtual ~ICompiler() = default;

        virtual void compile() = 0;

        virtual CLI::App& getConfig() = 0;
    };
} // Sierra::OdyCompiler::v1::Compiler

#endif //SIERRA_ODYCOMPILER_V1_COMPILER_ICOMPILER_H