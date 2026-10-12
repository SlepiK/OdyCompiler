#include <fmt/format.h>
#include <odycompiler/parser/odyparser/operators/pipe/project_operator_parser.h>
#include <spdlog/spdlog.h>

namespace sierra::odycompiler::parser::odyparser::operators::pipe::project {
    odyplan::v3::core::shared::types::OperatorTypeId ProjectOperatorParser::getOperatorType() const {
        return std::to_underlying(TYPE);
    }

    odyplan::v3::core::shared::identifiers::OperatorId ProjectOperatorParser::parse(odyplan::v3::core::logical::plan::Plan& lPlan, const nlohmann::json &config) {
        try {
            spdlog::debug("Parsing ProjectOperator");

            if (!config.contains("attributes")) {
                throw std::runtime_error("ProjectOperator config must contain 'attributes' field");
            }

            const auto attributes = config.value("attributes", std::vector<std::string>{});

            if (attributes.empty()) {
                throw std::runtime_error("ProjectOperator 'attributes' cannot be empty");
            }

            odyplan::v3::basic::operators::pipe::project::logical::ProjectOperator::NameMapping nameMapping;
            for (const auto& attr : attributes) {
                nameMapping.push_back(attr);
            }

            spdlog::info("Creating ProjectOperator with {} attributes", nameMapping.size());
            spdlog::debug("Attributes: {}", nlohmann::json(nameMapping).dump());

            return lPlan.addOperator(std::make_unique<odyplan::v3::basic::operators::pipe::project::logical::ProjectOperator>(nameMapping));
        } catch (const std::exception &e) {
            throw std::runtime_error(fmt::format("Failed to parse ProjectOperator: {}", e.what()));
        }
    }
}
