#ifndef SIERRA_ODYPLAN_V1_EDGE_IEDGE_H
#define SIERRA_ODYPLAN_V1_EDGE_IEDGE_H

#include <vector>

#include "../Operator/IOperator.h"

namespace Sierra::OdyPlan::v1::Edge {
    class IEdge {
    public:
        virtual ~IEdge() = default;

        virtual Operator::IOperator* source_operator() const = 0;
        virtual std::vector<Operator::IOperator*> sink_operators() const = 0;
    };
} // Sierra::OdyPlan::v1::Edge

#endif // SIERRA_ODYPLAN_V1_EDGE_IEDGE_H