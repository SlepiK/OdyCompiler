#ifndef SIERRA_ODYCOMPILER_V1_OPTIMIZER_IOPTIMIZER_H
#define SIERRA_ODYCOMPILER_V1_OPTIMIZER_IOPTIMIZER_H

namespace Sierra::OdyCompiler::v1::Optimizer {
    class IOptimizer {
    public:
        virtual ~IOptimizer() = default;
    };
} // Sierra::OdyCompiler::v1::Optimizer

#endif //SIERRA_ODYCOMPILER_V1_OPTIMIZER_IOPTIMIZER_H