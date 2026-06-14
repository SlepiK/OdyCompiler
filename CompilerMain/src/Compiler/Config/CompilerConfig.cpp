#include "Compiler/Config/CompilerConfig.h"

namespace Sierra::OdyCompiler::v1::Compiler::Config {
    CompilerConfig::CompilerConfig() {
        this->registerArgument("llvm_version",22,true);
    }
}