#include <odycompiler/parser/testparser/test_plan_parser.h>

namespace sierra::odycompiler::parser::testparser {
    std::unique_ptr<sierra::odyplan::v3::core::logical::plan::Plan> TestPlanParser::parse(const std::string &input) {
        SPDLOG_INFO("Parsing plan with TestPlanParser");

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

        std::unordered_map<odyplan::v3::core::shared::identifiers::OperatorId, odyplan::v3::core::shared::identifiers::OperatorId> operatorIdMap;

        for (const auto& jsonOperator : json["operators"]) {
            const odyplan::v3::core::shared::identifiers::OperatorId id = jsonOperator.value("id", -1);
            const odyplan::v3::core::shared::types::OperatorTypeId type = jsonOperator.value("type", -1);

            if (id == -1 || type == std::to_underlying(odyplan::v3::core::shared::types::BuiltinOperatorType::UNKNOWN)) {
                throw std::runtime_error("Operator must have supported 'id' and 'type' fields");
            }

            SPDLOG_DEBUG("Parsing operator: {} (type: {})", id, type);

            const auto parser = mRegistry.createParser(type);
            const auto config = jsonOperator.value("config", nlohmann::json::object());
            auto opr = parser->parse(config);

            operatorIdMap[id] = lPlan.addOperator(std::move(opr));
        }

        SPDLOG_INFO("Parsed {} operators", operatorIdMap.size());

        if (json.contains("connections")) {
            for (const auto& conn : json["connections"]) {
                const odyplan::v3::core::shared::identifiers::OperatorId fromId = conn.value("from", -1);
                const odyplan::v3::core::shared::identifiers::OperatorId toId = conn.value("to", -1);
                const odyplan::v3::core::shared::identifiers::PortId fromPort = conn.value("from_port", -1);
                const odyplan::v3::core::shared::identifiers::PortId toPort = conn.value("to_port", -1);

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

            SPDLOG_INFO("Parsed {} connections", json["connections"].size());
        }

        return std::make_unique<sierra::odyplan::v3::core::logical::plan::Plan>(std::move(lPlan));
    }

    const std::string& TestPlanParser::getName() const {
        return PARSER;
    }
}
