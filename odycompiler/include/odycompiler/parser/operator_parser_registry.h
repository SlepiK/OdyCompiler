#ifndef SIERRA_ODYCOMPILER_PARSER_OPERATORPARSERREGISTRY_HPP
#define SIERRA_ODYCOMPILER_PARSER_OPERATORPARSERREGISTRY_HPP
#include <memory>
#include <odycompiler/parser/plan_parser.h>
#include <core/logical/operators/operator.h>
#include <odycompiler/parser/operator_parser.h>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>

namespace sierra::odycompiler::parser {
    template <typename T>
    class OperatorParserRegistry {
    public:
        using ParserFactory = std::function<std::unique_ptr<IOperatorParser>()>;

        static T& instance() {
            static T registryInstance;
            return registryInstance;
        }

        virtual ~OperatorParserRegistry() = default;

        virtual void registerParser(odyplan::v3::core::shared::types::OperatorTypeId operatorType, ParserFactory factory) {
            SPDLOG_DEBUG("Registering operator parser type {}", operatorType);
            mParsers[operatorType] = std::move(factory);
        }

        virtual std::unique_ptr<IOperatorParser> createParser(odyplan::v3::core::shared::types::OperatorTypeId operatorType) {
            const auto it = mParsers.find(operatorType);
            if (it == mParsers.end()) {
                throw std::runtime_error(fmt::format("No parser registered for: {}", operatorType));
            }
            return it->second();
        }

        virtual bool hasParser(odyplan::v3::core::shared::types::OperatorTypeId operatorType) const {
            return mParsers.contains(operatorType);
        }

        virtual std::vector<odyplan::v3::core::shared::types::OperatorTypeId> getTypesV() const {
            std::vector<odyplan::v3::core::shared::types::OperatorTypeId> registeredTypes;
            registeredTypes.reserve(mParsers.size());
            for (const auto type: mParsers | std::views::keys) {
                registeredTypes.push_back(type);
            }
            return registeredTypes;
        }

    protected:
        OperatorParserRegistry() = default;

        OperatorParserRegistry(const OperatorParserRegistry&) = delete;
        OperatorParserRegistry& operator=(const OperatorParserRegistry&) = delete;

        std::unordered_map<odyplan::v3::core::shared::types::OperatorTypeId, ParserFactory> mParsers;
    };
}

#endif //SIERRA_ODYCOMPILER_PARSER_OPERATORPARSERREGISTRY_HPP