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
#include <llvm/IR/Verifier.h>

#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/TargetParser/Triple.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/CodeGen.h>
#include <llvm/IR/Module.h>

#include <odycompiler/parser/plan_parser.h>
#include <core/physical/plan/plan.h>
#include <core/builder/plan_builder.h>
#include <basic/operators/operator_transformations.h>

namespace sierra::odycompiler {
    class Compiler {
    public:
        struct Config {
            std::string outputPath = "compiled_plan";
            std::string targetTriple;
            std::optional<std::string> inputPath;
            std::optional<std::string> inputString;

            std::uint8_t optimizationLevel = 3;

            bool lnkExecutable = false;
            std::string linkerPath;
            bool dryrun = false;

            std::string parser;

            std::string odyPlanLib;
            std::string llvmLib;
        };

        explicit Compiler(Config config);
        ~Compiler() = default;

        void registerParser(std::unique_ptr<parser::IPlanParser> parser);

        void compileByString(const std::string& input);
        void compileByFile(const std::string& filepath);

    private:
        Config mConfig;
        std::unordered_map<std::string, std::unique_ptr<parser::IPlanParser>> mParsers;

        odyplan::v3::core::physical::types::CodegenTypeRegistry mCodegenTypeRegistry;

        std::unique_ptr<odyplan::v3::core::logical::plan::Plan> buildLogicalPlan(const std::string& input);

        std::unique_ptr<odyplan::v3::core::physical::plan::Plan> buildPhysicalPan(std::unique_ptr<odyplan::v3::core::logical::plan::Plan> lPlan, std::unique_ptr<llvm::LLVMContext>& ctx);

        std::unique_ptr<llvm::Module> generateLLVM(std::unique_ptr<odyplan::v3::core::physical::plan::Plan> pPlan, std::unique_ptr<llvm::LLVMContext>& ctx);

        void optimizeModule(std::unique_ptr<llvm::Module>& module, std::unique_ptr<llvm::LLVMContext>& ctx);

        void emitObjectFile(std::unique_ptr<llvm::Module> module, const std::string& outputPath, const std::string& targetTriple);

        void linkExecutable(const std::string& objectFile);

        std::string getLinkCommand(const std::string& objectFile, const std::string& outputPath = "") const;

        void compile(const std::string& input);

        void loadBuiltinParsers();

        void generateMainWrapper(std::unique_ptr<llvm::Module>& module, std::unique_ptr<llvm::LLVMContext>& ctx);
    };
}

#endif //SIERRA_ODYCOMPILER_COMPILER_HPP