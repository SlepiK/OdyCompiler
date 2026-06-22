#ifndef SIERRA_ODYPLAN_V1_OPERATOR_BASEPIPEOPERATOR_H
#define SIERRA_ODYPLAN_V1_OPERATOR_BASEPIPEOPERATOR_H
#include "IPipeOperator.h"

namespace Sierra::OdyPlan::v1::Operator {
    template <Tuple::Tuple T_in, Tuple::Tuple T_out>
    class BasePipeOperator : public IPipeOperator<T_in, T_out> {
    };
} // Sierra::OdyPlan::v1::Operator

#endif //SIERRA_ODYPLAN_V1_OPERATOR_BASEPIPEOPERATOR_H