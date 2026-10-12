#include <odycompiler/parser/testparser/test_plan_parser.h>

namespace sierra::odycompiler::parser::testparser {
    std::unique_ptr<sierra::odyplan::v3::core::logical::plan::Plan> TestPlanParser::parse(const std::string &input) {
        spdlog::info("Parsing plan with TestPlanParser");

        nlohmann::json json;
        try {
            json = nlohmann::json::parse(input);
        } catch (nlohmann::json::parse_error& e) {
            throw std::runtime_error(fmt::format("JSON parse error: {}", e.what()));
        }

        if (!json.contains("version") && !std::equal_to{}(json["version"],"test")) {
            throw std::runtime_error("TestPlanParser only supports testplans");
        }

        odyplan::v3::core::logical::plan::Plan lPlan;

        if (!json.contains("operators")) {
            throw std::runtime_error("Input must contain 'operators' array");
        }

        std::unordered_map<std::string, odyplan::v3::core::shared::identifiers::OperatorId> operatorIdMap;

        for (const auto& jsonOperator : json["operators"]) {
            const auto id = jsonOperator.value<std::string>("id", "");
            const auto type = jsonOperator.value<odyplan::v3::core::shared::types::OperatorTypeId>("type", -1);

            if (id.empty() || type == std::to_underlying(odyplan::v3::core::shared::types::BuiltinOperatorType::UNKNOWN)) {
                throw std::runtime_error("Operator must have supported 'id' and 'type' fields");
            }

            spdlog::debug("Parsing operator: {} (type: {})", id, type);

            const auto parser = mRegistry.createParser(type);
            const auto config = jsonOperator.value("config", nlohmann::json::object());
            operatorIdMap[id] = parser->parse(lPlan, config);
        }

        spdlog::info("Parsed {} operators", operatorIdMap.size());

        if (json.contains("connections")) {
            for (const auto& conn : json["connections"]) {
                const auto fromId = conn.value<std::string>("from", "");
                const auto toId = conn.value<std::string>("to", "");
                const odyplan::v3::core::shared::identifiers::PortId fromPort = conn.value("from_port", -1);
                const odyplan::v3::core::shared::identifiers::PortId toPort = conn.value("to_port", -1);

                if (fromId.empty() || toId.empty()) {
                    throw std::runtime_error("Connection must have supported 'from' and 'to'");
                }

                if (!operatorIdMap.contains(fromId) || !operatorIdMap.contains(toId)) {
                    throw std::runtime_error("Unknown operator in connection");
                }

                lPlan.connect(
                    odyplan::v3::core::shared::operators::OperatorPortReference{
                        .operatorId = operatorIdMap[fromId],
                        .portId = fromPort
                    },
                    odyplan::v3::core::shared::operators::OperatorPortReference{
                        .operatorId = operatorIdMap[toId],
                        .portId = toPort
                    }
                );
            }

            spdlog::info("Parsed {} connections", json["connections"].size());
        }

        return std::make_unique<sierra::odyplan::v3::core::logical::plan::Plan>(std::move(lPlan));
    }

    const std::string_view TestPlanParser::getName() const {
        return PARSER;
    }
}
