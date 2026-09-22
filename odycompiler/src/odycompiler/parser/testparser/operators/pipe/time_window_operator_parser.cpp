#include <fmt/format.h>
#include <fmt/std.h>
#include <odycompiler/parser/testparser/operators/pipe/time_window_operator_parser.h>
#include <spdlog/spdlog.h>

namespace sierra::odycompiler::parser::testparser::operators::pipe::window {
    odyplan::v3::core::shared::types::OperatorTypeId TimeWindowOperatorParser::getOperatorType() const {
        return std::to_underlying(TYPE);
    }

    odyplan::v3::core::shared::identifiers::OperatorId TimeWindowOperatorParser::parse(odyplan::v3::core::logical::plan::Plan& lPlan, const nlohmann::json &config) {
        try {
            spdlog::debug("Parsing TimeWindowOperator");

            if (config.empty()) {
                throw std::runtime_error("config is empty");
            }

            const auto timeWindowConfig = config.value("config", nlohmann::json::object());

            odyplan::v3::basic::window::time::TimeWindowConfig timeConfig;

            timeConfig.size = config.value<std::uint64_t>("size", 1);
            timeConfig.slide = config.value<std::uint64_t>("slide", 1);
            timeConfig.advance = config.value<std::uint64_t>("advance", 1);

            spdlog::info(fmt::format("Creating TimeWindowOperator with size={}, slide={}, advance={}", timeConfig.size, timeConfig.slide, timeConfig.advance));

            return lPlan.addOperator(std::make_unique<odyplan::v3::basic::operators::pipe::window::logical::TimeWindowOperator>(timeConfig));
        } catch (const std::exception &e) {
            throw std::runtime_error(fmt::format("Failed to parse TimeWindowOperator: {}", e.what()));
        }
    }
}
