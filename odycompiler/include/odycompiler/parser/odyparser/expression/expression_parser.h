#ifndef SIERRA_ODYCOMPILER_PARSER_ODYPARSER_EXPRESSION_EXPRESSIONPARSER_HPP
#define SIERRA_ODYCOMPILER_PARSER_ODYPARSER_EXPRESSION_EXPRESSIONPARSER_HPP

#include <memory>
#include <basic/expression/logical/expression.h>
#include <basic/expression/compare_operators.h>
#include <basic/expression/logical_operators.h>
#include <basic/expression/logical/compare_expression.h>
#include <basic/expression/logical/logical_expression.h>
#include <basic/expression/logical/constant_expression.h>
#include <basic/expression/logical/field_reference_expression.h>
#include <nlohmann/json.hpp>

namespace sierra::odycompiler::parser::odyparser::expression {
    class ExpressionParser {
    public:
        static std::unique_ptr<odyplan::v3::basic::expression::logical::IExpression> parseExpression(const nlohmann::json &expressionJson);
    private:
        static odyplan::v3::basic::expression::logical::ExpressionType parseExpressionType(const nlohmann::json &expressionJson);
        static odyplan::v3::basic::expression::CompareOp parseCompareOperator(const nlohmann::json &expressionJson);
        static odyplan::v3::basic::expression::LogicalOp parseLogicalOperator(const nlohmann::json &expressionJson);
    };
}

#endif //SIERRA_ODYCOMPILER_PARSER_ODYPARSER_EXPRESSION_EXPRESSIONPARSER_HPP