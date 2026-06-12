#include <spdlog/spdlog.h>
#include "Config/Config.h"

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Driver/Driver.h"
#include "clang/Driver/Compilation.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/VirtualFileSystem.h"
#include "llvm/TargetParser/Host.h"

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

    auto config = sierra::v1::compiler::config::CompilerConfig::loadAuto(argv[0]);

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
        config.clangExecutable,  // ClangExecutable
        targetTriple,
        diags
    );

    driver.ResourceDir = config.resourceDir;

    std::string stdFlag  = "-std=" + config.standard;
    std::string optFlag  = "-" + config.optimization;

    std::vector<std::string> argsstd = {
        config.clangExecutable,
        stdFlag,
        optFlag,
        "/home/simon/CLionProjects/odysseus_sierra_cpp/Executer.cpp",
        "-o", "/home/simon/CLionProjects/odysseus_sierra_cpp/build/Executer",
    };

    for (auto &f : config.extraFlags)
        argsstd.push_back(f);

    std::vector<const char *> args;
    for (auto &s : argsstd)
        args.push_back(s.c_str());

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