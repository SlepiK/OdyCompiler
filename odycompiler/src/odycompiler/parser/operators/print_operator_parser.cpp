#include <odycompiler/parser/operators/print_operator_parser.h>

namespace sierra::odycompiler::parser {
    std::unique_ptr<odyplan::v3::core::logical::operators::IOperator> PrintOperatorParser::parse(const nlohmann::json &config) {

    }

    odyplan::v3::basic::operators::sink::print::details::PrintOperatorOptions PrintOperatorParser::parseOptions(const nlohmann::json &config) {

    }
}
