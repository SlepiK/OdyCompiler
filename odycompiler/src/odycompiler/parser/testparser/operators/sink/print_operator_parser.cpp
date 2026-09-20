#include <fmt/format.h>
#include <odycompiler/parser/testparser/operators/sink/print_operator_parser.h>
#include <spdlog/spdlog.h>

namespace sierra::odycompiler::parser::testparser::operators::sink::print {
    odyplan::v3::core::shared::types::OperatorTypeId PrintOperatorParser::getOperatorType() const {
        return std::to_underlying(TYPE);
    }

    std::unique_ptr<odyplan::v3::core::logical::operators::IOperator> PrintOperatorParser::parse(const nlohmann::json &config) {
        try {
            SPDLOG_DEBUG("Parsing PrintOperator");

            if (!config.contains("config")) {
                throw std::runtime_error("config must contain 'config' field");
            }

            const auto printConfig = config.value("config", nlohmann::json::object());

            odyplan::v3::basic::operators::sink::print::details::PrintOperatorOptions tPrintConfig;

            tPrintConfig.printPunctuation = config.value<bool>("printPunctuation", false);

            const bool hasIndices = config.contains("port_field_indices");
            const bool hasNames = config.contains("port_field_names");

            std::unique_ptr<odyplan::v3::basic::operators::sink::print::logical::PrintOperator> opr;

            if (hasIndices && hasNames) {
                throw std::runtime_error(
                    "config cannot have both 'port_field_indices' and 'port_field_names'"
                );
            }

            if (hasIndices) {
                const auto indices = config.value("port_field_indices", odyplan::v3::basic::operators::sink::print::logical::PrintOperator::PortFieldIndexV{});

                SPDLOG_INFO("Creating with {} field indices", indices.size());
                SPDLOG_DEBUG("Indices: {}", nlohmann::json(indices).dump());
                opr = std::make_unique<odyplan::v3::basic::operators::sink::print::logical::PrintOperator>(tPrintConfig,indices);

            } else if (hasNames) {
                const auto names = config.value("port_field_names", odyplan::v3::basic::operators::sink::print::logical::PrintOperator::PortFieldNameV{});

                SPDLOG_INFO("Creating with {} field names", names.size());
                SPDLOG_DEBUG("Names: {}", nlohmann::json(names).dump());

                opr = std::make_unique<odyplan::v3::basic::operators::sink::print::logical::PrintOperator>(tPrintConfig,names);
            } else {
                SPDLOG_INFO("Creating with all fields");
                opr = std::make_unique<odyplan::v3::basic::operators::sink::print::logical::PrintOperator>(tPrintConfig);
            }
            return opr;
        } catch (const std::exception &e) {
            throw std::runtime_error(fmt::format("Failed to parse PrintOperator: {}", e.what()));
        }
    }
}
