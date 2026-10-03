#include <fmt/format.h>
#include <odycompiler/parser/testparser/operators/source/read_operator_parser.h>
#include <spdlog/spdlog.h>

namespace sierra::odycompiler::parser::testparser::operators::source::read {
    odyplan::v3::core::shared::types::OperatorTypeId ReadOperatorParser::getOperatorType() const {
        return std::to_underlying(TYPE);
    }

    odyplan::v3::core::shared::identifiers::OperatorId ReadOperatorParser::parse(odyplan::v3::core::logical::plan::Plan& lPlan, const nlohmann::json &config) {
        try {
            spdlog::debug("Parsing ReadOperator");

            if (!config.contains("params")) {
                throw std::runtime_error("config must contain 'params' field");
            }

            const auto& params = config["params"];
            if (!params.contains("path")) {
                throw std::runtime_error("ReadOperator params must contain 'path' field");
            }

            const int formatId = config.value("format", std::to_underlying(odyplan::v3::basic::io::BuiltinIOFormat::NDJSON));
            const auto ioFormat = static_cast<odyplan::v3::basic::io::IOFormatType>(formatId);

            const auto ioConfig = config.value("params", nlohmann::json::object());

            odyplan::v3::basic::operators::source::read::details::LReadOperatorOptions options;

            if (config.contains("startTimestampPosition")) {
                options.startTimestampPosition = config.value("startTimestampPosition", "");
            }

            if (config.contains("endTimestampPosition")) {
                options.endTimestampPosition = config.value("endTimestampPosition", "");
            }

            options.delay = std::chrono::milliseconds(config.value("delay", 0));
            options.allowedLateness = std::chrono::milliseconds(config.value("allowedLateness", 0));
            options.useProgramClock = config.value("useProgramClock", false);

            spdlog::info("Creating ReadOperator with format={}, path={}", formatId, params.value("path", "unknown"));

            return lPlan.addOperator(odyplan::v3::basic::operators::source::read::logical::ReadOperator::fromAutoSchema(
                ioFormat,
                ioConfig,
                options
            ));
        } catch (const std::exception &e) {
            throw std::runtime_error(fmt::format("Failed to parse ReadOperator: {}", e.what()));
        }
    }
}
