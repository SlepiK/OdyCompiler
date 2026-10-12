#ifndef SIERRA_ODYCOMPILER_PARSER_ODYPARSER_OPERATORS_JOINOPERATORPARSER_HPP
#define SIERRA_ODYCOMPILER_PARSER_ODYPARSER_OPERATORS_JOINOPERATORPARSER_HPP

#include <odycompiler/parser/operator_parser.h>
#include <core/shared/types/operator_type_id.h>
#include <basic/operators/pipe/join/details/join_operator_options.h>
#include <basic/operators/pipe/join/logical/join_operator.h>
#include <odycompiler/parser/odyparser/expression/expression_parser.h>

namespace sierra::odycompiler::parser::odyparser::operators::pipe::join {
    class JoinOperatorParser : public IOperatorParser {
    public:
        static constexpr auto TYPE = odyplan::v3::core::shared::types::BuiltinOperatorType::JOIN;

        odyplan::v3::core::shared::identifiers::OperatorId parse(odyplan::v3::core::logical::plan::Plan& lPlan, const nlohmann::json &config) override;

        [[nodiscard]] odyplan::v3::core::shared::types::OperatorTypeId getOperatorType() const override;
    };
}

#endif //SIERRA_ODYCOMPILER_PARSER_ODYPARSER_OPERATORS_JOINOPERATORPARSER_HPP