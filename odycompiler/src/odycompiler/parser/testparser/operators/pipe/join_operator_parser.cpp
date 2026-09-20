#include <fmt/format.h>
#include <fmt/std.h>
#include <odycompiler/parser/testparser/operators/pipe/join_operator_parser.h>
#include <spdlog/spdlog.h>

namespace sierra::odycompiler::parser::testparser::operators::pipe::join {
    odyplan::v3::core::shared::types::OperatorTypeId JoinOperatorParser::getOperatorType() const {
        return std::to_underlying(TYPE);
    }

    std::unique_ptr<odyplan::v3::core::logical::operators::IOperator> JoinOperatorParser::parse(const nlohmann::json &config) {
        try {
            SPDLOG_DEBUG("Parsing JoinOperator");

            if (!config.contains("config")) {
                throw std::runtime_error("config must contain 'config' field");
            }

            if (!config.contains("predicate")) {
                throw std::runtime_error("SelectOperator config must contain 'predicate' field");
            }

            const auto joinConfig = config.value("config", nlohmann::json::object());

            odyplan::v3::basic::operators::pipe::join::logical::details::JoinConfig tJoinConfig;

            tJoinConfig.allowedLateness = config.value<std::int64_t>("size", 1);
            tJoinConfig.card = config.value<odyplan::v3::basic::operators::pipe::join::logical::details::JoinCard>("card", odyplan::v3::basic::operators::pipe::join::logical::details::JoinCard::MANY_MANY);

            SPDLOG_INFO("Parsing expression");

            const auto& predicateJson = config["predicate"];

            auto predicate = expression::ExpressionParser::parseExpression(predicateJson);

            SPDLOG_INFO("Creating JoinOperator with allowedLateness={}, card={}", tJoinConfig.allowedLateness, std::to_underlying(tJoinConfig.card));

            return std::make_unique<odyplan::v3::basic::operators::pipe::join::logical::JoinOperator>(tJoinConfig, std::move(predicate));
        } catch (const std::exception &e) {
            throw std::runtime_error(fmt::format("Failed to parse JoinOperator: {}", e.what()));
        }
    }
}
