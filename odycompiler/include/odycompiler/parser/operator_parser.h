#ifndef SIERRA_ODYCOMPILER_PARSER_OPERATORPARSER_HPP
#define SIERRA_ODYCOMPILER_PARSER_OPERATORPARSER_HPP
#include <memory>
#include <core/logical/operators/operator.h>
#include <core/logical/plan/plan.h>
#include <nlohmann/json.hpp>

namespace sierra::odycompiler::parser {
    class IOperatorParser {
    public:
        virtual ~IOperatorParser() = default;

        virtual odyplan::v3::core::shared::identifiers::OperatorId parse(odyplan::v3::core::logical::plan::Plan& lPlan, const nlohmann::json& config) = 0;

        [[nodiscard]] virtual odyplan::v3::core::shared::types::OperatorTypeId getOperatorType() const = 0;
    };
}

#endif //SIERRA_ODYCOMPILER_PARSER_OPERATORPARSER_HPP