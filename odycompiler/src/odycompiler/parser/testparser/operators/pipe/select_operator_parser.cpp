#include <fmt/format.h>
#include <odycompiler/parser/testparser/operators/pipe/select_operator_parser.h>
#include <spdlog/spdlog.h>

namespace sierra::odycompiler::parser::testparser::operators::pipe::select {
    odyplan::v3::core::shared::types::OperatorTypeId SelectOperatorParser::getOperatorType() const {
        return std::to_underlying(TYPE);
    }

    std::unique_ptr<odyplan::v3::core::logical::operators::IOperator> SelectOperatorParser::parse(const nlohmann::json &config) {
        try {
            SPDLOG_DEBUG("Parsing SelectOperator");

            if (!config.contains("predicate")) {
                throw std::runtime_error("config must contain 'predicate' field");
            }

            const auto& predicateJson = config["predicate"];

            SPDLOG_INFO("Parsing expression");

            auto predicate = expression::ExpressionParser::parseExpression(predicateJson);

            SPDLOG_DEBUG("Creating SelectOperator");

            return std::make_unique<odyplan::v3::basic::operators::pipe::select::logical::SelectOperator>(std::move(predicate));
        } catch (const std::exception &e) {
            throw std::runtime_error(fmt::format("Failed to parse SelectOperator: {}", e.what()));
        }
    }
}
