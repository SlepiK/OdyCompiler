#ifndef SIERRA_ODYCOMPILER_PARSER_ODYPARSER_ODYSSEUSOPERATORPARSERREGISTRY_HPP
#define SIERRA_ODYCOMPILER_PARSER_ODYPARSER_ODYSSEUSOPERATORPARSERREGISTRY_HPP

#include <odycompiler/parser/operator_parser_registry.h>
#include <odycompiler/parser/odyparser/operators/source/read_operator_parser.h>
#include <odycompiler/parser/odyparser/operators/sink/write_operator_parser.h>
#include <odycompiler/parser/odyparser/operators/sink/print_operator_parser.h>
#include <odycompiler/parser/odyparser/operators/pipe/count_window_operator_parser.h>
#include <odycompiler/parser/odyparser/operators/pipe/time_window_operator_parser.h>
#include <odycompiler/parser/odyparser/operators/pipe/join_operator_parser.h>
#include <odycompiler/parser/odyparser/operators/pipe/project_operator_parser.h>
#include <odycompiler/parser/odyparser/operators/pipe/select_operator_parser.h>
#include <odycompiler/parser/odyparser/operators/pipe/union_operator_parser.h>
#include <odycompiler/parser/odyparser/operators/pipe/measure_operator_parser.h>

namespace sierra::odycompiler::parser::odyparser {
    class OdysseusOperatorParserRegistry : public OperatorParserRegistry<OdysseusOperatorParserRegistry> {
        friend class OperatorParserRegistry<OdysseusOperatorParserRegistry>;
    public:
    private:
        OdysseusOperatorParserRegistry() {
            initializeParsers();
        }

        ~OdysseusOperatorParserRegistry() override = default;

        void initializeParsers();
    };
}

#endif //SIERRA_ODYCOMPILER_PARSER_ODYPARSER_ODYSSEUSOPERATORPARSERREGISTRY_HPP