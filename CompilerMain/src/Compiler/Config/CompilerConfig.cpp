#include "Compiler/Config/CompilerConfig.h"

namespace Sierra::OdyCompiler::v1::Compiler::Config {
    CompilerConfig::CompilerConfig() {
        this->registerStaticArgument("llvm_version",22,true);
    }
}