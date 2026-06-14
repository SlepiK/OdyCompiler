#ifndef SIERRA_ODYCOMPILER_V1_CONFIG_ARGUMENT_ICONFIGARGUMENT_H
#define SIERRA_ODYCOMPILER_V1_CONFIG_ARGUMENT_ICONFIGARGUMENT_H

#include <string>
#include <typeindex>

namespace Sierra::OdyCompiler::v1::Config::Argument {
    class IConfigArgument {
    public:
        virtual ~IConfigArgument() = default;

        virtual bool isAvailable() = 0;

        virtual bool isRequired() = 0;

        virtual std::type_index getType() = 0;

        virtual std::string& getKey() = 0;
    };
} // Sierra::v1::Compiler::Config::Argument

#endif //SIERRA_ODYCOMPILER_V1_CONFIG_ARGUMENT_ICONFIGARGUMENT_H