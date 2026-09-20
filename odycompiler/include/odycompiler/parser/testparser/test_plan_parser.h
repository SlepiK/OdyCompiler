#ifndef SIERRA_ODYCOMPILER_PARSER_TESTPARSER_TESTPLANPARSER_HPP
#define SIERRA_ODYCOMPILER_PARSER_TESTPARSER_TESTPLANPARSER_HPP

#include <odycompiler/parser/plan_parser.h>
#include <odycompiler/parser/testparser/test_operator_parser_registry.h>
#include <core/shared/types/operator_type_id.h>

namespace sierra::odycompiler::parser::testparser {
    class TestPlanParser : public IPlanParser {
    public:
        static constexpr std::string PARSER = "TestPlanParser";

        TestPlanParser() = default;
        ~TestPlanParser() override = default;

        std::unique_ptr<sierra::odyplan::v3::core::logical::plan::Plan> parse(const std::string &input) override;

        const std::string& getName() const override;

    private:
        TestOperatorParserRegistry& mRegistry = TestOperatorParserRegistry::instance();

    };
}

#endif //SIERRA_ODYCOMPILER_PARSER_TESTPARSER_TESTPLANPARSER_HPP