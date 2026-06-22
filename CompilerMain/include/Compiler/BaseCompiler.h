#ifndef SIERRA_ODYCOMPILER_V1_COMPILER_BASECOMPILER_H
#define SIERRA_ODYCOMPILER_V1_COMPILER_BASECOMPILER_H

#include <CLI/CLI.hpp>

#include "Compiler/ICompiler.h"
#include "Builder/IBuilder.h"
#include "Translater/ITranslater.h"
#include "Optimizer/Optimizer.h"

namespace Sierra::OdyCompiler::v1::Compiler {
    class BaseCompiler : public ICompiler {
    public:
        BaseCompiler();
        ~BaseCompiler() override = default;

        CLI::App& getConfig() override;

        void configure();

    protected:
        Builder::IBuilder m_builder;
        Translater::ITranslater m_translater;
        Optimizer::IOptimizer m_optimizer;

        CLI::App m_config;
        bool m_compile;
        std::string m_logical_plan_path;

        virtual void extended_config() = 0;

    };
} // Sierra

#endif //SIERRA_ODYCOMPILER_V1_COMPILER_BASECOMPILER_H