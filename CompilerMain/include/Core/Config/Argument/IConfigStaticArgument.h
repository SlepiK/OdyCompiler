#ifndef SIERRA_ODYCOMPILER_V1_CORE_CONFIG_ARGUMENT_ICONFIGSTATICARGUMENT_H
#define SIERRA_ODYCOMPILER_V1_CORE_CONFIG_ARGUMENT_ICONFIGSTATICARGUMENT_H

#include <string>
#include <typeindex>

namespace Sierra::OdyCompiler::v1::Core::Config::Argument {
    class IConfigStaticArgument {
    public:
        virtual ~IConfigStaticArgument() = default;

        virtual bool isAvailable() = 0;

        virtual bool isRequired() = 0;

        virtual std::type_index getType() = 0;

        virtual std::string& getKey() = 0;
    };
} // Sierra::OdyCompiler::v1::Core::Config::Argument

#endif //SIERRA_ODYCOMPILER_V1_CORE_CONFIG_ARGUMENT_ICONFIGSTATICARGUMENT_H