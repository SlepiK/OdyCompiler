#include <fmt/format.h>
#include <odycompiler/parser/odyparser/operators/source/read_operator_parser.h>
#include <spdlog/spdlog.h>

namespace sierra::odycompiler::parser::odyparser::operators::source::read {
    odyplan::v3::core::shared::types::OperatorTypeId ReadOperatorParser::getOperatorType() const {
        return std::to_underlying(TYPE);
    }

    odyplan::v3::core::shared::identifiers::OperatorId ReadOperatorParser::parse(
        odyplan::v3::core::logical::plan::Plan &lPlan, const nlohmann::json &config) {
        try {
            spdlog::debug("Parsing ReadOperator");

            std::string filePath;
            if (config.contains("options") && config["options"].is_array()) {
                for (const auto& opt : config["options"]) {
                    if (opt.is_array() && opt.size() >= 2) {
                        std::string key = opt[0];
                        std::string value = opt[1];
                        if (key == "filename") {
                            filePath = value;
                            break;
                        }
                    }
                }
            }

            if (filePath.empty()) {
                throw std::runtime_error("ReadOperator options must contain 'filename' field");
            }

            const std::string protocol = config.value("protocol", "JSON");
            int formatId = std::to_underlying(odyplan::v3::basic::io::BuiltinIOFormat::NDJSON);

            if (protocol == "NDJSON") {
                formatId = std::to_underlying(odyplan::v3::basic::io::BuiltinIOFormat::NDJSON);
            }
            else if (protocol == "JSON") {
                if (filePath.ends_with(".ndjson")) {
                    formatId = std::to_underlying(odyplan::v3::basic::io::BuiltinIOFormat::NDJSON);
                } else if (filePath.ends_with(".json")) {
                    formatId = std::to_underlying(odyplan::v3::basic::io::BuiltinIOFormat::JSON);
                } else {
                    formatId = std::to_underlying(odyplan::v3::basic::io::BuiltinIOFormat::JSON);
                }
            }

            formatId = config.value("format", formatId);

            const auto ioFormat = static_cast<odyplan::v3::basic::io::IOFormatType>(formatId);
            auto ioConfig = nlohmann::json::object();
            ioConfig["path"] = filePath;
            if (config.contains("source")) ioConfig["source"] = config["source"];
            if (config.contains("wrapper")) ioConfig["wrapper"] = config["wrapper"];
            if (config.contains("transport")) ioConfig["transport"] = config["transport"];
            if (config.contains("protocol")) ioConfig["protocol"] = config["protocol"];
            if (config.contains("dataHandler")) ioConfig["dataHandler"] = config["dataHandler"];

            odyplan::v3::basic::operators::source::read::details::LReadOperatorOptions readOptions;

            if (config.contains("schema") && config["schema"].is_array()) {
                for (const auto &field: config["schema"]) {
                    if (field.is_array() && field.size() >= 2) {
                        std::string fieldName = field[0];

                        if (std::string fieldType = field[1]; fieldType == "STARTTIMESTAMP") {
                            readOptions.startTimestampPosition = fieldName;
                        } else if (fieldType == "ENDTIMESTAMP") {
                            readOptions.endTimestampPosition = fieldName;
                        }
                    }
                }
            }

            if (config.contains("schema")) {
                ioConfig["schema"] = config["schema"];
            }

            readOptions.delay = std::chrono::milliseconds(config.value("delay", 0));
            readOptions.allowedLateness = std::chrono::milliseconds(config.value("allowedLateness", 0));
            readOptions.useProgramClock = config.value("useProgramClock", false);

            spdlog::info("Creating ReadOperator with format={}, path={}, startTimestamp={}, endTimestamp={}",
                         formatId,
                         filePath,
                         readOptions.startTimestampPosition.value_or("none"),
                         readOptions.endTimestampPosition.value_or("none"));

            return lPlan.addOperator(odyplan::v3::basic::operators::source::read::logical::ReadOperator::fromAutoSchema(
                ioFormat,
                ioConfig,
                readOptions
            ));
        } catch (const std::exception &e) {
            throw std::runtime_error(fmt::format("Failed to parse ReadOperator: {}", e.what()));
        }
    }
}
