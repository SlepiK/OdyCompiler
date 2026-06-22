#ifndef SIERRA_ODYPLAN_V1_OPERATOR_BASEBLOCKINGPIPEOPERATOR_H
#define SIERRA_ODYPLAN_V1_OPERATOR_BASEBLOCKINGPIPEOPERATOR_H

#include "IBlockingPipeOperator.h"

namespace Sierra::OdyPlan::v1::Operator {
    template <Tuple::Tuple T_in, Tuple::Tuple T_out>
    class BaseBlockingPipeOperator : public IBlockingPipeOperator<T_in, T_out> {
    };
} // Sierra::OdyPlan::v1::Operator

#endif //SIERRA_ODYPLAN_V1_OPERATOR_BASEBLOCKINGPIPEOPERATOR_H