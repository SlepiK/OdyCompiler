#ifndef SIERRA_ODYCOMPILER_PARSER_OPERATORPARSER_HPP
#define SIERRA_ODYCOMPILER_PARSER_OPERATORPARSER_HPP
#include <memory>
#include <core/logical/operators/operator.h>
#include <nlohmann/json.hpp>

namespace sierra::odycompiler::parser {
    class IOperatorParser {
    public:
        virtual ~IOperatorParser() = default;

        virtual std::unique_ptr<odyplan::v3::core::logical::operators::IOperator>parse(const nlohmann::json& config) = 0;

        virtual odyplan::v3::core::shared::types::OperatorTypeId getOperatorType();
    };
}

#endif //SIERRA_ODYCOMPILER_PARSER_OPERATORPARSER_HPP