#include <fmt/format.h>
#include <odycompiler/parser/testparser/operators/pipe/measure_operator_parser.h>
#include <spdlog/spdlog.h>

namespace sierra::odycompiler::parser::testparser::operators::pipe::measure {
    odyplan::v3::core::shared::types::OperatorTypeId MeasureOperatorParser::getOperatorType() const {
        return std::to_underlying(TYPE);
    }

    odyplan::v3::core::shared::identifiers::OperatorId MeasureOperatorParser::parse(odyplan::v3::core::logical::plan::Plan& lPlan, const nlohmann::json &config) {
        try {
            spdlog::debug("Parsing MeasureOperator");

            if (!config.contains("label")) {
                throw std::runtime_error("config must contain 'label' field");
            }

            std::string label = config.value("label", "");

            if (label.empty()) {
                throw std::runtime_error("MeasureOperator label cannot be empty");
            }

            spdlog::info("Creating MeasureOperator with label={}", label);

            return lPlan.addOperator(std::make_unique<odyplan::v3::basic::operators::pipe::measure::logical::MeasureOperator>(std::move(label)));
        } catch (const std::exception &e) {
            throw std::runtime_error(fmt::format("Failed to parse MeasureOperator: {}", e.what()));
        }
    }
}
