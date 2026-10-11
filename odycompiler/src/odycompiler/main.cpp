#include <filesystem>
#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <odycompiler/compiler.h>
#include <odycompiler/timer.h>
#include <CLI/CLI.hpp>
#include <boost/dll.hpp>

#ifndef ODYCOMPILER_VERSION
    #define ODYCOMPILER_VERSION "unknown"
#endif

void printVersion() {
    spdlog::info("OdyCompiler");
    spdlog::info(fmt::format(" Version: {}", ODYCOMPILER_VERSION));
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

sierra::odycompiler::Compiler::Config parseCommandArguments(const int argc, char** argv) {
    sierra::odycompiler::Compiler::Config compilerConfig;

    CLI::App compilerApp{"OdyCompiler - Compile query plans via OdyPlan to native binaries"};

    compilerApp.set_help_flag("-h,--help", "Show this help message");
    compilerApp.add_flag_callback("--version", []() {
            printVersion();
            throw CLI::Success();
        }, "Show version information")->trigger_on_parse();

    compilerApp.get_formatter()->column_width(30);

    auto inputGroup = compilerApp.add_option_group("input", "Input source of query plan (choose one)");
    inputGroup->add_option("-f,--file", compilerConfig.inputPath, "Input file path");
    inputGroup->add_option("-s,--string",compilerConfig.inputString, "Input string");
    inputGroup->require_option(1);

    compilerApp.add_option("-o,--output", compilerConfig.outputPath, "Output path with filename")->default_val("compiled_plan");

    compilerApp.add_flag("-l,--link", compilerConfig.lnkExecutable, "Automatic link to executable");

    compilerApp.add_option("-p,--parser", compilerConfig.parser, "Parser type (TestPlanParser, OdyPlanParser and maybe more...)")->default_val("TestPlanParser");

    compilerApp.add_option("-t,--target", compilerConfig.targetTriple, "Target triple (default: system default)");

    compilerApp.add_option("-O,--optimize", compilerConfig.optimizationLevel, "Optimization level (0-3)")->default_val(3)->check(CLI::Range(0, 3));

    compilerApp.add_option("--odyplan_lib", compilerConfig.odyPlanLib, "Path to OdyPlan lib folder");
    compilerApp.add_option("--llvm_lib", compilerConfig.llvmLib, "Path to LLVM library file");

    compilerApp.add_flag("-v,--verbose", compilerConfig.verbose, "Verbose output");
    compilerApp.add_flag("--dry", compilerConfig.dryrun, "Execute a dry-run");
    compilerApp.add_flag("--timer", compilerConfig.timer, "Measure time consumption during compilation");

    std::filesystem::path cfgPath = std::filesystem::path(boost::dll::program_location().string()).parent_path() / "config.cfg";
    compilerApp.set_config("--config",cfgPath.string(), "Read config from file", false);

    if (argc == 1) {
        std::cout << compilerApp.help() << std::endl;
        exit(0);
    }

    try {
        compilerApp.parse(argc, argv);
    } catch (const CLI::CallForHelp& e) {
        exit(compilerApp.exit(e));
    } catch (const CLI::CallForVersion& e) {
        exit(compilerApp.exit(e));
    } catch (const CLI::Success& e) {
        exit(compilerApp.exit(e));
    } catch (const CLI::ParseError& e) {
        throw std::runtime_error(fmt::format("Could not parse given parameters. Use [-h,--help] for help about usage"));
    }

    if (compilerConfig.inputPath.has_value()) {
        if (!std::filesystem::exists(compilerConfig.inputPath.value())) {
            throw std::runtime_error(fmt::format("Input file for query plan does not exist: {}", compilerConfig.inputPath.value()));
        }
    }

    return compilerConfig;
}

int main(const int argc, char* argv[]) {
    spdlog::stdout_color_mt("OdyCompiler");
    spdlog::set_level(spdlog::level::info);
    try {
        const auto config = parseCommandArguments(argc, argv);

        if (config.verbose) {
            spdlog::set_level(spdlog::level::debug);
            spdlog::debug("Debug mode enabled");
        }

        sierra::odycompiler::Timer compilerTimer{};
        if (config.timer) {
            compilerTimer.start();
        }

        sierra::odycompiler::Compiler compiler(config);

        spdlog::info("=== OdyCompiler ===");

        if (config.inputPath.has_value()) {
            compiler.compileByFile(config.inputPath.value());
        } else if (config.inputString.has_value()) {
            compiler.compileByString(config.inputString.value());
        }

        if (config.timer) {
            compilerTimer.stop();
        }

        return 0;
    } catch (const std::exception& e) {
        spdlog::error("Error: {}", e.what());
        return 1;
    }
}