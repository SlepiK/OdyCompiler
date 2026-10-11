#include <fstream>
#include <odycompiler/compiler.h>

#include <odycompiler/parser/testparser/test_plan_parser.h>
#include <odycompiler/parser/odyparser/odysseus_plan_parser.h>

namespace sierra::odycompiler {
    Compiler::Compiler(Config config) : mConfig(std::move(config)){
        loadBuiltinParsers();
    }

    void Compiler::registerParser(std::unique_ptr<parser::IPlanParser> parser) {
        spdlog::debug("Registering parser {}", parser->getName());
        mParsers.emplace(parser->getName(), std::move(parser));
    }

    void Compiler::loadBuiltinParsers() {
        registerParser(std::make_unique<parser::testparser::TestPlanParser>());
        registerParser(std::make_unique<parser::odyparser::OdysseusPlanParser>());
    }

    void Compiler::compileByFile(const std::string &filepath) {
        if (mConfig.dryrun) {
            spdlog::warn("=== DRY-RUN ===");
        }
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

        compile(content);
    }

    void Compiler::compile(const std::string& input) {
        try {
            auto lPlan = buildLogicalPlan(input);

            auto ctx = std::make_unique<llvm::LLVMContext>();

            auto pPlan = buildPhysicalPan(std::move(lPlan), ctx);

            auto module = generateLLVM(std::move(pPlan), ctx);

            optimizeModule(module, ctx);


            std::filesystem::path objectFile = mConfig.outputPath;
            if (objectFile.extension() != ".o") {
                objectFile += ".o";
            }

            emitObjectFile(std::move(module), objectFile.c_str(), mConfig.targetTriple);

            if (mConfig.lnkExecutable) {
                linkExecutable(objectFile.c_str());
            } else {
                spdlog::info("=== Compilation Successful ===");
                spdlog::info("Object file: {}", objectFile.c_str());
                spdlog::info("=== Linking Instructions ===");
                spdlog::info("To create an executable, run:");
                spdlog::info(fmt::format("{}", getLinkCommand(objectFile.c_str())));
            }
        } catch (std::exception& e) {
            throw std::runtime_error(fmt::format("Compilation error: {}", e.what()));
        }
    }

    void Compiler::compileByString(const std::string &input) {
        if (mConfig.dryrun) {
            spdlog::warn("=== DRY-RUN ===");
        }
        spdlog::info("=== Mode: String ===");
        spdlog::info("Output path: {}", mConfig.outputPath);
        spdlog::info("Target: {}", mConfig.targetTriple.empty() ? "default" : mConfig.targetTriple);
        spdlog::info("Optimization: {}", mConfig.optimizationLevel);
        spdlog::info("Link Executable: {}", mConfig.lnkExecutable);

        compile(input);
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

    std::unique_ptr<odyplan::v3::core::physical::plan::Plan> Compiler::buildPhysicalPan(std::unique_ptr<odyplan::v3::core::logical::plan::Plan> lPlan, std::unique_ptr<llvm::LLVMContext>& ctx) {
        spdlog::info("Building physical plan...");

        odyplan::v3::core::builder::PlanBuilder builder{};

        odyplan::v3::basic::operators::loadBuiltinOperatorTransformations(builder);

        auto pPlan = builder.build(*lPlan, *ctx);

        spdlog::info("Physical plan built successfully");

        return std::make_unique<odyplan::v3::core::physical::plan::Plan>(std::move(pPlan));
    }

    std::unique_ptr<llvm::Module> Compiler::generateLLVM(std::unique_ptr<odyplan::v3::core::physical::plan::Plan> pPlan, std::unique_ptr<llvm::LLVMContext>& ctx) {
        spdlog::info("Generating LLVM IR...");

        auto module = std::make_unique<llvm::Module>("compiled_odyplan_plan",*ctx);

        pPlan->codegen(*module, *ctx);

        if (mConfig.lnkExecutable) {
            generateMainWrapper(module, ctx);
        }

        spdlog::info("LLVM IR generated");

        return module;
    }


    void Compiler::generateMainWrapper(std::unique_ptr<llvm::Module>& module, std::unique_ptr<llvm::LLVMContext>& ctx) {
        spdlog::info("Generating mainFn...");
        auto* voidTy = llvm::Type::getVoidTy(*ctx);
        auto* ptrTy = llvm::PointerType::getUnqual(*ctx);

        auto* mainType = llvm::FunctionType::get(llvm::Type::getInt32Ty(*ctx), {}, false);

        auto* mainFn = llvm::Function::Create(mainType, llvm::Function::ExternalLinkage, "main", *module);

        auto* entryBB = llvm::BasicBlock::Create(*ctx, "entry", mainFn);
        llvm::IRBuilder<> builder(entryBB);

        auto* initFn = module->getFunction("ody_init");

        if (initFn != nullptr) {
                builder.CreateCall(initFn);
        } else {
            throw std::runtime_error("ody_init() not found in module");
        }

        auto* odyMainFn = module->getFunction("ody_main");
        if (odyMainFn != nullptr) {
            builder.CreateCall(odyMainFn);
        } else {
            throw std::runtime_error("ody_main() not found in module");
        }

        auto* shutdownFn = module->getFunction("ody_shutdown");
        if (shutdownFn != nullptr) {
            builder.CreateCall(shutdownFn, {});
        } else {
            throw std::runtime_error("ody_shutdown() not found in module!");
        }

        builder.CreateRet(llvm::ConstantInt::get(llvm::Type::getInt32Ty(*ctx), 0));

        spdlog::info("mainFn generated");
    }

    void Compiler::optimizeModule(std::unique_ptr<llvm::Module> &module, std::unique_ptr<llvm::LLVMContext>& ctx) {
        spdlog::info("Optimizing module (level {})...", mConfig.optimizationLevel);

        std::string errorMsg;
        llvm::raw_string_ostream os(errorMsg);
        if (llvm::verifyModule(*module, &os)) {
            spdlog::error("LLVM Module verification failed:\n{}", errorMsg);
            throw std::runtime_error("Invalid LLVM IR generated");
        }

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
        const std::unique_ptr<llvm::TargetMachine> targetMachine(target->createTargetMachine(targetTripleObj, "generic", "", targetOptions, llvm::Reloc::PIC_));

        if (!targetMachine) {
            throw std::runtime_error(fmt::format("Failed to create target machine"));
        }

        module->setDataLayout(targetMachine->createDataLayout());

        if (!mConfig.dryrun) {
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
        }

        spdlog::info("Object file written: {}", outputPath);
    }

    void Compiler::linkExecutable(const std::string& objectFile) {
        spdlog::info("Linking executable...");

        std::filesystem::path outputPath = mConfig.outputPath;
        if (outputPath.extension() == ".o") {
            outputPath = outputPath.stem();
        }

        #ifdef _WIN32
        outputPath += ".exe";
        #endif

        spdlog::info("Output executable: {}", outputPath.c_str());

        std::string linkCmd = getLinkCommand(objectFile, outputPath.c_str());

        spdlog::debug("Link command: {}", linkCmd);

        if (!mConfig.dryrun) {
            if (int result = std::system(linkCmd.c_str()); result != 0) {
                throw std::runtime_error(fmt::format("Linker failed with exit code {}", result));
            }
        }

        spdlog::info("=== Compilation & Linking Successful ===");
        spdlog::info("Executable: {}", outputPath.c_str());
    }


    std::string Compiler::getLinkCommand(const std::string& objectFile, const std::string& outputPath) const {
        std::string lnkCmd;

        std::string linker = mConfig.linkerPath;
        if (linker.empty()) {
            #ifdef _WIN32
            linker = "link";
            #else
            linker = "c++";
            #endif
        }

        std::string output = outputPath;
        if (output.empty()) {
            output = objectFile;
            if (output.size() > 3 && output.substr(output.size() - 3) == ".o") {
                output = output.substr(0, output.size() - 3);
            }
            #ifdef _WIN32
            output += ".exe";
            #endif
        }

        std::string libPathFlag;
        if (!mConfig.odyPlanLib.empty()) {
            #ifdef _WIN32
            libPathFlag = fmt::format("/LIBPATH:\"{}\"", mConfig.odyPlanLib);
            #else
            libPathFlag = fmt::format("-L\"{}\"", mConfig.odyPlanLib);
            #endif
        }

        std::string llvmLibFlag;
        if (!mConfig.llvmLib.empty()) {
            #ifdef _WIN32
            llvmLibFlag = fmt::format("/LIBPATH:\"{}\"", mConfig.llvmLib);
            #else
            llvmLibFlag = fmt::format("-L\"{}\"", mConfig.llvmLib);
            #endif
        }

        std::string llvmLibs =
        "-lLLVMSupport "
        "-lLLVMCore "
        "-lLLVMBitReader "
        "-lLLVMX86CodeGen "
        "-lLLVMX86Desc "
        "-lLLVMX86Info "
        "-lLLVMAsmPrinter "
        "-lLLVMCodeGen "
        "-lLLVMTarget";

        #ifdef _WIN32
        lnkCmd = fmt::format(
            "{} /OUT:{} {} {} {}"
            "odyplan_core.lib odyplan_basic.lib "
            "{} "
            "kernel32.lib user32.lib advapi32.lib",
            linker, output, objectFile, libPathFlag, llvmLibFlag, llvmLibs
        );
        #elif defined(__APPLE__)
        lnkCmd = fmt::format(
            "{} -o {} {} {} {} "
            "-lodyplan_core -lodyplan_basic "
            "{} "
            "-lpthread -ldl",
            linker, output, objectFile, libPathFlag, llvmLibFlag, llvmLibs
        );
        #else
        lnkCmd = fmt::format(
            "{} -o {} {} {} {} "
            "-Wl,--start-group "
            "-lodyplan_core -lodyplan_basic "
            "{} "
            "-Wl,--end-group "
            "-lpthread -ldl",
            linker, output, objectFile, libPathFlag, llvmLibFlag, llvmLibs
        );
        #endif

        return lnkCmd;
    }
}
