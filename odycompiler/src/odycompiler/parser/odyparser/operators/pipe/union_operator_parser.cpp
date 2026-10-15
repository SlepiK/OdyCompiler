#include <fmt/format.h>
#include <odycompiler/parser/odyparser/operators/pipe/union_operator_parser.h>
#include <spdlog/spdlog.h>

namespace sierra::odycompiler::parser::odyparser::operators::pipe::un {
    odyplan::v3::core::shared::types::OperatorTypeId UnionOperatorParser::getOperatorType() const {
        return std::to_underlying(TYPE);
    }

    odyplan::v3::core::shared::identifiers::OperatorId UnionOperatorParser::parse(odyplan::v3::core::logical::plan::Plan& lPlan, const nlohmann::json &config) {
        try {
            spdlog::debug("Parsing UnionOperator");

            const auto& j = config.value("assureOrder", nlohmann::json(true));
            const bool assureOrder = j.is_string() ? nlohmann::json::parse(j.get<std::string>()).get<bool>()
                                                   : j.get<bool>();

            spdlog::info("Creating UnionOperator");

            return lPlan.addOperator(std::make_unique<odyplan::v3::basic::operators::pipe::union_::logical::UnionOperator>(assureOrder));
        } catch (const std::exception &e) {
            throw std::runtime_error(fmt::format("Failed to parse UnionOperator: {}", e.what()));
        }
    }
}
