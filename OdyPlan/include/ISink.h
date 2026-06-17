#ifndef SIERRA_ODYPLAN_V1_OPERATOR_ISINK_H
#define SIERRA_ODYPLAN_V1_OPERATOR_ISINK_H

#include "Tuple/Tuple.h"


namespace Sierra::OdyPlan::v1::Operator {
    template <Tuple::OdyTuple T_in>
    class ISink {
    public:
        virtual ~ISink() = default;
        virtual void consume(T_in previous_data) = 0;
        virtual void flush() = 0;
    };
} // Sierra::OdyPlan::v1::Operator

#endif //SIERRA_ODYPLAN_V1_OPERATOR_ISINK_H