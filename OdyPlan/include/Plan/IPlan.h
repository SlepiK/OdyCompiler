#ifndef SIERRA_ODYPLAN_V1_PLAN_IPLAN_H
#define SIERRA_ODYPLAN_V1_PLAN_IPLAN_H
#include <memory>
#include <vector>

#include "../Edge/IEdge.h"
#include "../Operator/IOperator.h"

namespace Sierra::OdyPlan::v1::Plan {
    class IPlan {
    public:
        virtual ~IPlan() = default;

        virtual const std::vector<std::unique_ptr<Operator::IOperator>>& nodes() const = 0;
        virtual const std::vector<std::unique_ptr<Edge::IEdge>>& edges() const = 0;
        virtual std::vector<Edge::IEdge*> edges_from(const Operator::IOperator source_operator) const = 0;
        virtual std::vector<Edge::IEdge*> edges_into(const Operator::IOperator sink_operator) const = 0;
    };
} // Sierra::OdyPlan::v1::Plan

#endif //SIERRA_ODYPLAN_V1_PLAN_IPLAN_H