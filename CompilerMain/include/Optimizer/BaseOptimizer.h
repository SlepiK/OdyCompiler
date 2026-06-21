#ifndef SIERRA_ODYCOMPILER_V1_OPTIMIZER_BASEOPTIMIZER_H
#define SIERRA_ODYCOMPILER_V1_OPTIMIZER_BASEOPTIMIZER_H

#include "IOptimizer.h"

#include "Plan/BasePlan.h"

namespace Sierra::OdyCompiler::v1::Optimizer {
    class BaseOptimizer : public IOptimizer {
    public:
        BaseOptimizer();
        ~BaseOptimizer() override = default;

    protected:
        Plan::IPlan m_plan;
    };
} // Sierra::OdyCompiler::v1::Optimizer

#endif //SIERRA_ODYCOMPILER_V1_OPTIMIZER_BASEOPTIMIZER_H