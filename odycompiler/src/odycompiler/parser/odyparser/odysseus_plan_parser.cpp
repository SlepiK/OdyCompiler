#include <regex>
#include <odycompiler/parser/odyparser/odysseus_plan_parser.h>

namespace sierra::odycompiler::parser::odyparser {
    std::unique_ptr<sierra::odyplan::v3::core::logical::plan::Plan>
    OdysseusPlanParser::parse(const std::string &input) {
        spdlog::info("Parsing plan with OdysseusPlanParser");

        nlohmann::json json;
        json["operators"] = nlohmann::json::array();
        json["connections"] = nlohmann::json::array();

        std::unordered_map<std::string, int> operatorIndex;
        std::unordered_set<std::string> hasOutgoingConnection;
        std::unordered_map<std::string, std::string> totupleRedirect;

        std::istringstream stream(input);
        std::string line;
        std::string currentBlock;
        int braceCount = 0;
        bool inBlock = false;

        while (std::getline(stream, line)) {
            line = trim(line);
            if (line.empty() || line.starts_with("//") || line.starts_with("#")) {
                continue;
            }

            if (!inBlock) {
                currentBlock = line;
                braceCount = countBrace(line);
                if (line.contains('{')) inBlock = true;
            } else {
                currentBlock += " " + line;
                braceCount += countBrace(line);
            }

            if (inBlock && braceCount == 0) {
                if (auto op = parseOperatorLine(currentBlock)) {
                    std::string type = op->at("type").get<std::string>();

                    if (isSkippableOperator(type)) {
                        if (op->contains("args")) {
                            std::string argsStr = op->at("args").get<std::string>();
                            std::stringstream ss(argsStr);
                            if (std::string inputOp; std::getline(ss, inputOp, ',')) {
                                inputOp = trim(inputOp);
                                totupleRedirect[op->at("id").get<std::string>()] = inputOp;
                                spdlog::debug("TOTUPLE '{}' redirects to '{}'", op->at("id").get<std::string>(), inputOp);
                            }
                        }
                        currentBlock.clear();
                        inBlock = false;
                        continue;
                    }

                    nlohmann::json opJson;
                    opJson["id"] = op->at("id").get<std::string>();
                    opJson["type"] = getOperatorTypeId(mapOperatorType(type));
                    opJson["config"] = op->value("config", nlohmann::json::object());
                    json["operators"].push_back(opJson);

                    operatorIndex[opJson["id"]] = static_cast<int>(json["operators"].size()) - 1;

                    if (op->contains("args")) {
                        buildConnections(op->at("args").get<std::string>(), json["connections"], operatorIndex, hasOutgoingConnection,
                                         totupleRedirect);
                    }
                }
                currentBlock.clear();
                inBlock = false;
            }
        }

#ifdef ODYSSEUS_PLAN_PARSER_AUTO_PRINT_OPERATOR
        addPrintOperators(json["operators"], json["connections"], operatorIndex, hasOutgoingConnection);
#endif

        return parseJsonPlan(json);
    }

    const std::string_view OdysseusPlanParser::getName() const {
        return PARSER;
    }

    std::optional<nlohmann::json> OdysseusPlanParser::parseOperatorLine(const std::string& line) {
        const std::size_t eqPos = line.find('=');
        if (eqPos == std::string::npos) return std::nullopt;

        std::string name = trim(line.substr(0, eqPos));
        std::string rest = trim(line.substr(eqPos + 1));

        const std::size_t openParen = rest.find('(');
        const std::size_t closeParen = rest.rfind(')');
        if (openParen == std::string::npos || closeParen == std::string::npos) {
            return std::nullopt;
        }

        const std::string type = trim(rest.substr(0, openParen));
        const std::string content = rest.substr(openParen + 1, closeParen - openParen - 1);

        const std::size_t braceEnd = content.rfind('}');
        const std::string configStr = (braceEnd != std::string::npos) ?
                                content.substr(0, braceEnd + 1) : content;
        std::string argsStr = (braceEnd != std::string::npos && braceEnd + 1 < content.size()) ?
                              content.substr(braceEnd + 1) : "";

        if (!argsStr.empty() && argsStr[0] == ',') {
            argsStr = argsStr.substr(1);
        }

        const nlohmann::json config = configStrToJson(configStr);

        nlohmann::json op;
        op["id"] = name;
        op["type"] = type;
        op["config"] = config;
        if (!argsStr.empty()) {
            op["args"] = argsStr;
        }

        return op;
    }

    nlohmann::json OdysseusPlanParser::configStrToJson(const std::string &config) {
        std::string jsonStr = config;

        for (char &c: jsonStr) {
            if (c == '\'') c = '"';
        }

        std::string inner = trim(jsonStr);
        if (inner.starts_with('{') && inner.ends_with('}')) {
            inner = inner.substr(1, inner.size() - 2);
        }

        nlohmann::json result = nlohmann::json::object();

        size_t i = 0;
        while (i < inner.size()) {
            while (i < inner.size() && std::isspace(inner[i])) {
                i++;
            }

            if (i >= inner.size()) {
                break;
            }

            const size_t keyStart = i;
            while (i < inner.size() && inner[i] != '=' && inner[i] != ':') {
                i++;
            }

            if (i >= inner.size()) {
                break;
            }

            std::string key = trim(inner.substr(keyStart, i - keyStart));
            i++;

            while (i < inner.size() && std::isspace(inner[i])) i++;
            if (i >= inner.size()) {
                break;
            }

            std::string value;
            if (inner[i] == '"') {
                const std::size_t valueStart = i;
                i++;
                while (i < inner.size() && inner[i] != '"') {
                    if (inner[i] == '\\' && i + 1 < inner.size()) {
                        i++;
                    }
                    i++;
                }
                i++;
                value = inner.substr(valueStart, i - valueStart);
            } else if (inner[i] == '[') {
                const size_t valueStart = i;
                int bracketDepth = 0;
                bool inString = false;
                while (i < inner.size()) {
                    if (inner[i] == '"' && (i == 0 || inner[i - 1] != '\\')) {
                        inString = !inString;
                    } else if (!inString) {
                        if (inner[i] == '[') {
                            bracketDepth++;
                        } else if (inner[i] == ']') {
                            bracketDepth--;
                            if (bracketDepth == 0) {
                                i++;
                                break;
                            }
                        }
                    }
                    i++;
                }
                value = inner.substr(valueStart, i - valueStart);
            } else if (inner[i] == '{') {
                const std::size_t valueStart = i;
                int braceDepth = 0;
                bool inString = false;
                while (i < inner.size()) {
                    if (inner[i] == '"' && (i == 0 || inner[i - 1] != '\\')) {
                        inString = !inString;
                    } else if (!inString) {
                        if (inner[i] == '{') {
                            braceDepth++;
                        } else if (inner[i] == '}') {
                            braceDepth--;
                            if (braceDepth == 0) {
                                i++;
                                break;
                            }
                        }
                    }
                    i++;
                }
                value = inner.substr(valueStart, i - valueStart);
            } else {
                const std::size_t valueStart = i;
                while (i < inner.size() && inner[i] != ',' && inner[i] != '}' && !std::isspace(inner[i])) {
                    i++;
                }
                value = trim(inner.substr(valueStart, i - valueStart));
                if (!value.empty() && !std::isdigit(value[0]) && value[0] != '-' && value[0] != '.') {
                    value = "\"" + value + "\"";
                }
            }

            try {
                if (value.starts_with('[') || value.starts_with('{')) {
                    result[key] = nlohmann::json::parse(value);
                } else if (value.starts_with('"')) {
                    result[key] = nlohmann::json::parse(value);
                } else if (!value.empty() && (std::isdigit(value[0]) || value[0] == '-')) {
                    result[key] = nlohmann::json::parse(value);
                } else {
                    result[key] = value;
                }
            } catch (nlohmann::json::parse_error &e) {
                spdlog::warn("Failed to parse '{}': {}", key, e.what());
                result[key] = value;
            }

            while (i < inner.size() && (std::isspace(inner[i]) || inner[i] == ',')) {
                i++;
            }
        }

        spdlog::debug("Parsed config: {}", result.dump(2));
        return result;
    }

    void OdysseusPlanParser::buildConnections(const std::string& args, nlohmann::json& connections, const std::unordered_map<std::string, int>& opIndex, std::unordered_set<std::string>& hasOutgoing, const std::unordered_map<std::string, std::string>& totupleRedirect) {
        std::vector<std::string> ops;
        std::stringstream ss(args);
        std::string op;

        while (std::getline(ss, op, ',')) {
            op = trim(op);

            while (totupleRedirect.contains(op)) {
                spdlog::debug("Redirecting TOTUPLE: {} → {}", op, totupleRedirect.at(op));
                op = totupleRedirect.at(op);
            }

            if (opIndex.contains(op)) {
                ops.push_back(op);
                hasOutgoing.insert(op);
            }
        }

        for (size_t i = 0; i + 1 < ops.size(); ++i) {
            nlohmann::json conn;
            conn["from"] = ops[i];
            conn["to"] = ops[i + 1];
            conn["from_port"] = 0;
            conn["to_port"] = 0;
            connections.push_back(conn);
        }
    }

    std::string OdysseusPlanParser::mapOperatorType(const std::string& type) {
        //QD - Change later by something better
        static const std::unordered_map<std::string, std::string> map = {
            {"ACCESS", "read"}, {"SINK", "write"}, {"TIMEWINDOW", "window"},
            {"JOIN", "join"}, {"TOTUPLE", "convert"}, {"MAP", "map"},
            {"FILTER", "filter"}, {"UNION", "union"}
        };
        auto it = map.find(type);
        return (it != map.end()) ? it->second : type;
    }

    int OdysseusPlanParser::getOperatorTypeId(const std::string& type) {
        //QD - Change later by something better
        static const std::unordered_map<std::string, int> map = {
            {"read", 21}, {"write", 20}, {"window", 30}, {"join", 40},
            {"convert", 50}, {"map", 60}, {"filter", 70}, {"union", 80}, {"print", 20}
        };
        auto it = map.find(type);
        return (it != map.end()) ? it->second : -1;
    }

    std::unique_ptr<sierra::odyplan::v3::core::logical::plan::Plan> OdysseusPlanParser::parseJsonPlan(const nlohmann::json& json) {
        if (!json.contains("operators")) {
            throw std::runtime_error("Input must contain 'operators' array");
        }

        odyplan::v3::core::logical::plan::Plan lPlan;
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

    nlohmann::json OdysseusPlanParser::parseInfixExpression(const std::string& expr) {
        std::string trimmed = trim(expr);

        static const std::vector<std::pair<std::string, int>> operators = {
            {" AND ", 0},
            {" OR ", 1},
            {" = ", 0},
            {" == ", 0},
            {" != ", 1},
            {" < ", 2},
            {" <= ", 3},
            {" > ", 4},
            {" >= ", 5},
        };

        for (const auto& [op, opId] : operators) {
            if (std::size_t pos = findOperatorInExpression(trimmed, op); pos != std::string::npos) {
                const std::string left = trimmed.substr(0, pos);
                const std::string right = trimmed.substr(pos + op.length());

                const bool isLogical = (op == " AND " || op == " OR ");

                nlohmann::json node;
                node["type"] = isLogical ? 3 : 2;
                node["op"] = opId;
                node["left"] = parseInfixExpression(left);
                node["right"] = parseInfixExpression(right);
                return node;
            }
        }

        if (trimmed.starts_with("NOT ")) {
            std::string operand = trimmed.substr(4);
            nlohmann::json node;
            node["type"] = 3;
            node["op"] = 2;
            node["operand"] = parseInfixExpression(operand);
            return node;
        }

        if (trimmed.starts_with('(') && trimmed.ends_with(')')) {
            return parseInfixExpression(trimmed.substr(1, trimmed.size() - 2));
        }

        return createExpressionLeaf(trimmed);
    }

    nlohmann::json OdysseusPlanParser::createExpressionLeaf(const std::string& token) {
        std::string t = trim(token);
        nlohmann::json node;

        if ((t.starts_with('"') && t.ends_with('"')) ||
        (t.starts_with('\'') && t.ends_with('\''))) {
            node["type"] = 1;
            node["typeId"] = 20;
            node["value"] = t.substr(1, t.size() - 2);
        } else if (std::isdigit(t[0]) || (t[0] == '-' && t.size() > 1)) {
            node["type"] = 1;
            node["typeId"] = t.find('.') != std::string::npos ? 21 : 19; // Double oder Integer
            node["value"] = t;
        } else {
            node["type"] = 0;
            node["field"] = t;
            node["port"] = -1;
        }

        return node;
    }

#ifdef ODYSSEUS_PLAN_PARSER_AUTO_PRINT_OPERATOR
    void OdysseusPlanParser::addPrintOperators(nlohmann::json& operators, nlohmann::json& connections, std::unordered_map<std::string, int>& opIndex, std::unordered_set<std::string>& hasOutgoing) {
        int printCnt = 0;

        for (const auto& op : operators) {
            if (std::string opId = op["id"]; !hasOutgoing.contains(opId)) {
                std::string printId = fmt::format("print_{}", printCnt++);

                spdlog::info("Adding PrintOperator '{}' after '{}'", printId, opId);

                nlohmann::json printOp;
                printOp["id"] = printId;
                printOp["type"] = getOperatorTypeId("print");
                printOp["config"] = {
                    {"options", nlohmann::json::object()}
                };
                operators.push_back(printOp);

                nlohmann::json conn;
                conn["from"] = opId;
                conn["to"] = printId;
                conn["from_port"] = 0;
                conn["to_port"] = 0;
                connections.push_back(conn);
            }
        }

        spdlog::info("Added {} PrintOperators", printCnt);
    }
#endif
}
