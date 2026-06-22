#ifndef SIERRA_ODYPLAN_V1_PLAN_BASEPLAN_H
#define SIERRA_ODYPLAN_V1_PLAN_BASEPLAN_H

#include <memory>
#include <vector>

#include "IPlan.h"
#include "../Edge/BaseEdge.h"
#include "../Edge/IEdge.h"
#include "../Operator/IOperator.h"
#include "../Tuple/Tuple.h"

namespace Sierra::OdyPlan::v1::Plan {
    class BasePlan : public IPlan {
    public:
        BasePlan();
        ~BasePlan() override = default;

        template <typename TOperator>
        TOperator& add_node(std::unique_ptr<TOperator> node) {
            TOperator& ref = *node;
            this->m_nodes.push_back(std::move(node));
            return ref;
        }

        template <Tuple::Tuple TTuple>
        Edge::BaseEdge<TTuple>& add_edge(Operator::IOperator& source_operator) {
            auto edge = std::make_unique<Edge::BaseEdge<TTuple>>(source_operator);
            auto* raw = edge.get();
            this->m_edges.push_back(std::move(edge));
            return *raw;
        }

        const std::vector<std::unique_ptr<Operator::IOperator>>& nodes() const override {
            return this->m_nodes;
        }

        const std::vector<std::unique_ptr<Edge::IEdge>>& edges() const override {
            return this->m_edges;
        }

        std::vector<Edge::IEdge*> edges_from(const Operator::IOperator source_operator) const override {
            std::vector<Edge::IEdge*> result;
            for (auto& edge : this->m_edges) {
                if (edge->source_operator() == &source_operator) {
                    result.push_back(edge.get());
                }
            }
            return result;
        }

        std::vector<Edge::IEdge*> edges_into(const Operator::IOperator sink_operator) const override {
            std::vector<Edge::IEdge*> result;
            for (auto& edge : this->m_edges) {
                for (auto* sink : edge->sink_operators()) {
                    if (sink == &sink_operator) {
                        result.push_back(edge.get());
                        break;
                    }
                }
            }
            return result;
        }


    protected:
        std::vector<std::unique_ptr<Operator::IOperator>> m_nodes;
        std::vector<std::unique_ptr<Edge::IEdge>> m_edges;
    };
} // Sierra::OdyPlan::v1::Plan

#endif //SIERRA_ODYPLAN_V1_PLAN_BASEPLAN_H