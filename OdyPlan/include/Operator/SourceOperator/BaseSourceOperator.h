#ifndef SIERRA_ODYPLAN_V1_OPERATOR_BASESOURCEOPERATOR_H
#define SIERRA_ODYPLAN_V1_OPERATOR_BASESOURCEOPERATOR_H

#include "ISourceOperator.h"

namespace Sierra::OdyPlan::v1::Operator {

    template <Tuple::Tuple T_out>
    class BaseSourceOperator : public ISourceOperator<T_out> {
    };
} // Sierra::OdyPlan::v1::Operator

#endif //SIERRA_ODYPLAN_V1_OPERATOR_BASESOURCEOPERATOR_H