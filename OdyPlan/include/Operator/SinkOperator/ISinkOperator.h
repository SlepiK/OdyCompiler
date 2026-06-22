#ifndef SIERRA_ODYPLAN_V1_OPERATOR_ISINKOPERATOR_H
#define SIERRA_ODYPLAN_V1_OPERATOR_ISINKOPERATOR_H

#include "../IOperator.h"
#include "../../Tuple/Tuple.h"
#include "../Internal/ISink.h"

namespace Sierra::OdyPlan::v1::Operator {
    template <Tuple::Tuple T_in>
    class ISinkOperator : public virtual IOperator, public ISink<T_in> {
    };
} // Sierra::OdyPlan::v1::Operator

#endif //SIERRA_ODYPLAN_V1_OPERATOR_ISINKOPERATOR_H