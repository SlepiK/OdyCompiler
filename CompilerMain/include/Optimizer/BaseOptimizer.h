#ifndef SIERRA_ODYCOMPILER_V1_OPTIMIZER_BASEOPTIMIZER_H
#define SIERRA_ODYCOMPILER_V1_OPTIMIZER_BASEOPTIMIZER_H

#include "IOptimizer.h"

namespace Sierra::OdyCompiler::v1::Optimizer {
    class BaseOptimizer : public IOptimizer {
    public:
        BaseOptimizer();
        ~BaseOptimizer() override = default;

    protected:
    };
} // Sierra::OdyCompiler::v1::Optimizer

#endif //SIERRA_ODYCOMPILER_V1_OPTIMIZER_BASEOPTIMIZER_H