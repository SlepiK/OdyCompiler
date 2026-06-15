#ifndef SIERRA_ODYCOMPILER_V1_BUILDER_BASEBUILDER_H
#define SIERRA_ODYCOMPILER_V1_BUILDER_BASEBUILDER_H

#include "IBuilder.h"

namespace Sierra::OdyCompiler::v1::Builder {
    class BaseBuilder : public IBuilder {
    public:
        BaseBuilder();
        ~BaseBuilder() override = default;
    };
} // Sierra::OdyCompiler::v1::Builder

#endif //SIERRA_ODYCOMPILER_V1_BUILDER_BASEBUILDER_H