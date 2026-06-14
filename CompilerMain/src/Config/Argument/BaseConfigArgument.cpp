#include "Config/Argument/BaseConfigArgument.h"

namespace Sierra::OdyCompiler::v1::Config::Argument {
    BaseConfigArgument::BaseConfigArgument(bool is_required, std::string key, std::type_index type)
        : m_is_required(is_required),
        m_key(std::move(key)),
        m_type(type) {
    }

    bool BaseConfigArgument::isRequired() {
        return this->m_is_required;
    }


    std::type_index BaseConfigArgument::getType() {
        return this->m_type;
    }
} // Sierra::v1::Compiler::Config