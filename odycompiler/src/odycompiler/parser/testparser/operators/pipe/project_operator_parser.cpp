#include <fmt/format.h>
#include <odycompiler/parser/testparser/operators/pipe/project_operator_parser.h>
#include <spdlog/spdlog.h>

namespace sierra::odycompiler::parser::testparser::operators::pipe::project {
    odyplan::v3::core::shared::types::OperatorTypeId ProjectOperatorParser::getOperatorType() const {
        return std::to_underlying(TYPE);
    }

    std::unique_ptr<odyplan::v3::core::logical::operators::IOperator> ProjectOperatorParser::parse(const nlohmann::json &config) {
        try {
            SPDLOG_DEBUG("Parsing ProjectOperator");

            const bool hasMapping = config.contains("mapping");
            const bool hasNameMapping = config.contains("name_mapping");

            if (hasMapping && hasNameMapping) {
                throw std::runtime_error("ProjectOperator config cannot have both 'mapping' and 'name_mapping'.");
            }

            if (!hasMapping && !hasNameMapping) {
                throw std::runtime_error("ProjectOperator config must contain either 'mapping' or 'name_mapping' field");
            }

            std::unique_ptr<odyplan::v3::basic::operators::pipe::project::logical::ProjectOperator> op;

            if (hasMapping) {
                const auto mapping = config.value("mapping", odyplan::v3::basic::operators::pipe::project::logical::ProjectOperator::IndexMapping{});

                SPDLOG_INFO("Creating with {} field indices", mapping.size());
                SPDLOG_DEBUG("Mapping: {}", nlohmann::json(mapping).dump());

                op = std::make_unique<odyplan::v3::basic::operators::pipe::project::logical::ProjectOperator>(mapping);
            } else if (hasNameMapping) {
                const auto nameMapping = config.value("name_mapping", odyplan::v3::basic::operators::pipe::project::logical::ProjectOperator::NameMapping{});

                SPDLOG_INFO("Creating with {} field names", nameMapping.size());
                SPDLOG_DEBUG("Name mapping: {}", nlohmann::json(nameMapping).dump());

                op = std::make_unique<odyplan::v3::basic::operators::pipe::project::logical::ProjectOperator>(nameMapping);
            }

            return op;
        } catch (const std::exception &e) {
            throw std::runtime_error(fmt::format("Failed to parse ProjectOperator: {}", e.what()));
        }
    }
}
