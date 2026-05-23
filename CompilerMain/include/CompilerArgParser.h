#ifndef SIERRA_V1_COMPILER_ARGPARSER_ARGPARSER_H
#define SIERRA_V1_COMPILER_ARGPARSER_ARGPARSER_H

#include <string>
#include <functional>
#include <unordered_map>
#include <expected>

namespace Sierra::v1::Compiler::ArgParser {
    class ArgParser {
    public:
        /*using Handler = std::function<void(std::string_view)>;

        template <typename H>
        void add_argument(std::string argument, bool is_required, H&& handler) {
            this->options.emplace(
                std::move(argument),
                Option{
                    is_required,
                    Handler(std::forward<H>(handler))
                }
            );
        }

        void parse(int argc, char* argv[]);*/
    private:
        /*struct Option {
            bool is_required;
            Handler handler;
        };

        std::unordered_map<std::string, Option> options;*/
    };
}

// Sierra::v1::Compiler::ArgParser

#endif //SIERRA_V1_COMPILER_ARGPARSER_ARGPARSER_H