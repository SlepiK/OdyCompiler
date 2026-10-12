#ifndef SIERRA_ODYCOMPILER_PARSER_ODYPARSER_OPERATORS_PRINTOPERATORPARSER_HPP
#define SIERRA_ODYCOMPILER_PARSER_ODYPARSER_OPERATORS_PRINTOPERATORPARSER_HPP

#include <odycompiler/parser/operator_parser.h>
#include <core/shared/types/operator_type_id.h>
#include <basic/operators/sink/print/details/print_operator_options.h>
#include <basic/operators/sink/print/logical/print_operator.h>

namespace sierra::odycompiler::parser::odyparser::operators::sink::print {
    class PrintOperatorParser : public IOperatorParser {
    public:
        static constexpr auto TYPE = odyplan::v3::core::shared::types::BuiltinOperatorType::PRINT;

        odyplan::v3::core::shared::identifiers::OperatorId parse(odyplan::v3::core::logical::plan::Plan& lPlan, const nlohmann::json &config) override;

        [[nodiscard]] odyplan::v3::core::shared::types::OperatorTypeId getOperatorType() const override;
    };
}

#endif //SIERRA_ODYCOMPILER_PARSER_ODYPARSER_OPERATORS_PRINTOPERATORPARSER_HPP