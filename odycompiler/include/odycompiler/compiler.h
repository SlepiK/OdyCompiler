#ifndef SIERRA_ODYCOMPILER_COMPILER_HPP
#define SIERRA_ODYCOMPILER_COMPILER_HPP

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>

#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_os_ostream.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/OptimizationLevel.h>

#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/TargetParser/Triple.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/CodeGen.h>

#include <odycompiler/parser/plan_parser.h>
#include <core/physical/plan/plan.h>
#include <core/builder/plan_builder.h>
#include <basic/operators/operator_transformations.h>

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

            std::string parser;
        };

        explicit Compiler(Config config);
        ~Compiler() = default;

        void registerParser(std::unique_ptr<parser::IPlanParser> parser);

        void compileByString(const std::string& input);
        void compileByFile(const std::string& filepath);

    private:
        Config mConfig;
        std::unordered_map<std::string, std::unique_ptr<parser::IPlanParser>> mParsers;

        std::unique_ptr<odyplan::v3::core::logical::plan::Plan> buildLogicalPlan(const std::string& input);

        std::unique_ptr<odyplan::v3::core::physical::plan::Plan> buildPhysicalPan(std::unique_ptr<odyplan::v3::core::logical::plan::Plan> lPlan, llvm::LLVMContext& ctx);

        std::unique_ptr<llvm::Module> generateLLVM(std::unique_ptr<odyplan::v3::core::physical::plan::Plan> pPlan, llvm::LLVMContext& ctx);

        void optimizeModule(std::unique_ptr<llvm::Module>& module, llvm::LLVMContext& ctx);

        void emitObjectFile(std::unique_ptr<llvm::Module> module, const std::string& outputPath, const std::string& targetTriple);
    };
}

#endif //SIERRA_ODYCOMPILER_COMPILER_HPP