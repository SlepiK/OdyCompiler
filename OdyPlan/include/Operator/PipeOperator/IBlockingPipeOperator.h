#ifndef SIERRA_ODYPLAN_V1_OPERATOR_IBLOCKINGPIPEOPERATOR_H
#define SIERRA_ODYPLAN_V1_OPERATOR_IBLOCKINGPIPEOPERATOR_H

#include "../SinkOperator/ISinkOperator.h"
#include "../../Tuple/Tuple.h"
#include "../SourceOperator/ISourceOperator.h"

namespace Sierra::OdyPlan::v1::Operator {
    template <Tuple::Tuple T_in, Tuple::Tuple T_out>
    class IBlockingPipeOperator : public ISinkOperator<T_in>, public ISourceOperator<T_out> {
    };
} // Sierra::OdyPlan::v1::Operator

#endif //SIERRA_ODYPLAN_V1_OPERATOR_IBLOCKINGPIPEOPERATOR_H