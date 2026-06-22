#ifndef SIERRA_ODYPLAN_V1_OPERATOR_ISOURCEOPERATOR_H
#define SIERRA_ODYPLAN_V1_OPERATOR_ISOURCEOPERATOR_H

#include "../Internal/ISource.h"
#include "../IOperator.h"
#include "../../Tuple/Tuple.h"
#include "../Internal/IProduce.h"

namespace Sierra::OdyPlan::v1::Operator {
    template <Tuple::Tuple T_out>
    class ISourceOperator : public virtual IOperator, public ISource<T_out>, public Internal::IProduce<T_out> {
    };
} // Sierra::OdyPlan::v1::Operator

#endif //SIERRA_ODYPLAN_V1_OPERATOR_ISOURCEOPERATOR_H