#include <fstream>
#include <odycompiler/compiler.h>

namespace sierra::odycompiler::parser {
    Compiler::Compiler(Config config) : mConfig(std::move(config)){
    }

    void Compiler::registerParser(std::unique_ptr<parser::IPlanParser> parser) {
        spdlog::debug("Registering parser {}", parser->getName());
        mParsers.emplace(parser->getName(), std::move(parser));
    }

    void Compiler::compileByFile(const std::string &filepath) {
        spdlog::info("=== OdyCompiler ===");
        spdlog::info("=== Mode: File ===");
        spdlog::info("Output path: {}", mConfig.outputPath);
        spdlog::info("Input path: {}", mConfig.inputPath.value());
        spdlog::info("Target: {}", mConfig.targetTriple.empty() ? "default" : mConfig.targetTriple);
        spdlog::info("Optimization: {}", mConfig.optimizationLevel);
        spdlog::info("Link Executable: {}", mConfig.lnkExecutable);

        if (!std::filesystem::exists(filepath)) {
            throw std::runtime_error(fmt::format("Input file does not exist: {}", filepath));
        }

        std::ifstream file(filepath, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error(fmt::format("Cannot open file: {}", filepath));
        }

        const std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        compileByString(content);
    }

    void Compiler::compileByString(const std::string &input) {
        spdlog::info("=== OdyCompiler ===");
        spdlog::info("=== Mode: String ===");
        spdlog::info("Output path: {}", mConfig.outputPath);
        spdlog::info("Target: {}", mConfig.targetTriple.empty() ? "default" : mConfig.targetTriple);
        spdlog::info("Optimization: {}", mConfig.optimizationLevel);
        spdlog::info("Link Executable: {}", mConfig.lnkExecutable);

        try {
            auto lPlan = buildLogicalPlan(input);

            llvm::LLVMContext ctx;

            auto pPlan = buildPhysicalPan(std::move(lPlan), ctx);

            auto module = generateLLVM(std::move(pPlan), ctx);

            optimizeModule(module, ctx);

            emitObjectFile(std::move(module), mConfig.outputPath, mConfig.targetTriple);

            spdlog::info("=== Compilation Successful ===");
            spdlog::info("Object file: {}", mConfig.outputPath);
        } catch (std::exception& e) {
            throw std::runtime_error(fmt::format("Compilation error: {}", e.what()));
        }
    }

    std::unique_ptr<odyplan::v3::core::logical::plan::Plan> Compiler::buildLogicalPlan(const std::string &input) {
        spdlog::info("Building logical plan with '{}' parser...", mConfig.parser);

        const auto it = mParsers.find(mConfig.parser);
        if (it == mParsers.end()) {
            throw std::runtime_error(fmt::format("Unknown parser: {}", mConfig.parser));
        }

        auto lPlan = it->second->parse(input);

        spdlog::info("Logical plan built successfully");
        return lPlan;
    }

    std::unique_ptr<odyplan::v3::core::physical::plan::Plan> Compiler::buildPhysicalPan(std::unique_ptr<odyplan::v3::core::logical::plan::Plan> lPlan, llvm::LLVMContext &ctx) {
        spdlog::info("Building physical plan...");

        const auto codegenTypeRegistry = odyplan::v3::core::physical::types::CodegenTypeRegistry::withBuiltins();

        odyplan::v3::core::builder::PlanBuilder builder(codegenTypeRegistry);

        odyplan::v3::basic::operators::loadBuiltinOperatorTransformations(builder);

        auto pPlan = builder.build(*lPlan, ctx);

        spdlog::info("Physical plan built successfully");

        return std::make_unique<odyplan::v3::core::physical::plan::Plan>(std::move(pPlan));
    }

    std::unique_ptr<llvm::Module> Compiler::generateLLVM(std::unique_ptr<odyplan::v3::core::physical::plan::Plan> pPlan, llvm::LLVMContext &ctx) {
        spdlog::info("Generating LLVM IR...");

        auto module = std::make_unique<llvm::Module>("compiled_odyplan_plan",ctx);
        pPlan->codegen(*module, ctx);

        spdlog::info("LLVM IR generated");

        return module;
    }

    void Compiler::optimizeModule(std::unique_ptr<llvm::Module> &module, llvm::LLVMContext &ctx) {
        spdlog::info("Optimizing module (level {})...", mConfig.optimizationLevel);

        llvm::LoopAnalysisManager loopAnaMgr;
        llvm::FunctionAnalysisManager fnAnaMgr;
        llvm::ModuleAnalysisManager modAnaMgr;
        llvm::CGSCCAnalysisManager cgsccAnaMgr;

        llvm::PassBuilder passBuilder;

        passBuilder.registerLoopAnalyses(loopAnaMgr);
        passBuilder.registerFunctionAnalyses(fnAnaMgr);
        passBuilder.registerModuleAnalyses(modAnaMgr);
        passBuilder.registerCGSCCAnalyses(cgsccAnaMgr);
        passBuilder.crossRegisterProxies(loopAnaMgr, fnAnaMgr, cgsccAnaMgr, modAnaMgr);

        llvm::OptimizationLevel level;
        switch (mConfig.optimizationLevel) {
            case 0:
                level = llvm::OptimizationLevel::O0;
                break;
            case 1:
                level = llvm::OptimizationLevel::O1;
                break;
            case 2:
                level = llvm::OptimizationLevel::O2;
                break;
            case 3:
                level = llvm::OptimizationLevel::O3;
                break;
            default:
                level = llvm::OptimizationLevel::O2;
        }

        llvm::ModulePassManager modPassMgr = passBuilder.buildPerModuleDefaultPipeline(level);
        modPassMgr.run(*module, modAnaMgr);

        spdlog::info("Optimization complete");
    }

    void Compiler::emitObjectFile(std::unique_ptr<llvm::Module> module, const std::string &outputPath, const std::string &targetTriple) {
        spdlog::info("Emitting object file...");

        llvm::InitializeAllTargetInfos();
        llvm::InitializeAllTargets();
        llvm::InitializeAllTargetMCs();
        llvm::InitializeAllAsmPrinters();
        llvm::InitializeAllAsmParsers();

        const std::string triple = targetTriple.empty() ? llvm::sys::getDefaultTargetTriple() : targetTriple;

        spdlog::info("Target triple: {}", triple);

        const llvm::Triple targetTripleObj(triple);

        std::string lookupErr;
        const auto* target = llvm::TargetRegistry::lookupTarget(targetTripleObj, lookupErr);
        if (!target) {
            throw std::runtime_error(fmt::format("Failed to lookup target: {}", lookupErr));
        }

        const llvm::TargetOptions targetOptions;
        const std::unique_ptr<llvm::TargetMachine> targetMachine(target->createTargetMachine(targetTripleObj, "gneric", "", targetOptions, llvm::Reloc::PIC_));

        if (!targetMachine) {
            throw std::runtime_error(fmt::format("Failed to create target machine"));
        }

        module->setDataLayout(targetMachine->createDataLayout());

        std::error_code ec;
        llvm::raw_fd_ostream dest(outputPath, ec, llvm::sys::fs::OF_None);
        if (ec) {
            throw std::runtime_error(fmt::format("Could not open output file: {}", ec.message()));
        }

        llvm::legacy::PassManager pass;
        if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, llvm::CodeGenFileType::ObjectFile)) {
            throw std::runtime_error(fmt::format("TargetMachine cannot emit object file for this target"));
        }

        pass.run(*module);
        dest.flush();

        spdlog::info("Object file written: {}", outputPath);
    }
}
