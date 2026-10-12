file(GLOB_RECURSE ALL_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

set(MAIN_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/odycompiler/main.cpp")

list(FIND ALL_SOURCES "${MAIN_SOURCE}" MAIN_SOURCE_INDEX)
if(MAIN_SOURCE_INDEX EQUAL -1)
    message(FATAL_ERROR "MAIN_SOURCE not found in ALL_SOURCES: ${MAIN_SOURCE}")
endif()

list(REMOVE_ITEM ALL_SOURCES "${MAIN_SOURCE}")

set(LIB_SOURCES ${ALL_SOURCES})

set(LIB_NAME odycompiler_lib)
set(EXE_NAME odycompiler)

add_library(${LIB_NAME} STATIC ${LIB_SOURCES})
add_library(odycompiler::lib ALIAS ${LIB_NAME})

target_include_directories(${LIB_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(${LIB_NAME} PUBLIC
        odyplan::core
        odyplan::basic

        PRIVATE
        spdlog::spdlog
        nlohmann_json::nlohmann_json
        CLI11::CLI11
        Boost::headers
        Boost::filesystem

        ${ODYCOMPILER_SYSTEM_LIBS}
)

odyplan_link_llvm_aot(${LIB_NAME} PUBLIC)

add_executable(${EXE_NAME}
        ${MAIN_SOURCE}
)

target_link_libraries(${EXE_NAME} PRIVATE
        ${LIB_NAME}
)

target_compile_definitions(${EXE_NAME} PRIVATE
        ODYCOMPILER_VERSION="${PROJECT_VERSION}"
        ODYCOMPILER_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
        ODYCOMPILER_VERSION_MINOR=${PROJECT_VERSION_MINOR}
        ODYCOMPILER_VERSION_PATCH=${PROJECT_VERSION_PATCH}
)

set_target_properties(${EXE_NAME} PROPERTIES
        INSTALL_RPATH "$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
)

install(TARGETS ${EXE_NAME}
        RUNTIME_DEPENDENCIES
        DIRECTORIES
        ${LLVM_LIB_DIR_RUNTIME}
        $ENV{ODYPLAN_ROOT}/lib
        $ENV{ODYPLAN_ROOT}/lib64
        PRE_EXCLUDE_REGEXES
        "^libc\\.so" "^libm\\.so" "^libpthread" "^libdl" "^librt"
        "^ld-linux" "^libstdc\\+\\+" "^libgcc_s"
        POST_EXCLUDE_REGEXES
        "^/lib64/ld-linux"
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

if(ENABLE_TOOLING)
    enable_clang_tidy_on_target(${LIB_NAME})
    enable_clang_tidy_on_target(${EXE_NAME})
    register_lint_sources(${LIB_SOURCES} ${MAIN_SOURCE} ${HEADERS})
    register_tidy_sources(${LIB_SOURCES} ${MAIN_SOURCE})
endif()