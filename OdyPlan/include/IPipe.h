#ifndef SIERRA_ODYPLAN_V1_OPERATOR_IPIPE_H
#define SIERRA_ODYPLAN_V1_OPERATOR_IPIPE_H

#include "ISink.h"
#include "ISource.h"
#include "Tuple/Tuple.h"

namespace Sierra::OdyPlan::v1::Operator {
    template <Tuple::OdyTuple In, Tuple::OdyTuple Out>
    class IPipe : public ISink<In>, ISource<Out>{
    protected:
        virtual ISink<Out>* next() = 0;
    };
} // Sierra::OdyPlan::v1::Operator

#endif //SIERRA_ODYPLAN_V1_OPERATOR_IPIPE_H