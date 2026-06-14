#include "Compiler/BaseCompiler.h"

namespace Sierra::OdyCompiler::v1::Compiler {
    BaseCompiler::BaseCompiler()
        : m_config(Config::CompilerConfig()) {
    }

    Config::CompilerConfig& BaseCompiler::getConfig() {
        return this->m_config;
    }
} // Sierra::OdyCompiler::v1::Compiler