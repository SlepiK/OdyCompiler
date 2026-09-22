#ifndef SIERRA_ODYCOMPILER_PARSER_TESTPARSER_TESTOPERATORPARSERREGISTRY_HPP
#define SIERRA_ODYCOMPILER_PARSER_TESTPARSER_TESTOPERATORPARSERREGISTRY_HPP

#include <odycompiler/parser/operator_parser_registry.h>
#include <odycompiler/parser/testparser/operators/source/read_operator_parser.h>
#include <odycompiler/parser/testparser/operators/sink/write_operator_parser.h>
#include <odycompiler/parser/testparser/operators/sink/print_operator_parser.h>
#include <odycompiler/parser/testparser/operators/pipe/count_window_operator_parser.h>
#include <odycompiler/parser/testparser/operators/pipe/time_window_operator_parser.h>
#include <odycompiler/parser/testparser/operators/pipe/join_operator_parser.h>
#include <odycompiler/parser/testparser/operators/pipe/project_operator_parser.h>
#include <odycompiler/parser/testparser/operators/pipe/select_operator_parser.h>
#include <odycompiler/parser/testparser/operators/pipe/union_operator_parser.h>
#include <odycompiler/parser/testparser/operators/pipe/measure_operator_parser.h>

namespace sierra::odycompiler::parser::testparser {
    class TestOperatorParserRegistry : public OperatorParserRegistry<TestOperatorParserRegistry> {
        friend class OperatorParserRegistry<TestOperatorParserRegistry>;
    public:
    private:
        TestOperatorParserRegistry() {
            initializeParsers();
        }

        ~TestOperatorParserRegistry() override = default;

        void initializeParsers();
    };
}

#endif //SIERRA_ODYCOMPILER_PARSER_TESTPARSER_TESTOPERATORPARSERREGISTRY_HPP