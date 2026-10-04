#include <fmt/format.h>
#include <odycompiler/parser/testparser/operators/pipe/union_operator_parser.h>
#include <spdlog/spdlog.h>

namespace sierra::odycompiler::parser::testparser::operators::pipe::un {
    odyplan::v3::core::shared::types::OperatorTypeId UnionOperatorParser::getOperatorType() const {
        return std::to_underlying(TYPE);
    }

    odyplan::v3::core::shared::identifiers::OperatorId UnionOperatorParser::parse(odyplan::v3::core::logical::plan::Plan& lPlan, const nlohmann::json &config) {
        try {
            spdlog::debug("Parsing UnionOperator");



            spdlog::info("Creating UnionOperator");

            return lPlan.addOperator(std::make_unique<odyplan::v3::basic::operators::pipe::union_::logical::UnionOperator>());
        } catch (const std::exception &e) {
            throw std::runtime_error(fmt::format("Failed to parse UnionOperator: {}", e.what()));
        }
    }
}
