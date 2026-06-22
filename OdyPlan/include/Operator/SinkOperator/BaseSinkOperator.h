#ifndef SIERRA_ODYPLAN_V1_OPERATOR_BASESINKOPERATOR_H
#define SIERRA_ODYPLAN_V1_OPERATOR_BASESINKOPERATOR_H

#include "ISinkOperator.h"

namespace Sierra::OdyPlan::v1::Operator {
    template <Tuple::Tuple T_in>
    class BaseSinkOperator : public ISinkOperator<T_in> {
    };
} // Sierra::OdyPlan::v1::Operator

#endif //SIERRA_ODYPLAN_V1_OPERATOR_BASESINKOPERATOR_H