#include <fmt/format.h>
#include <fmt/std.h>
#include <odycompiler/parser/testparser/operators/pipe/join_operator_parser.h>
#include <spdlog/spdlog.h>

namespace sierra::odycompiler::parser::testparser::operators::pipe::join {
    odyplan::v3::core::shared::types::OperatorTypeId JoinOperatorParser::getOperatorType() const {
        return std::to_underlying(TYPE);
    }

    odyplan::v3::core::shared::identifiers::OperatorId JoinOperatorParser::parse(odyplan::v3::core::logical::plan::Plan& lPlan, const nlohmann::json &config) {
        try {
            spdlog::debug("Parsing JoinOperator");

            if (!config.contains("config")) {
                throw std::runtime_error("config must contain 'config' field");
            }

            if (!config.contains("predicate")) {
                throw std::runtime_error("SelectOperator config must contain 'predicate' field");
            }

            const auto joinOptions = config.value("config", nlohmann::json::object());

            odyplan::v3::basic::operators::pipe::join::details::JoinOperatorOptions tJoinOptions;

            tJoinOptions.allowedLateness = config.value<std::int64_t>("size", 1);
            tJoinOptions.card = config.value<odyplan::v3::basic::operators::pipe::join::details::JoinOperatorCard>("card", odyplan::v3::basic::operators::pipe::join::details::JoinOperatorCard::MANY_MANY);

            spdlog::debug("Parsing expression");

            const auto& predicateJson = config["predicate"];

            auto predicate = expression::ExpressionParser::parseExpression(predicateJson);

            spdlog::info("Creating JoinOperator with allowedLateness={}, card={}", tJoinOptions.allowedLateness, std::to_underlying(tJoinOptions.card));

            return lPlan.addOperator(std::make_unique<odyplan::v3::basic::operators::pipe::join::logical::JoinOperator>(tJoinOptions, std::move(predicate)));
        } catch (const std::exception &e) {
            throw std::runtime_error(fmt::format("Failed to parse JoinOperator: {}", e.what()));
        }
    }
}
