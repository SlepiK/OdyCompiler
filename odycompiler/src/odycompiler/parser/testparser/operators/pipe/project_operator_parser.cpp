#include <fmt/format.h>
#include <odycompiler/parser/testparser/operators/pipe/project_operator_parser.h>
#include <spdlog/spdlog.h>

namespace sierra::odycompiler::parser::testparser::operators::pipe::project {
    odyplan::v3::core::shared::types::OperatorTypeId ProjectOperatorParser::getOperatorType() const {
        return std::to_underlying(TYPE);
    }

    odyplan::v3::core::shared::identifiers::OperatorId ProjectOperatorParser::parse(odyplan::v3::core::logical::plan::Plan& lPlan, const nlohmann::json &config) {
        try {
            spdlog::debug("Parsing ProjectOperator");

            const bool hasMapping = config.contains("mapping");
            const bool hasNameMapping = config.contains("name_mapping");

            if (hasMapping && hasNameMapping) {
                throw std::runtime_error("ProjectOperator config cannot have both 'mapping' and 'name_mapping'.");
            }

            if (!hasMapping && !hasNameMapping) {
                throw std::runtime_error("ProjectOperator config must contain either 'mapping' or 'name_mapping' field");
            }

            if (hasMapping) {
                const auto mapping = config.value("mapping", odyplan::v3::basic::operators::pipe::project::logical::ProjectOperator::IndexMapping{});

                spdlog::info("Creating with {} field indices", mapping.size());
                spdlog::debug("Mapping: {}", nlohmann::json(mapping).dump());

                return lPlan.addOperator(std::make_unique<odyplan::v3::basic::operators::pipe::project::logical::ProjectOperator>(mapping));
            }

            if (hasNameMapping) {
                const auto nameMapping = config.value("name_mapping", odyplan::v3::basic::operators::pipe::project::logical::ProjectOperator::NameMapping{});

                spdlog::info("Creating with {} field names", nameMapping.size());
                spdlog::debug("Name mapping: {}", nlohmann::json(nameMapping).dump());

                return lPlan.addOperator(std::make_unique<odyplan::v3::basic::operators::pipe::project::logical::ProjectOperator>(nameMapping));
            }

            throw std::runtime_error("EOC");
        } catch (const std::exception &e) {
            throw std::runtime_error(fmt::format("Failed to parse ProjectOperator: {}", e.what()));
        }
    }
}
