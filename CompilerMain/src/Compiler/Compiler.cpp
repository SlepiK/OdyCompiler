#include <spdlog/spdlog.h>

#include "Compiler/Compiler.h"

namespace Sierra::OdyCompiler::v1::Compiler {
    Compiler::Compiler() {
        auto result = m_config.getStaticArgument<int>("llvm_version");
        if (result) {
            int& value = result->get().getValue()->get();
            spdlog::info("CONFIG {}", value);
        }
    }
}