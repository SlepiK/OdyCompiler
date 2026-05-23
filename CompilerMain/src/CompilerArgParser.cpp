#include <utility>
#include "../include/CompilerArgParser.h"

#include <iostream>

namespace sierra::v1::compiler::argparser {
    /*void ArgParser::parse(int argc, char* argv[]) {
        for (int i = 1; i < argc; ++i) {
            std::string argument = argv[i];

            auto it = this->options.find(argument);

            if (it != this->options.end()) {
                std::cerr << "Unknown argument: " << argument << std::endl;
                return false;
            }

            auto& [is_required, handler] = it->second;

            if (is_required) {
                if (i + 1 >= argc) {
                    std::cerr << "Missing value for argument " << argument << std::endl;
                    return false;
                }
                handler(argv[++i]);
            } else {
                handler("");
            }
        }
    }*/
} // sierra::v1::compiler::argparser