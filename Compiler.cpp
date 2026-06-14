#include <spdlog/spdlog.h>

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Driver/Driver.h"
#include "clang/Driver/Compilation.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/VirtualFileSystem.h"
#include "llvm/TargetParser/Host.h"

#include "Compiler/Compiler.h"

/***
 * ToDo:
 * 1. ArgParser (not perfect)
 * 2. ArgParser Exceptions
 * 3. ArgParser rework/tuning with Exceptions
 * 4. Add more todos
 *
 */

int main(int argc, char* argv[]) {
    spdlog::info("Starting Compilation");

    Sierra::OdyCompiler::v1::Compiler::Compiler compiler;
    auto& config = compiler.getConfig();
    config.parse(argc, argv);
    spdlog::info("LLVM={}",config.getArgument<int>("llvm_version")->get().getValue()->get());


    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    auto invocation = std::make_shared<clang::CompilerInvocation>();

    clang::DiagnosticOptions diagOpts;
    auto *diagClient = new clang::TextDiagnosticPrinter(llvm::errs(), diagOpts);
    auto diagID = llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs>(new clang::DiagnosticIDs());
    clang::DiagnosticsEngine diags(diagID, diagOpts, diagClient);

    std::string targetTriple = llvm::sys::getDefaultTargetTriple();

    clang::driver::Driver driver(
        LLVM_CLANG_EXECUTABLE,  // ClangExecutable
        targetTriple,
        diags
    );

    driver.ResourceDir = LLVM_RESOURCE_DIR;

    std::vector<std::string> argsstd = {
        LLVM_CLANG_EXECUTABLE,
        "-std=c++20",
        "-O2",
        "/home/simon/CLionProjects/OdyCompiler/Executer.cpp",
        "-o", "/home/simon/CLionProjects/OdyCompiler/build/Executer",
    };

    std::vector<const char *> args;
    for (auto &s : argsstd)
        args.push_back(s.c_str());
    args.push_back("-lstdc++");
    args.push_back("-lm");

    std::unique_ptr<clang::driver::Compilation> compilation(
        driver.BuildCompilation(args)
    );

    if (!compilation) {
        spdlog::critical("Failed to build compilation");
        return 1;
    }

    llvm::SmallVector<std::pair<int, const clang::driver::Command *>, 4> failingCmds;
    int result = driver.ExecuteCompilation(*compilation, failingCmds);

    if (result != 0) {
        for (auto &[code, cmd] : failingCmds) {
            spdlog::critical("Command failed with code {}", code);
        }
        spdlog::error("Compilation failed");
        return 1;
    }

    spdlog::info("Compiled and Linked");

    return 0;
}