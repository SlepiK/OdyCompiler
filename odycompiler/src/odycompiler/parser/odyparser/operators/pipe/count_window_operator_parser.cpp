#include <fmt/format.h>
#include <odycompiler/parser/odyparser/operators/pipe/count_window_operator_parser.h>
#include <spdlog/spdlog.h>

namespace sierra::odycompiler::parser::odyparser::operators::pipe::window {
    odyplan::v3::core::shared::types::OperatorTypeId CountWindowOperatorParser::getOperatorType() const {
        return std::to_underlying(TYPE);
    }

    odyplan::v3::core::shared::identifiers::OperatorId CountWindowOperatorParser::parse(odyplan::v3::core::logical::plan::Plan& lPlan, const nlohmann::json &config) {
        try {
            spdlog::debug("Parsing CountWindowOperator");

            odyplan::v3::basic::operators::pipe::window::details::CountWindowOptions countOptions;

            countOptions.size = config.value<std::uint64_t>("size", 1);
            countOptions.slide = config.value<std::uint64_t>("slide", 1);
            countOptions.advance = config.value<std::uint64_t>("advance", 1);

            spdlog::info("Creating CountWindowOperator with size={}, slide={}, advance={}", countOptions.size, countOptions.slide, countOptions.advance);

            return lPlan.addOperator(std::make_unique<odyplan::v3::basic::operators::pipe::window::logical::CountWindowOperator>(countOptions));
        } catch (const std::exception &e) {
            throw std::runtime_error(fmt::format("Failed to parse CountWindowOperator: {}", e.what()));
        }
    }
}
