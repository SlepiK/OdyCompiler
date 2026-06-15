#include "Compiler/BaseCompiler.h"

#include <csignal>

namespace Sierra::OdyCompiler::v1::Compiler {
    BaseCompiler::BaseCompiler()
        : m_config("OdyCompiler"),
        m_compile(false),
        m_logical_plan_path("./logical_plan.json") {
    }

    CLI::App& BaseCompiler::getConfig() {
        return this->m_config;
    }

    void BaseCompiler::configure() {
        this->m_config.add_flag("--c,--compile", this->m_compile);
        this->m_config.add_option("--lp, --logical_plan", this->m_logical_plan_path);

        this->extended_config();
    }

} // Sierra::OdyCompiler::v1::Compiler