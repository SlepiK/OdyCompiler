#ifndef SIERRA_ODYPLAN_V1_OPERATOR_IPRODUCE_H
#define SIERRA_ODYPLAN_V1_OPERATOR_IPRODUCE_H

#include "../../Edge/BaseEdge.h"

namespace Sierra::OdyPlan::v1::Operator::Internal {
    template <Tuple::Tuple T_out>
    class IProduce {
    public:
        virtual ~IProduce() = default;

        virtual bool produce(const Edge::BaseEdge<T_out>& edge) = 0;
    };
} // Sierra::OdyPlan::v1::Operator::Internal

#endif //SIERRA_ODYPLAN_V1_OPERATOR_IPRODUCE_H