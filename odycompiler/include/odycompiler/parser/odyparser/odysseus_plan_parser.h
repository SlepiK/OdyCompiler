#ifndef SIERRA_ODYCOMPILER_PARSER_ODYPARSER_ODYSSEUSPLANPARSER_HPP
#define SIERRA_ODYCOMPILER_PARSER_ODYPARSER_ODYSSEUSPLANPARSER_HPP

#include <odycompiler/parser/plan_parser.h>
#include <odycompiler/parser/odyparser/odysseus_operator_parser_registry.h>
#include <core/shared/types/operator_type_id.h>

namespace sierra::odycompiler::parser::odyparser {
    class OdysseusPlanParser : public IPlanParser {
    public:
        static constexpr std::string_view PARSER = "OdysseusPlanParser";

        OdysseusPlanParser() = default;
        ~OdysseusPlanParser() override = default;

        std::unique_ptr<sierra::odyplan::v3::core::logical::plan::Plan> parse(const std::string &input) override;

        [[nodiscard]] const std::string_view getName() const override;

    private:
        OdysseusOperatorParserRegistry& mRegistry = OdysseusOperatorParserRegistry::instance();

        std::unique_ptr<sierra::odyplan::v3::core::logical::plan::Plan> parseJsonPlan(const nlohmann::json &json,
            const std::unordered_map<std::string, std::string> &totupleRedirect);

        nlohmann::json configStrToJson(const std::string& config);

        void buildConnections(const std::string& args, nlohmann::json& connections, const std::unordered_map<std::string, int>& opIndex, std::unordered_set<std::string>& hasOutgoing, const std::unordered_map<std::string, std::string>& totupleRedirect);

        std::string mapOperatorType(const std::string& type);

        int getOperatorTypeId(const std::string& type);

        static std::string trim(const std::string& str) {
            const size_t first = str.find_first_not_of(" \t\n\r");

            if (first == std::string::npos) {
                return "";
            }

            const size_t last = str.find_last_not_of(" \t\n\r");

            return str.substr(first, last - first + 1);
        }

        nlohmann::json parseInfixExpression(const std::string& expr);

        nlohmann::json createExpressionLeaf(const std::string& token);

        static size_t findOperatorInExpression(const std::string& expr, const std::string& op) {
            int parenDepth = 0;
            for (size_t i = 0; i + op.size() <= expr.size(); ++i) {
                if (expr[i] == '(') {
                    parenDepth++;
                } else if (expr[i] == ')') {
                    parenDepth--;
                } else if (parenDepth == 0 && expr.substr(i, op.size()) == op) {
                    return i;
                }
            }
            return std::string::npos;
        }
        static bool isEquals(std::string_view a, std::string_view b) {
            return std::ranges::equal(a, b, [](unsigned char x, unsigned char y) {
                return std::tolower(x) == std::tolower(y);
            });
        }

        static bool isSkippableOperator(const std::string_view type) {
            return isEquals(type, "totuple");
        }

        static bool isExpressionField(const std::string_view key) {
            return isEquals(key, "predicate");
        }

        static int countBrace(const std::string& str) {
            int count = 0;
            bool inString = false;
            for (size_t i = 0; i < str.size(); ++i) {
                if (const char c = str[i]; c == '"' && (i == 0 || str[i-1] != '\\')) inString = !inString;
                else if (!inString) {
                    if (c == '{') count++;
                    else if (c == '}') count--;
                }
            }
            return count;
        }

        std::optional<nlohmann::json> parseOperatorLine(const std::string& line);
#ifdef ODYSSEUS_PLAN_PARSER_AUTO_PRINT_OPERATOR
        void addPrintOperators(nlohmann::json& operators, nlohmann::json& connections, std::unordered_map<std::string, int>& opIndex, std::unordered_set<std::string>& hasOutgoing);
#endif
    };
}

#endif //SIERRA_ODYCOMPILER_PARSER_ODYPARSER_ODYSSEUSPLANPARSER_HPP