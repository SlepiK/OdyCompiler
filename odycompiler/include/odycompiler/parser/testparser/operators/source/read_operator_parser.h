#ifndef SIERRA_ODYCOMPILER_PARSER_TESTPARSER_OPERATORS_READOPERATORPARSER_HPP
#define SIERRA_ODYCOMPILER_PARSER_TESTPARSER_OPERATORS_READOPERATORPARSER_HPP

#include <odycompiler/parser/operator_parser.h>
#include <core/shared/types/operator_type_id.h>
#include <basic/operators/source/read/details/read_operator_options.h>
#include <basic/operators/source/read/logical/read_operator.h>

namespace sierra::odycompiler::parser::testparser::operators::source::read {
    class ReadOperatorParser : public IOperatorParser {
    public:
        static constexpr auto TYPE = odyplan::v3::core::shared::types::BuiltinOperatorType::READ;

        std::unique_ptr<odyplan::v3::core::logical::operators::IOperator> parse(const nlohmann::json &config) override;

        [[nodiscard]] odyplan::v3::core::shared::types::OperatorTypeId getOperatorType() const override;
    };
}

#endif //SIERRA_ODYCOMPILER_PARSER_TESTPARSER_OPERATORS_READOPERATORPARSER_HPP