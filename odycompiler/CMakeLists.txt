file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

set(TARGET_NAME OdyCompiler)

add_executable(${TARGET_NAME}
        ${SOURCES}
)

target_include_directories(${TARGET_NAME} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(${TARGET_NAME} PRIVATE
        odyplan::core
        odyplan::basic

        spdlog::spdlog
        nlohmann_json::nlohmann_json

        ${ODYCOMPILER_SYSTEM_LIBS}
)

odyplan_link_llvm_aot(${TARGET_NAME} PRIVATE)

enable_clang_tidy_on_target(${TARGET_NAME})

if(ENABLE_TOOLING)
    register_lint_sources(${SOURCES} ${HEADERS})
    register_tidy_sources(${SOURCES})
endif()