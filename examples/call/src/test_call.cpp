#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include <odycompiler/compiler.h>
#include <odycompiler/parser/testparser/test_plan_parser.h>
#include <odycompiler/parser/testparser/test_operator_parser_registry.h>

int main(int argc, char** argv) {
    try {
        std::cout << "=== OdyCompiler User Test ===" << std::endl;

        std::string inputFile = "test_plan.json";
        if (argc > 1) {
            inputFile = argv[1];
        }

        if (!std::filesystem::exists(inputFile)) {
            throw std::runtime_error("Input file not found: " + inputFile);
        }

        std::ifstream file(inputFile);
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string jsonContent = buffer.str();

        std::cout << "Input: " << inputFile << std::endl;

        std::cout << '\n';

        std::cout << "=== Test 1: Object File Only ===" << std::endl;

        sierra::odycompiler::Compiler::Config config1;
        config1.outputPath = "test_plan.o";
        config1.targetTriple = "";
        config1.optimizationLevel = 2;
        config1.lnkExecutable = false;
        config1.parser = sierra::odycompiler::parser::testparser::TestPlanParser::PARSER;
        config1.odyPlanLib = ODYPLAN_LIB_PATH;
        config1.llvmLib = LLVM_LIB_PATH;
        spdlog::set_level(spdlog::level::debug);

        sierra::odycompiler::Compiler compiler1(config1);

        compiler1.compileByString(jsonContent);

        std::cout << "Object file created: test_plan.o" << std::endl;

        std::cout << '\n';

        std::cout << "=== Test 2: Compile and Link ===" << std::endl;

        sierra::odycompiler::Compiler::Config config2;
        config2.outputPath = "test_plan_runner";
        config2.targetTriple = "";
        config2.optimizationLevel = 2;
        config2.lnkExecutable = true;
        config2.parser = sierra::odycompiler::parser::testparser::TestPlanParser::PARSER;
        config2.odyPlanLib = ODYPLAN_LIB_PATH;
        config2.llvmLib = LLVM_LIB_PATH;
        spdlog::set_level(spdlog::level::debug);

        sierra::odycompiler::Compiler compiler2(config2);

        compiler2.compileByString(jsonContent);

        std::cout << "Executable created: test_plan_runner" << std::endl;

        std::cout << '\n';

        std::cout << "=== Test 3: Verify Symbols ===" << std::endl;

#ifdef __linux__
        std::string nmCmd = "nm test_plan.o | grep -E 'ody_init|ody_main|ody_shutdown'";
        int result = std::system(nmCmd.c_str());
        if (result == 0) {
            std::cout << "Runtime symbols found in .o file" << std::endl;
        } else {
            std::cout << "Symbols check failed (may need nm installed)" << std::endl;
        }
#endif

        std::cout << '\n';

        std::cout << "=== All Tests Complete ===" << std::endl;

        return 0;
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}