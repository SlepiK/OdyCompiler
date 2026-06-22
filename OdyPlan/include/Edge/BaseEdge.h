#ifndef SIERRA_ODYPLAN_V1_EDGE_BASEEDGE_H
#define SIERRA_ODYPLAN_V1_EDGE_BASEEDGE_H

#include "IEdge.h"
#include "../Operator/SinkOperator/ISinkOperator.h"
#include "../Operator/SourceOperator/ISourceOperator.h"
#include "../Tuple/Tuple.h"

namespace Sierra::OdyPlan::v1::Edge {
    template <Tuple::Tuple Sink_Tuple>
    class BaseEdge : public IEdge {
    public:
        explicit BaseEdge(Operator::IOperator& source_operator) : m_source(&source_operator) {
        };

        void add_sink(Operator::ISinkOperator<Sink_Tuple>& sink_operator) {
            this->m_sinks.push_back(&sink_operator);
        }

        Operator::ISinkOperator<Sink_Tuple>& sink_at(std::size_t index) const {
            return *this->m_sinks.at(index);
        }

        std::size_t sink_count() const {
            return this->m_sinks.size();
        }

        Operator::IOperator* source_operator() const override {
            return this->m_source;
        }

        std::vector<Operator::IOperator*> sink_operators() const override {
            std::vector<Operator::IOperator*> result;
            result.reserve(this->m_sinks.size());
            for (auto* sink : this->m_sinks) {
                result.push_back(sink);
            }
            return result;
        }

    protected:
        Operator::IOperator* m_source;
        std::vector<Operator::ISinkOperator<Sink_Tuple>*> m_sinks;
    };
} // Sierra::OdyPlan::v1::Edge

#endif // SIERRA_ODYPLAN_V1_EDGE_BASEEDGE_H