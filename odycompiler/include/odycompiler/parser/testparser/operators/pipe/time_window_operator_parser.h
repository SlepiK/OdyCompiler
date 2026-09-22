#ifndef SIERRA_ODYCOMPILER_PARSER_TESTPARSER_OPERATORS_TIMEWINDOWOPERATORPARSER_HPP
#define SIERRA_ODYCOMPILER_PARSER_TESTPARSER_OPERATORS_TIMEWINDOWOPERATORPARSER_HPP

#include <odycompiler/parser/operator_parser.h>
#include <core/shared/types/operator_type_id.h>
#include <basic/operators/pipe/window/logical/time_window_operator.h>

namespace sierra::odycompiler::parser::testparser::operators::pipe::window {
    class TimeWindowOperatorParser : public IOperatorParser {
    public:
        static constexpr auto TYPE = odyplan::v3::core::shared::types::BuiltinOperatorType::WINDOW_TIME;

        odyplan::v3::core::shared::identifiers::OperatorId parse(odyplan::v3::core::logical::plan::Plan& lPlan, const nlohmann::json &config) override;

       [[nodiscard]] odyplan::v3::core::shared::types::OperatorTypeId getOperatorType() const override;
    };
}

#endif //SIERRA_ODYCOMPILER_PARSER_TESTPARSER_OPERATORS_TIMEWINDOWOPERATORPARSER_HPP