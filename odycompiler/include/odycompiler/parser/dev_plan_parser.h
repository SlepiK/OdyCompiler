#ifndef SIERRA_ODYCOMPILER_PARSER_DEVPLANPARSER_HPP
#define SIERRA_ODYCOMPILER_PARSER_DEVPLANPARSER_HPP

#include <odycompiler/parser/plan_parser.h>
#include <basic/expression/logical/expression.h>

namespace sierra::odycompiler::parser {
    class DevPlanParser : public IPlanParser {
    public:
        DevPlanParser() = default;
        ~DevPlanParser() override = default;

        std::unique_ptr<sierra::odyplan::v3::core::logical::plan::Plan> parse(const std::string &input) override;

        std::string getName() const override {
            return "Development Plan Parser";
        }

    private:
        std::unique_ptr<sierra::odyplan::v3::core::logical::operators::IOperator>parseOperator(const nlohmann::json& op);

        std::unique_ptr<sierra::odyplan::v3::basic::expression::logical::IExpression>parseExpression(const nlohmann::json& op);
    };
}
#endif //SIERRA_ODYCOMPILER_PARSER_DEVPLANPARSER_HPP