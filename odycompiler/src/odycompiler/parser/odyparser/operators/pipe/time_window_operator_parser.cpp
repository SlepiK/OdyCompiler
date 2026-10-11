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

            if (!config.contains("options")) {
                throw std::runtime_error("config must contain 'options' field");
            }

            const auto options = config.value("options", nlohmann::json::object());
            const auto timeWindowConfig = config.value("config", nlohmann::json::object());

            odyplan::v3::basic::operators::pipe::window::details::TimeWindowOptions timeOptions;

            timeOptions.size = options.value<std::uint64_t>("size", 1);

            if (options.contains("slide")) {
                timeOptions.slide = options["slide"].get<std::size_t>();
            }

            if (options.contains("advance")) {
                timeOptions.advance = options["advance"].get<std::size_t>();
            }

            spdlog::info("Creating TimeWindowOperator with size={}, slide={}, advance={}", timeOptions.size, timeOptions.slide.has_value() ? std::to_string(timeOptions.slide.value()) : "default" ,timeOptions.advance.has_value() ? std::to_string(timeOptions.advance.value()) : "default");

            return lPlan.addOperator(std::make_unique<odyplan::v3::basic::operators::pipe::window::logical::TimeWindowOperator>(timeOptions));
        } catch (const std::exception &e) {
            throw std::runtime_error(fmt::format("Failed to parse TimeWindowOperator: {}", e.what()));
        }
    }
}
