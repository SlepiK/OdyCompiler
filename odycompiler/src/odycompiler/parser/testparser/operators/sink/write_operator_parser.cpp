#include <fmt/format.h>
#include <odycompiler/parser/testparser/operators/sink/write_operator_parser.h>
#include <spdlog/spdlog.h>

namespace sierra::odycompiler::parser::testparser::operators::sink::write {
    odyplan::v3::core::shared::types::OperatorTypeId WriteOperatorParser::getOperatorType() const {
        return std::to_underlying(TYPE);
    }

    std::unique_ptr<odyplan::v3::core::logical::operators::IOperator> WriteOperatorParser::parse(const nlohmann::json &config) {
        try {
            SPDLOG_DEBUG("Parsing WriteOperator");

            if (!config.contains("params")) {
                throw std::runtime_error("config must contain 'params' field");
            }

            const auto& params = config["params"];
            if (!params.contains("path")) {
                throw std::runtime_error("params must contain 'path' field");
            }

            const int formatId = config.value("format", std::to_underlying(odyplan::v3::basic::io::BuiltinIOFormat::NDJSON));
            const auto ioFormat = static_cast<odyplan::v3::basic::io::IOFormatType>(formatId);

            const auto ioConfig = config.value("params", nlohmann::json::object());

            SPDLOG_INFO("Creating with format={}, path={}", formatId, params.value("path", "unknown"));

            return std::make_unique<odyplan::v3::basic::operators::sink::write::logical::WriteOperator>(ioFormat,ioConfig);
        } catch (const std::exception &e) {
            throw std::runtime_error(fmt::format("Failed to parse WriteOperator: {}", e.what()));
        }
    }
}
