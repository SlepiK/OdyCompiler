#include <odycompiler/parser/operators/ReadOperatorParser.h>
#include <spdlog/spdlog.h>

namespace sierra::odycompiler::parser {
    std::unique_ptr<odyplan::v3::core::logical::operators::IOperator> ReadOperatorParser::parse(const nlohmann::json &config) {
        SPDLOG_DEBUG("Parsing ReadOperator config...");

        const odyplan::v3::basic::io::IOFormatType ioFormat = config.value("format", 0);
        const auto ioConfig = config.value("params", nlohmann::json::object());
        const auto options = parseOptions(config);

        SPDLOG_DEBUG("Creating ReadOperator: format={}, path={}, timestamp_field={}", ioFormat, ioConfig.value("path", "unknown"), options.startTimestampPosition.value_or("none"));

        return sierra::odyplan::v3::basic::operators::source::read::logical::ReadOperator::fromAutoSchema(ioFormat, ioConfig, options);
    }

    odyplan::v3::basic::operators::source::read::details::LReadOperatorOptions ReadOperatorParser::parseOptions(const nlohmann::json &config) {
        odyplan::v3::basic::operators::source::read::details::LReadOperatorOptions options;

        if (config.contains("startTimestampPosition")) {
            options.startTimestampPosition = config.value("startTimestampPosition","");
            if (options.startTimestampPosition->empty()) {
                options.startTimestampPosition = std::nullopt;
            }
        }

        if (config.contains("endTimestampPosition")) {
            options.endTimestampPosition = config.value("endTimestampPosition","");
            if (options.endTimestampPosition->empty()) {
                options.endTimestampPosition = std::nullopt;
            }
        }

        const std::int64_t delay = config.value("delay",0);
        options.delay = std::chrono::milliseconds(delay);

        const std::int64_t allowedLateness = config.value("allowedLateness",0);
        options.allowedLateness = std::chrono::milliseconds(allowedLateness);

        options.useProgramClock = config.value("useProgramClock",false);

        return options;
    }
}
