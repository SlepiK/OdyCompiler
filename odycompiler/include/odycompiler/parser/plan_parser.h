#ifndef SIERRA_ODYCOMPILER_PARSER_PLANPARSER_HPP
#define SIERRA_ODYCOMPILER_PARSER_PLANPARSER_HPP

#include <nlohmann/json.hpp>
#include <vector>
#include <memory>
#include <core/logical/plan/plan.h>

namespace sierra::odycompiler::parser {
    class IPlanParser {
    public:
        virtual ~IPlanParser() = default;

        virtual std::unique_ptr<sierra::odyplan::v3::core::logical::plan::Plan> parse(const std::string& input) = 0;

        virtual std::string getName() const = 0;
    };
}

#endif //SIERRA_ODYCOMPILER_PARSER_PLANPARSER_HPP