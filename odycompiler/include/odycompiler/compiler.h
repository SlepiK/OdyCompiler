#ifndef SIERRA_ODYCOMPILER_COMPILER_HPP
#define SIERRA_ODYCOMPILER_COMPILER_HPP

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>

#include <odycompiler/parser/plan_parser.h>
#include <core/physical/plan/plan.h>

namespace sierra::odycompiler::parser {
    class Compiler {
    public:
        struct Config {
            std::string outputPath = "compiled_plan";
            std::string targetTriple;
            std::optional<std::string> inputPath;
            std::optional<std::string> inputString;

            std::uint8_t optimizationLevel = 3;
            bool verbose = false;

            bool lnkExecutable = false; //temporary
        };

        explicit Compiler(Config config);
        ~Compiler() = default;

        void registerParser(std::unique_ptr<parser::IPlanParser> parser);

        bool compileByString(const std::string& input);
        bool compileByFile(const std::string& filepath);

        const std::string& getLastError();

    private:
        Config mConfig;
        std::unordered_map<std::string, std::unique_ptr<parser::IPlanParser>> mParsers;

        bool parseInput(const std::string& input, std::unique_ptr<parser::IPlanParser>& parser);

        bool buildPhysicalPan(const odyplan::v3::core::logical::plan::Plan& lPlan, odyplan::v3::core::physical::plan::Plan& pPlan, llvm::LLVMContext& ctx);

        bool generateLLVM(const odyplan::v3::core::physical::plan::Plan& pPlan, std::unique_ptr<llvm::Module>& module, llvm::LLVMContext& ctx);

        bool optimizeModule(llvm::Module& module, llvm::LLVMContext& ctx);

        bool emitObjectFile(const llvm::Module& module, const std::string& outputPath, const std::string& targetTriple);
    };
}

#endif //SIERRA_ODYCOMPILER_COMPILER_HPP