#include <fmt/format.h>
#include <fmt/std.h>
#include <odycompiler/parser/odyparser/operators/pipe/time_window_operator_parser.h>
#include <spdlog/spdlog.h>

namespace sierra::odycompiler::parser::odyparser::operators::pipe::window {
    odyplan::v3::core::shared::types::OperatorTypeId TimeWindowOperatorParser::getOperatorType() const {
        return std::to_underlying(TYPE);
    }

    odyplan::v3::core::shared::identifiers::OperatorId TimeWindowOperatorParser::parse(odyplan::v3::core::logical::plan::Plan& lPlan, const nlohmann::json &config) {
        try {
            spdlog::debug("Parsing TimeWindowOperator");

            odyplan::v3::basic::operators::pipe::window::details::TimeWindowOptions timeOptions;

            timeOptions.size = config.value<std::uint64_t>("size", 1);

            if (config.contains("slide")) {
                timeOptions.slide = config["slide"].get<std::size_t>();
            }

            if (config.contains("advance")) {
                timeOptions.advance = config["advance"].get<std::size_t>();
            }

            spdlog::info("Creating TimeWindowOperator with size={}, slide={}, advance={}", timeOptions.size, timeOptions.slide.has_value() ? std::to_string(timeOptions.slide.value()) : "default" ,timeOptions.advance.has_value() ? std::to_string(timeOptions.advance.value()) : "default");

            return lPlan.addOperator(std::make_unique<odyplan::v3::basic::operators::pipe::window::logical::TimeWindowOperator>(timeOptions));
        } catch (const std::exception &e) {
            throw std::runtime_error(fmt::format("Failed to parse TimeWindowOperator: {}", e.what()));
        }
    }
}
