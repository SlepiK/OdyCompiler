#include <fmt/format.h>
#include <fmt/std.h>
#include <odycompiler/parser/odyparser/operators/pipe/join_operator_parser.h>
#include <spdlog/spdlog.h>

namespace sierra::odycompiler::parser::odyparser::operators::pipe::join {
    odyplan::v3::core::shared::types::OperatorTypeId JoinOperatorParser::getOperatorType() const {
        return std::to_underlying(TYPE);
    }

    odyplan::v3::core::shared::identifiers::OperatorId JoinOperatorParser::parse(odyplan::v3::core::logical::plan::Plan& lPlan, const nlohmann::json &config) {
        try {
            spdlog::debug("Parsing JoinOperator");

            if (!config.contains("options")) {
                throw std::runtime_error("config must contain 'options' field");
            }

            if (!config.contains("predicate")) {
                throw std::runtime_error("SelectOperator config must contain 'predicate' field");
            }

            const bool assureOrder = config.value("assure_order", true);

            const auto options = config.value("options", nlohmann::json::object());

            odyplan::v3::basic::operators::pipe::join::details::JoinOperatorOptions joinOptions;

            joinOptions.allowedLateness = options.value<std::int64_t>("size", 0);
            joinOptions.card = options.value<odyplan::v3::basic::operators::pipe::join::details::JoinOperatorCard>("card", odyplan::v3::basic::operators::pipe::join::details::JoinOperatorCard::MANY_MANY);

            spdlog::debug("Parsing expression");

            const auto& predicateJson = config["predicate"];

            auto predicate = expression::ExpressionParser::parseExpression(predicateJson);

            spdlog::info("Creating JoinOperator with allowedLateness={}, card={}", joinOptions.allowedLateness, std::to_underlying(joinOptions.card));

            return lPlan.addOperator(std::make_unique<odyplan::v3::basic::operators::pipe::join::logical::JoinOperator>(joinOptions, std::move(predicate), assureOrder));
        } catch (const std::exception &e) {
            throw std::runtime_error(fmt::format("Failed to parse JoinOperator: {}", e.what()));
        }
    }
}
