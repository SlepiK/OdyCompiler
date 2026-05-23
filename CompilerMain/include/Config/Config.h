
#ifndef SIERRA_V1_COMPILER_CONFIG_H
#define SIERRA_V1_COMPILER_CONFIG_H
#include <fstream>
#include <string_view>
#include <nlohmann/json.hpp>
#include <vector>

namespace sierra::v1::compiler::config {
    inline constexpr std::string_view app_name = "Sierra_Compiler";
    inline constexpr std::string_view app_version = "v0.0.1";

    struct CompilerConfig {
        std::string clangExecutable;
        std::string resourceDir;

        std::string standard;
        std::string optimization;
        std::vector<std::string> extraFlags;

        std::string outputDir;

        static CompilerConfig loadFromFile(const std::string &path) {
            std::ifstream f(path);
            if (!f.is_open())
                throw std::runtime_error("Could not open file " + path);

            nlohmann::json json = nlohmann::json::parse(f);

            CompilerConfig config;
            config.clangExecutable = json.at("llvm").at("clang_executable");
            config.resourceDir = json.at("llvm").at("resource_dir");
            config.standard = json.at("compiler").at("standard");
            config.optimization = json.at("compiler").at("optimization");
            config.extraFlags = json.at("compiler").at("extra_flags").get<std::vector<std::string>>();
            config.outputDir = json.at("output").at("output_dir");

            return config;
        }

        static CompilerConfig loadAuto(const char *argv) {
            const std::filesystem::path binaryDir = std::filesystem::canonical(argv).parent_path().parent_path();
            const std::filesystem::path configPath = binaryDir / "Compiler.json";
            return loadFromFile(configPath.string());
        }
    };
} // sierra::v1::compiler::config

#endif //SIERRA_V1_COMPILER_CONFIG_H