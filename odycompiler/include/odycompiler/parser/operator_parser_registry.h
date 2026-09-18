#ifndef SIERRA_ODYCOMPILER_PARSER_OPERATORPARSERREGISTRY_HPP
#define SIERRA_ODYCOMPILER_PARSER_OPERATORPARSERREGISTRY_HPP
#include <memory>
#include <odycompiler/parser/plan_parser.h>
#include <core/logical/operators/operator.h>
#include <odycompiler/parser/operator_parser.h>

namespace sierra::odycompiler::parser {
    class OperatorParserRegistry {
    public:
        using ParserFactory = std::function<std::unique_ptr<IOperatorParser>()>;

        static OperatorParserRegistry& instance() {
            static OperatorParserRegistry registryInstance;
            return registryInstance;
        }

        void registerParser(odyplan::v3::core::shared::types::OperatorTypeId operatorType, ParserFactory factory);

        std::unique_ptr<IOperatorParser> createParser(odyplan::v3::core::shared::types::OperatorTypeId operatorType);

        bool hasParser(odyplan::v3::core::shared::types::OperatorTypeId operatorType) const;

        std::vector<odyplan::v3::core::shared::types::OperatorTypeId> getTypesV() const;

    private:
        OperatorParserRegistry() = default;
        ~OperatorParserRegistry() = default;

        OperatorParserRegistry(const OperatorParserRegistry&) = delete;
        OperatorParserRegistry& operator=(const OperatorParserRegistry&) = delete;

        std::unordered_map<odyplan::v3::core::shared::types::OperatorTypeId, ParserFactory> mParsers;
    };
}

#endif //SIERRA_ODYCOMPILER_PARSER_OPERATORPARSERREGISTRY_HPP