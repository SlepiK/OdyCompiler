#ifndef SIERRA_ODYCOMPILER_V1_BUILDER_BASEBUILDER_H
#define SIERRA_ODYCOMPILER_V1_BUILDER_BASEBUILDER_H

#include "IBuilder.h"

#include "Plan/BasePlan.h"

namespace Sierra::OdyCompiler::v1::Builder {
    class BaseBuilder : public IBuilder {
    public:
        BaseBuilder();
        ~BaseBuilder() override = default;

    protected:
        Plan::IPlan m_plan;
    };
} // Sierra::OdyCompiler::v1::Builder

#endif //SIERRA_ODYCOMPILER_V1_BUILDER_BASEBUILDER_H