#ifndef SIERRA_ODYCOMPILER_PARSER_OPERATORS_READOPERATORPARSER_HPP
#define SIERRA_ODYCOMPILER_PARSER_OPERATORS_READOPERATORPARSER_HPP

#include <odycompiler/parser/operator_parser.h>
#include <core/shared/types/operator_type_id.h>
#include <basic/operators/source/read/details/read_operator_options.h>
#include <basic/operators/source/read/logical/read_operator.h>
#include <basic/io/io_format.h>

namespace sierra::odycompiler::parser {
    class ReadOperatorParser : public IOperatorParser {
    public:
        std::unique_ptr<odyplan::v3::core::logical::operators::IOperator> parse(const nlohmann::json &config) override;

        odyplan::v3::core::shared::types::OperatorTypeId getOperatorType() override {
            return std::to_underlying(odyplan::v3::core::shared::types::BuiltinOperatorType::READ);
        }

    private:
        odyplan::v3::basic::operators::source::read::details::LReadOperatorOptions parseOptions(const nlohmann::json &config);
    };
}

#endif //SIERRA_ODYCOMPILER_PARSER_OPERATORS_READOPERATORPARSER_HPP