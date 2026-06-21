#ifndef SIERRA_ODYCOMPILER_V1_PLAN_BASEPLAN_H
#define SIERRA_ODYCOMPILER_V1_PLAN_BASEPLAN_H

#include "IPlan.h"

namespace Sierra::OdyCompiler::v1::Plan {
    class BasePlan : public IPlan {
    public:
        BasePlan();
        ~BasePlan() override = default;
    };
} // Sierra::OdyCompiler::v1::Plan

#endif //SIERRA_ODYCOMPILER_V1_PLAN_BASEPLAN_H