cmake_minimum_required(VERSION 4.1)
project(OdyCompiler VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(ENABLE_TOOLING "Enable clang-tidy/format targets" ON)
option(ENABLE_CLANG_TIDY "Run clang-tidy as part of the build" OFF)
option(ENABLE_CLANG_FORMAT_CHECK "Enable clang-format/format-check targets" OFF)

define_property(GLOBAL PROPERTY LINT_SOURCE_FILES_GLOBAL
        BRIEF_DOCS "Aggregated list of source and header files for linting"
        FULL_DOCS "..."
)
set_property(GLOBAL PROPERTY LINT_SOURCE_FILES_GLOBAL "")

define_property(GLOBAL PROPERTY TIDY_SOURCE_FILES_GLOBAL
        BRIEF_DOCS "Aggregated .cpp files for clang-tidy (NO headers!)"
        FULL_DOCS "..."
)
set_property(GLOBAL PROPERTY TIDY_SOURCE_FILES_GLOBAL "")

define_property(GLOBAL PROPERTY TIDY_HEADER_TARGETS_GLOBAL
        BRIEF_DOCS "Names of header-check object targets for clang-tidy"
        FULL_DOCS "..."
)
set_property(GLOBAL PROPERTY TIDY_HEADER_TARGETS_GLOBAL "")

include(cmake/dependencies.cmake)

include(cmake/llvm.cmake)

if(ENABLE_TOOLING)
    include(cmake/tooling.cmake)
endif ()

enable_testing()
add_subdirectory(odycompiler)
add_subdirectory(examples)

if(ENABLE_TOOLING)
    finalize_lint_targets()
endif ()

install(EXPORT OdyCompilerTargets
        FILE OdyCompilerTargets.cmake
        NAMESPACE odycompiler::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OdyCompiler
)

configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/OdyCompilerConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/OdyCompilerConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OdyCompiler
)

write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/OdyCompilerConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/OdyCompilerConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/OdyCompilerConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OdyCompiler
)

message(STATUS "=== OdyCompiler Build Done ===")
message(STATUS "Version: ${ODYCOMPILER_VERSION}")
message(STATUS "Install Prefix: ${CMAKE_INSTALL_PREFIX}")