#include <odycompiler/parser/dev_plan_parser.h>
#include <spdlog/spdlog.h>

namespace sierra::odycompiler::parser {
    std::unique_ptr<sierra::odyplan::v3::core::logical::plan::Plan> DevPlanParser::parse(const std::string &input) {
        SPDLOG_INFO("Parsing development plan");

        nlohmann::json json;
        try {
            json = nlohmann::json::parse(input);
        } catch (const nlohmann::json::exception& e) {
            throw std::runtime_error(fmt::format("Parse error: {}", e.what()));
        }

        odyplan::v3::core::logical::plan::Plan lPlan;

        std::unordered_map<std::string, std::size_t> operatorMap;

        if (!json.contains("operators")) {
            throw std::runtime_error(fmt::format("No operator array found"));
        }

        for (const auto& opObj : json["operators"]) {
            const std::string id = opObj.value("id", "");
            const std::string type = opObj.value("type", "");

            if (id.empty()) {
                throw std::runtime_error(fmt::format("Invalid operator id {}", id));
            }

            if (type.empty()) {
                throw std::runtime_error(fmt::format("Operator '{}' must have a type field", id));
            }

            SPDLOG_DEBUG("Parsing operator {} (type: {})", id, type);

            auto op = parseOperator(opObj);
            const std::size_t lOpId = lPlan.addOperator(std::move(op));
            operatorMap[id] = lOpId;
        }

        SPDLOG_INFO("Parsed {} operators", json["operators"].size());

        if (json.contains("connections")) {
            for (const auto& connObj : json["connections"]) {
                const std::string fromId = connObj.value("from", "");
                const std::string toId = connObj.value("to", "");
                const std::size_t fromPort = connObj.value("from_port", 0);
                const std::size_t toPort = connObj.value("to_port", 0);

                if (!operatorMap.contains(fromId)) {
                    throw std::runtime_error(fmt::format("Unknown operator in connection 'from': {}", fromId));
                }

                if (!operatorMap.contains(toId)) {
                    throw std::runtime_error(fmt::format("Unknown operator in connection 'to': {}", toId));
                }

                SPDLOG_DEBUG("Connecting {}:{} -> {}:{}", fromId, fromPort, toId, toPort);

                lPlan.connect(
                    odyplan::v3::core::shared::operators::OperatorPortReference{
                        .operatorId = operatorMap.at(fromId),
                        .portId = fromPort
                    },
                    odyplan::v3::core::shared::operators::OperatorPortReference{
                        .operatorId = operatorMap.at(toId),
                        .portId = toPort
                    }
                );
            }

            SPDLOG_INFO("Parsed {} connections", json["connections"].size());
        }

        return std::make_unique<odyplan::v3::core::logical::plan::Plan>(std::move(lPlan));
    }

    std::unique_ptr<sierra::odyplan::v3::core::logical::operators::IOperator> DevPlanParser::parseOperator(const nlohmann::json &op) {
        const std::string type = op.value("type", "");
        const nlohmann::json config = op.value("config", nlohmann::json::object());

    }
}
