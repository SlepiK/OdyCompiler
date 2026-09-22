#include <fmt/format.h>
#include <odycompiler/parser/testparser/operators/pipe/count_window_operator_parser.h>
#include <spdlog/spdlog.h>

namespace sierra::odycompiler::parser::testparser::operators::pipe::window {
    odyplan::v3::core::shared::types::OperatorTypeId CountWindowOperatorParser::getOperatorType() const {
        return std::to_underlying(TYPE);
    }

    odyplan::v3::core::shared::identifiers::OperatorId CountWindowOperatorParser::parse(odyplan::v3::core::logical::plan::Plan& lPlan, const nlohmann::json &config) {
        try {
            spdlog::debug("Parsing CountWindowOperator");

            if (!config.contains("config")) {
                throw std::runtime_error("config must contain 'config' field");
            }

            const auto countWindowConfig = config.value("config", nlohmann::json::object());

            odyplan::v3::basic::window::count::CountWindowConfig countConfig;

            countConfig.size = config.value<std::uint64_t>("size", 1);
            countConfig.slide = config.value<std::uint64_t>("slide", 1);
            countConfig.advance = config.value<std::uint64_t>("advance", 1);

            spdlog::info("Creating CountWindowOperator with size={}, slide={}, advance={}", countConfig.size, countConfig.slide, countConfig.advance);

            return lPlan.addOperator(std::make_unique<odyplan::v3::basic::operators::pipe::window::logical::CountWindowOperator>(countConfig));
        } catch (const std::exception &e) {
            throw std::runtime_error(fmt::format("Failed to parse CountWindowOperator: {}", e.what()));
        }
    }
}
