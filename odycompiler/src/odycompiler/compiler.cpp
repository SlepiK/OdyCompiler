#include <odycompiler/compiler.h>

namespace sierra::odycompiler::parser {
    Compiler::Compiler(Config config) : mConfig(std::move(config)){
    }

    void Compiler::registerParser(std::unique_ptr<parser::IPlanParser> parser) {
        SPDLOG_DEBUG("Registering parser {}", parser->getName());
        mParsers.emplace(parser->getName(), std::move(parser));
    }

    bool Compiler::compileByFile(const std::string &filepath) {
        spdlog::info("=== OdyCompiler ===");
        spdlog::info("=== Mode: File ===");
        spdlog::info("Output path: {}", mConfig.outputPath);
        spdlog::info("Input path: {}", mConfig.inputPath.value());
        spdlog::info("Target: {}", mConfig.targetTriple.empty() ? "default" : mConfig.targetTriple);
        spdlog::info("Optimization: {}", mConfig.optimizationLevel);
        spdlog::info("Link Executable: {}", mConfig.lnkExecutable);

        try {
            std::unique_ptr<sierra::odyplan::v3::core::logical::plan::Plan> lPlan;
            if (!parseInput())


        } catch (std::exception& e) {
            spdlog::error("Compilation error: {}", e.what());
            return false;
        }
    }

    bool Compiler::compileByString(const std::string &input) {
        spdlog::info("=== OdyCompiler ===");
        spdlog::info("=== Mode: String ===");
        spdlog::info("Output path: {}", mConfig.outputPath);
        spdlog::info("Target: {}", mConfig.targetTriple.empty() ? "default" : mConfig.targetTriple);
        spdlog::info("Optimization: {}", mConfig.optimizationLevel);
        spdlog::info("Link Executable: {}", mConfig.lnkExecutable);

        try {

        } catch (std::exception& e) {
            spdlog::error("Compilation error: {}", e.what());
            return false;
        }
    }
}
