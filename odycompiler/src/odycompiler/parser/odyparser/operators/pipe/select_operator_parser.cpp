#include <fmt/format.h>
#include <odycompiler/parser/odyparser/operators/pipe/select_operator_parser.h>
#include <spdlog/spdlog.h>

namespace sierra::odycompiler::parser::odyparser::operators::pipe::select {
    odyplan::v3::core::shared::types::OperatorTypeId SelectOperatorParser::getOperatorType() const {
        return std::to_underlying(TYPE);
    }

    odyplan::v3::core::shared::identifiers::OperatorId SelectOperatorParser::parse(odyplan::v3::core::logical::plan::Plan& lPlan, const nlohmann::json &config) {
        try {
            spdlog::debug("Parsing SelectOperator");

            if (!config.contains("PREDICATE") && !config.contains("predicate")) {
                throw std::runtime_error("config must contain 'PREDICATE' or 'predicate' field");
            }

            const auto& predicateJson = config.contains("PREDICATE") ? config["PREDICATE"] : config["predicate"];
            auto predicate = expression::ExpressionParser::parseExpression(predicateJson);

            spdlog::info("Creating SelectOperator");

            return lPlan.addOperator(std::make_unique<odyplan::v3::basic::operators::pipe::select::logical::SelectOperator>(std::move(predicate)));
        } catch (const std::exception &e) {
            throw std::runtime_error(fmt::format("Failed to parse SelectOperator: {}", e.what()));
        }
    }
}
