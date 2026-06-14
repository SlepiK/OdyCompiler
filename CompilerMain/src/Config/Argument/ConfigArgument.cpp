#include "Config/Argument/ConfigArgument.h"

namespace Sierra::OdyCompiler::v1::Config::Argument {
    template <typename T>
    ConfigArgument<T>::ConfigArgument(T value, std::string key, const bool is_required)
        : BaseConfigArgument(is_required, std::move(key), typeid(T)),
        m_value(value) {
    }

    template <typename T>
    std::optional<T> ConfigArgument<T>::getValue() {
        return this->m_value;
    }
} // Sierra::v1::Compiler::Config