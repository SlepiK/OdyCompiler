#ifndef SIERRA_ODYCOMPILER_PARSER_TESTPARSER_OPERATORS_JOINOPERATORPARSER_HPP
#define SIERRA_ODYCOMPILER_PARSER_TESTPARSER_OPERATORS_JOINOPERATORPARSER_HPP

#include <odycompiler/parser/operator_parser.h>
#include <core/shared/types/operator_type_id.h>
#include <basic/operators/pipe/join/logical/details/join_config.h>
#include <basic/operators/pipe/join/logical/join_operator.h>
#include <odycompiler/parser/testparser/expression/expression_parser.h>

namespace sierra::odycompiler::parser::testparser::operators::pipe::join {
    class JoinOperatorParser : public IOperatorParser {
    public:
        static constexpr auto TYPE = odyplan::v3::core::shared::types::BuiltinOperatorType::JOIN;

        std::unique_ptr<odyplan::v3::core::logical::operators::IOperator> parse(const nlohmann::json &config) override;

        [[nodiscard]] odyplan::v3::core::shared::types::OperatorTypeId getOperatorType() const override;
    };
}

#endif //SIERRA_ODYCOMPILER_PARSER_TESTPARSER_OPERATORS_JOINOPERATORPARSER_HPP