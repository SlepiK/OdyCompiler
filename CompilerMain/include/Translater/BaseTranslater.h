#ifndef SIERRA_ODYCOMPILER_V1_TRANSLATER_BASETRANSLATER_H
#define SIERRA_ODYCOMPILER_V1_TRANSLATER_BASETRANSLATER_H

#include "ITranslater.h"

#include "Plan/BasePlan.h"

namespace Sierra::OdyCompiler::v1::Translater {
    class BaseTranslater : public ITranslater {
    public:
        BaseTranslater();
        ~BaseTranslater() override = default;

    protected:
        Plan::IPlan m_plan;
    };
} // Sierra::OdyCompiler::v1::Translater

#endif //SIERRA_ODYCOMPILER_V1_TRANSLATER_BASETRANSLATER_H