#include <odycompiler/parser/odyparser/odysseus_operator_parser_registry.h>

namespace sierra::odycompiler::parser::odyparser {
    void OdysseusOperatorParserRegistry::initializeParsers() {
        spdlog::debug("Initializing TestOperatorParserRegistry");

        registerParser(std::to_underlying(operators::source::read::ReadOperatorParser::TYPE),[]() {
            return std::make_unique<operators::source::read::ReadOperatorParser>();
        });

        registerParser(std::to_underlying(operators::sink::write::WriteOperatorParser::TYPE), []() {
            return std::make_unique<operators::sink::write::WriteOperatorParser>();
        });

        registerParser(std::to_underlying(operators::sink::write::WriteOperatorParser::TYPE), []() {
            return std::make_unique<operators::sink::write::WriteOperatorParser>();
        });

        registerParser(std::to_underlying(operators::sink::print::PrintOperatorParser::TYPE), []() {
            return std::make_unique<operators::sink::print::PrintOperatorParser>();
        });

        registerParser(std::to_underlying(operators::pipe::window::CountWindowOperatorParser::TYPE), []() {
            return std::make_unique<operators::pipe::window::CountWindowOperatorParser>();
        });

        registerParser(std::to_underlying(operators::pipe::window::TimeWindowOperatorParser::TYPE), []() {
            return std::make_unique<operators::pipe::window::TimeWindowOperatorParser>();
        });

        registerParser(std::to_underlying(operators::pipe::join::JoinOperatorParser::TYPE), []() {
            return std::make_unique<operators::pipe::join::JoinOperatorParser>();
        });

        registerParser(std::to_underlying(operators::pipe::measure::MeasureOperatorParser::TYPE), []() {
            return std::make_unique<operators::pipe::measure::MeasureOperatorParser>();
        });

        registerParser(std::to_underlying(operators::pipe::project::ProjectOperatorParser::TYPE), []() {
            return std::make_unique<operators::pipe::project::ProjectOperatorParser>();
        });

        registerParser(std::to_underlying(operators::pipe::select::SelectOperatorParser::TYPE), []() {
            return std::make_unique<operators::pipe::select::SelectOperatorParser>();
        });

        registerParser(std::to_underlying(operators::pipe::un::UnionOperatorParser::TYPE), []() {
            return std::make_unique<operators::pipe::un::UnionOperatorParser>();
        });

        spdlog::debug("OdysseusOperatorParserRegistry initialized with {} operator parsers", getTypesV().size());

        spdlog::debug("Registered operator types:");
        for (const auto& type : getTypesV()) {
            spdlog::debug("  -> {}", type);
        }
    }
}
