#ifndef SIERRA_ODYCOMPILER_V1_COMPILER_BASECOMPILER_H
#define SIERRA_ODYCOMPILER_V1_COMPILER_BASECOMPILER_H

#include <CLI/CLI.hpp>
#include "Compiler/ICompiler.h"

namespace Sierra::OdyCompiler::v1::Compiler {
    class BaseCompiler : public ICompiler {
    public:
        BaseCompiler();
        ~BaseCompiler() override = default;

        CLI::App& getConfig() override;

        void configure();

    protected:
        CLI::App m_config;
        bool m_compile;
        std::string m_logical_plan_path;

        virtual void extended_config() = 0;

    };
} // Sierra

#endif //SIERRA_ODYCOMPILER_V1_COMPILER_BASECOMPILER_H