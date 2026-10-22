#include <fmt/format.h>
#include <odycompiler/parser/odyparser/operators/pipe/count_window_operator_parser.h>
#include <spdlog/spdlog.h>

namespace sierra::odycompiler::parser::odyparser::operators::pipe::window {
    odyplan::v3::core::shared::types::OperatorTypeId CountWindowOperatorParser::getOperatorType() const {
        return std::to_underlying(TYPE);
    }

    odyplan::v3::core::shared::identifiers::OperatorId CountWindowOperatorParser::parse(odyplan::v3::core::logical::plan::Plan& lPlan, const nlohmann::json &config) {
        try {
            spdlog::debug("Parsing CountWindowOperator");

            odyplan::v3::basic::operators::pipe::window::details::CountWindowOptions countOptions;

            const bool assureOrder = config.value("assure_order", true);

            countOptions.size = config.value<std::uint64_t>("size", 1);
            countOptions.slide = config.value<std::uint64_t>("slide", 1);
            countOptions.advance = config.value<std::uint64_t>("advance", 1);

            spdlog::info("Creating CountWindowOperator with size={}, slide={}, advance={}", countOptions.size, countOptions.slide, countOptions.advance);

            /*
             * ToDo:Check why isOutOfOrder and assureOrder is in Odysseus something weird implemented... . Due to the fact,
             * that this operator needs currently to run always a puffer for watermarks, because we always have watermarks, its directly set to false
             */
            return lPlan.addOperator(std::make_unique<odyplan::v3::basic::operators::pipe::window::logical::CountWindowOperator>(countOptions,/*assureOrder*/ false));
        } catch (const std::exception &e) {
            throw std::runtime_error(fmt::format("Failed to parse CountWindowOperator: {}", e.what()));
        }
    }
}
