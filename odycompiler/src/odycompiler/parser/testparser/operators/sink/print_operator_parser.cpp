#include <fmt/format.h>
#include <odycompiler/parser/testparser/operators/sink/print_operator_parser.h>
#include <spdlog/spdlog.h>

namespace sierra::odycompiler::parser::testparser::operators::sink::print {
    odyplan::v3::core::shared::types::OperatorTypeId PrintOperatorParser::getOperatorType() const {
        return std::to_underlying(TYPE);
    }

    odyplan::v3::core::shared::identifiers::OperatorId PrintOperatorParser::parse(odyplan::v3::core::logical::plan::Plan& lPlan, const nlohmann::json &config) {
        try {
            spdlog::debug("Parsing PrintOperator");

            if (config.empty()) {
                throw std::runtime_error("config is empty");
            }

            const auto printConfig = config.value("config", nlohmann::json::object());

            odyplan::v3::basic::operators::sink::print::details::PrintOperatorOptions tPrintConfig;

            tPrintConfig.printPunctuation = config.value<bool>("printPunctuation", false);

            const bool hasIndices = config.contains("port_field_indices");
            const bool hasNames = config.contains("port_field_names");

            if (hasIndices && hasNames) {
                throw std::runtime_error(
                    "config cannot have both 'port_field_indices' and 'port_field_names'"
                );
            }

            if (hasIndices) {
                const auto indices = config.value("port_field_indices", odyplan::v3::basic::operators::sink::print::logical::PrintOperator::PortFieldIndexV{});

                spdlog::info("Creating with {} field indices", indices.size());
                spdlog::debug("Indices: {}", nlohmann::json(indices).dump());
                return lPlan.addOperator(std::make_unique<odyplan::v3::basic::operators::sink::print::logical::PrintOperator>(tPrintConfig,indices));

            }

            if (hasNames) {
                const auto names = config.value("port_field_names", odyplan::v3::basic::operators::sink::print::logical::PrintOperator::PortFieldNameV{});

                spdlog::info("Creating with {} field names", names.size());
                spdlog::debug("Names: {}", nlohmann::json(names).dump());

                return lPlan.addOperator(std::make_unique<odyplan::v3::basic::operators::sink::print::logical::PrintOperator>(tPrintConfig,names));
            }

            spdlog::info("Creating with all fields");
            return lPlan.addOperator(std::make_unique<odyplan::v3::basic::operators::sink::print::logical::PrintOperator>(tPrintConfig));
        } catch (const std::exception &e) {
            throw std::runtime_error(fmt::format("Failed to parse PrintOperator: {}", e.what()));
        }
    }
}
