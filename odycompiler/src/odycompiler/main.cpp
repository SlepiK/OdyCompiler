#include <filesystem>
#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <odycompiler/compiler.h>
#include <odycompiler/timer.h>

void printUsage(const char* prg) {
    spdlog::info(fmt::format("Usage {} [OPTIONS] <input>", prg));
    spdlog::info("Input (choose on):");
    spdlog::info(" -f/--file <file>");
    spdlog::info(" -s/--string <string>");
    spdlog::info("Output:");
    spdlog::info(" -op/--output <path>");
    spdlog::info(" -l/--link");
    spdlog::info("Parser:");
    spdlog::info(" -p/--parser <name>   Parser type: test, ody (default: test)");
    spdlog::info("Target:");
    spdlog::info(" -t/--target <triple> Target triple (default: system default)");
    spdlog::info("Optimization:");
    spdlog::info(" -o/--optimize <level>    Optimization level 0-3 (default: 3)");
    spdlog::info("Libraries:");
    spdlog::info(" -opl/--odyplan_lib <path>    Path to lib-folder of OdyPlan");
    spdlog::info(" -lvl/--llvm_lib <path>    Path to lib-folder of llvm");
    spdlog::info("Other:");
    spdlog::info(" -v/--verbose Verbose output");
    spdlog::info(" -h/--help    Show this help message");
    spdlog::info(" --version    Show version information");
    spdlog::info(" --dry    Executes a dry-run");
    spdlog::info("Examples:");
    spdlog::info(fmt::format(" {} plan.test.json    Creates plan.o (object file)", prg));
    spdlog::info(fmt::format(" {} plan.test.json -l    Creates plan executable (links automatically)", prg));
    spdlog::info(fmt::format(" {} plan.test.json -l -o my_plan    Creates my_plan executable", prg));
    spdlog::info(fmt::format(" {} plan.test.json -l --linker=/usr/bin/ld    Uses custom linker", prg));
}

void printVersion() {
    spdlog::info("OdyCompiler");
    spdlog::info(fmt::format(" Version: {}", "v0.1.0"));
    spdlog::info(fmt::format(" Git: {}", "https://github.com/SlepiK/OdyCompiler"));
    spdlog::info(fmt::format(" LLVM: {}", LLVM_VERSION_STRING));
    spdlog::info(fmt::format(" Target: {}", llvm::sys::getDefaultTargetTriple()));
}

struct CommandArgs {
    std::string outputPath = "compiled_odyplan_plan";
    std::string targetTriple;
    std::optional<std::string> inputPath;
    std::optional<std::string> inputString;
    std::uint8_t optimizationLevel = 3;
    bool verbose = false;
    bool linkExecutable = false;
    std::string parser = "TestPlanParser";
    bool help = false;
    bool version = false;
    bool dryrun = false;
    std::string odyPlanLib;
    std::string llvmLib;
};

CommandArgs parseCommandArguments(const int argc, char** argv) {
    CommandArgs cmdArgs;

    if (argc == 1) {
        throw std::runtime_error("No parameter specified! Use -h/--help for help!");
    }

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            cmdArgs.help = true;
            return cmdArgs;
        }

        if (arg == "-v" || arg == "--version") {
            cmdArgs.version = true;
            return cmdArgs;
        }

        if (arg == "-op" || arg == "--output") {
            if (i + 1 >= argc) {
                throw std::runtime_error("Missing argument for output");
            }
            cmdArgs.outputPath = argv[++i];
            continue;
        }

        if (arg == "-opl" || arg == "--odyplan_lib") {
            if (i + 1 >= argc) {
                throw std::runtime_error("Missing argument for odyplan_lib");
            }
            cmdArgs.odyPlanLib = argv[++i];
            continue;
        }

        if (arg == "-lvl" || arg == "--llvm_lib") {
            if (i + 1 >= argc) {
                throw std::runtime_error("Missing argument for llvm_lib");
            }
            cmdArgs.llvmLib = argv[++i];
            continue;
        }

        if (arg == "-s" || arg == "--string") {
            if (i + 1 >= argc) {
                throw std::runtime_error("Missing argument for string based input");
            }
            cmdArgs.inputString = argv[++i];
            continue;
        }

        if (arg == "-f" || arg == "--file") {
            if (i + 1 >= argc) {
                throw std::runtime_error("Missing argument for file based input");
            }
            cmdArgs.inputPath = argv[++i];
            continue;
        }

        if (arg == "-p" || arg == "--parser") {
            if (i + 1 >= argc) {
                throw std::runtime_error("Missing argument for parser");
            }
            cmdArgs.parser = argv[++i];
            continue;
        }

        if (arg == "-t" || arg == "--target") {
            if (i + 1 >= argc) {
                throw std::runtime_error("Missing argument for target");
            }
            cmdArgs.targetTriple = argv[++i];
            continue;
        }

        if (arg == "-o" || arg == "--optimize") {
            if (i + 1 >= argc) {
                throw std::runtime_error("Missing argument for optimize");
            }
            const int level = std::stoi(argv[++i]);
            if (level < 0 || level > 3) {
                throw std::runtime_error("Optimization level must be 0-3");
            }
            cmdArgs.optimizationLevel = static_cast<std::uint8_t>(level);
            continue;
        }

        if (arg == "-l" || arg == "--link") {
            cmdArgs.linkExecutable = true;
            continue;
        }

        if (arg == "--verbose") {
            cmdArgs.verbose = true;
            continue;
        }

        if (arg == "--dry") {
            cmdArgs.dryrun = true;
            continue;
        }

        throw std::runtime_error(fmt::format("Unknown option: {}", arg));
    }

    if (!cmdArgs.inputPath.has_value() && !cmdArgs.inputString.has_value()) {
        throw std::runtime_error("No input specified. Use -f/--file or -s/--string");
    }

    if (cmdArgs.inputPath.has_value() && cmdArgs.inputString.has_value()) {
        throw std::runtime_error("Cannot specify input file and input string together");
    }

    return cmdArgs;
}

int main(int argc, char* argv[]) {
    auto logger = spdlog::stdout_color_mt("OdyCompiler");
    logger->set_level(spdlog::level::info);
    try {
        CommandArgs cmdArgs;
        try {
            cmdArgs = parseCommandArguments(argc, argv);
        } catch (const std::exception& e) {
            spdlog::error(fmt::format("Error: {}", e.what()));
            //printUsage(argv[0]);
            return 1;
        }

        if (cmdArgs.help) {
            printUsage(argv[0]);
            return 0;
        }

        if (cmdArgs.version) {
            printVersion();
            return 0;
        }

        if (cmdArgs.verbose) {
            logger->set_level(spdlog::level::debug);
            spdlog::debug("Debug mode enabled");
        }

        if (cmdArgs.inputPath.has_value()) {
            if (!std::filesystem::exists(cmdArgs.inputPath.value())) {
                spdlog::error(fmt::format("Input file does not exist: {}", cmdArgs.inputPath.value()));
                return 1;
            }
        }


        sierra::odycompiler::Compiler::Config config;
        config.outputPath = cmdArgs.outputPath;
        config.targetTriple = cmdArgs.targetTriple;
        config.inputPath = cmdArgs.inputPath;
        config.inputString = cmdArgs.inputString;
        config.optimizationLevel = cmdArgs.optimizationLevel;
        config.lnkExecutable = cmdArgs.linkExecutable;
        config.parser = cmdArgs.parser;
        config.dryrun = cmdArgs.dryrun;
        config.odyPlanLib = cmdArgs.odyPlanLib;
        config.llvmLib = cmdArgs.llvmLib;

        sierra::odycompiler::Timer compilerTimer{};
        compilerTimer.start();
        sierra::odycompiler::Compiler compiler(config);

        spdlog::info("=== OdyCompiler ===");

        if (config.inputPath.has_value()) {
            compiler.compileByFile(config.inputPath.value());
        } else if (config.inputString.has_value()) {
            compiler.compileByString(config.inputString.value());
        }
        compilerTimer.stop();

        return 0;
    } catch (const std::exception& e) {
        spdlog::error("Error: {}", e.what());
        return 1;
    }
}