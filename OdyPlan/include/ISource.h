#ifndef SIERRA_ODYPLAN_V1_OPERATOR_ISOURCE_H
#define SIERRA_ODYPLAN_V1_OPERATOR_ISOURCE_H

#include "ISink.h"
#include "Tuple/Tuple.h"

namespace Sierra::OdyPlan::v1::Operator {
    template <Tuple::OdyTuple T_out>
    class ISource {
    public:
        virtual ~ISource() = default;
        virtual void produce(ISink<T_out>& sink_operator) = 0;
        virtual void open() = 0;
        virtual void close() = 0;
    };
} // Sierra::OdyPlan::v1::Operator

#endif //SIERRA_ODYPLAN_V1_OPERATOR_ISOURCE_H