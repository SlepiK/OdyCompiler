#include <odycompiler/parser/operator_parser_registry.h>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>

namespace sierra::odycompiler::parser {
    void OperatorParserRegistry::registerParser(odyplan::v3::core::shared::types::OperatorTypeId operatorType, ParserFactory factory) {
        SPDLOG_DEBUG("Registering operator parser type {}", operatorType);
        mParsers[operatorType] = std::move(factory);
    }

    std::unique_ptr<IOperatorParser> OperatorParserRegistry::createParser(const odyplan::v3::core::shared::types::OperatorTypeId operatorType) {
        const auto it = mParsers.find(operatorType);
        if (it == mParsers.end()) {
            throw std::runtime_error(fmt::format("No parser registered for: {}", operatorType));
        }
        return it->second();
    }

    bool OperatorParserRegistry::hasParser(odyplan::v3::core::shared::types::OperatorTypeId operatorType) const {
        return mParsers.contains(operatorType);
    }

    std::vector<odyplan::v3::core::shared::types::OperatorTypeId> OperatorParserRegistry::getTypesV() const {
        std::vector<odyplan::v3::core::shared::types::OperatorTypeId> registeredTypes;
        registeredTypes.reserve(mParsers.size());
        for (const auto type: mParsers | std::views::keys) {
            registeredTypes.push_back(type);
        }
        return registeredTypes;
    }
}
