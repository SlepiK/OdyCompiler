#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

int main(int argc, char* argv[]) {
    auto logger = spdlog::stdout_color_mt("OdyCompiler");
    logger->set_level(spdlog::level::info);
    spdlog::info("=== OdyCompiler ===");
    spdlog::info("Starting Compilation");

    if (std::getenv("DEBUG")) {
        logger->set_level(spdlog::level::debug);
        SPDLOG_INFO("Debug mode enabled");
    }
}