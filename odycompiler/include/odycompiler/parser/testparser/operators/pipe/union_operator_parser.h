#ifndef SIERRA_ODYCOMPILER_PARSER_TESTPARSER_OPERATORS_UNIONOPERATORPARSER_HPP
#define SIERRA_ODYCOMPILER_PARSER_TESTPARSER_OPERATORS_UNIONOPERATORPARSER_HPP

#include <odycompiler/parser/operator_parser.h>
#include <core/shared/types/operator_type_id.h>
#include <basic/operators/pipe/merge/logical/merge_operator.h>

namespace sierra::odycompiler::parser::testparser::operators::pipe::un {
    class UnionOperatorParser : public IOperatorParser {
    public:
        static constexpr auto TYPE = odyplan::v3::core::shared::types::BuiltinOperatorType::MERGE;

        odyplan::v3::core::shared::identifiers::OperatorId parse(odyplan::v3::core::logical::plan::Plan& lPlan, const nlohmann::json &config) override;

        [[nodiscard]] odyplan::v3::core::shared::types::OperatorTypeId getOperatorType() const override;
    };
}

#endif //SIERRA_ODYCOMPILER_PARSER_TESTPARSER_OPERATORS_UNIONOPERATORPARSER_HPP