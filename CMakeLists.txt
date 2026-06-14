cmake_minimum_required(VERSION 4.1)
project(odysseus_sierra_cpp)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG        v3.12.0
)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.17.0
)
set(SPDLOG_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(SPDLOG_FMT_EXTERNAL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(spdlog)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(zstd QUIET)
if(NOT zstd_FOUND)
    # Fedora: zstd-devel installieren falls nicht vorhanden
    find_library(ZSTD_LIB NAMES zstd REQUIRED)
    add_library(zstd::libzstd_static UNKNOWN IMPORTED)
    set_target_properties(zstd::libzstd_static PROPERTIES
            IMPORTED_LOCATION "${ZSTD_LIB}"
    )
endif()

if(NOT LLVM_INSTALL_DIR)
    set(LLVM_INSTALL_DIR /mnt/storage/ssdbig/llvm/llvm-22)
endif()

set(LLVM_DIR  ${LLVM_INSTALL_DIR}/lib/cmake/llvm)
set(Clang_DIR ${LLVM_INSTALL_DIR}/lib/cmake/clang)

find_package(LLVM REQUIRED CONFIG
    PATHS "${LLVM_DIR}"
    NO_DEFAULT_PATH)

find_package(Clang REQUIRED CONFIG
    PATHS "${Clang_DIR}"
    NO_DEFAULT_PATH)

message(STATUS "FOUND LLVM ${LLVM_VERSION}: ${LLVM_DIR}")
message(STATUS "FOUND Clang ${Clang_VERSION}: ${Clang_DIR}")

include_directories(${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

if(LLVM_ENABLE_ABI_BREAKING_CHECKS)
    add_definitions(-DLLVM_ENABLE_ABI_BREAKING_CHECKS=1)
else()
    add_definitions(-DLLVM_ENABLE_ABI_BREAKING_CHECKS=0)
endif()

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/CompilerMain/src/*.cpp"
)

add_executable(odysseus_sierra_cpp
        Compiler.cpp
        ${SOURCES}
)

target_compile_definitions(odysseus_sierra_cpp PRIVATE
        LLVM_CLANG_EXECUTABLE="${LLVM_INSTALL_DIR}/bin/clang"
        LLVM_RESOURCE_DIR="${LLVM_INSTALL_DIR}/lib/clang/${LLVM_VERSION_MAJOR}"
)

target_include_directories(odysseus_sierra_cpp PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/CompilerMain/include"
)

target_link_libraries(odysseus_sierra_cpp PRIVATE
        nlohmann_json::nlohmann_json
        spdlog::spdlog_header_only

        # Clang frontend
        clangFrontend
        clangFrontendTool
        clangDriver
        clangSerialization
        clangCodeGen
        clangParse
        clangSema
        clangAnalysis
        clangAST
        clangASTMatchers
        clangEdit
        clangLex
        clangBasic

        # LLVM backend
        LLVMX86CodeGen
        LLVMX86AsmParser
        LLVMX86Desc
        LLVMX86Info
        LLVMPasses
        LLVMipo
        LLVMLinker
        LLVMBitWriter
        LLVMTarget
        LLVMMC
        LLVMMCParser
        LLVMSupport
        LLVMCore
        LLVMAnalysis
)