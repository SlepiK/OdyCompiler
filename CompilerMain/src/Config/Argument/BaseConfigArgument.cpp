#include "Config/Argument/BaseConfigArgument.h"

namespace Sierra::OdyCompiler::v1::Config::Argument {
    template <typename T>
    BaseConfigArgument<T>::BaseConfigArgument(std::string key, T value, bool is_required)
        : m_is_required(is_required),
        m_key(std::move(key)),
        m_type(typeid(T)),
        m_value(value){
    }

    template <typename T>
    bool BaseConfigArgument<T>::isRequired() {
        return this->m_is_required;
    }

    template <typename T>
    std::type_index BaseConfigArgument<T>::getType() {
        return this->m_type;
    }

    template <typename T>
    std::string& BaseConfigArgument<T>::getKey() {
        return this->m_key;
    }

    template <typename T>
    std::expected<std::reference_wrapper<T>, std::string> BaseConfigArgument<T>::getValue() {
        if (!this->m_value.has_value())
            return std::unexpected("Argument \"" +this->m_key +"\" has no value");
        return std::ref(*this->m_value);
    }
} // Sierra::v1::Compiler::Config