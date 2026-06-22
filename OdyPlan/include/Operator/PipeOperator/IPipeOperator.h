#ifndef SIERRA_ODYPLAN_V1_OPERATOR_IPIPEOPERATOR_H
#define SIERRA_ODYPLAN_V1_OPERATOR_IPIPEOPERATOR_H

#include "../SinkOperator/ISinkOperator.h"
#include "../Internal/ISource.h"
#include "../../Tuple/Tuple.h"

namespace Sierra::OdyPlan::v1::Operator {
    template <Tuple::Tuple T_in, Tuple::Tuple T_out>
    class IPipeOperator : public ISinkOperator<T_in>, public ISource<T_out> {
    };
} // Sierra::OdyPlan::v1::Operator

#endif //SIERRA_ODYPLAN_V1_OPERATOR_IPIPEOPERATOR_H