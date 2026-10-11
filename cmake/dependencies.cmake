include(FetchContent)

option(ODYPLAN_USE_SYSTEM_DEPS "Use system-installed dependencies (production)" ON)

message(STATUS "Searching spdlog")

if(ODYPLAN_USE_SYSTEM_DEPS)
    find_package(spdlog REQUIRED)
    message(STATUS "Found system spdlog: ${spdlog_DIR}")
else()
    find_package(spdlog QUIET)
    if(NOT spdlog_FOUND)
        include(FetchContent)
        FetchContent_Declare(
                spdlog
                GIT_REPOSITORY https://github.com/gabime/spdlog.git
                GIT_TAG v1.17.0
                SYSTEM
        )
        FetchContent_MakeAvailable(spdlog)
        message(STATUS "Fetched spdlog: ${spdlog_SOURCE_DIR}")
    endif()
endif()

message(STATUS "Searching nlohmann_json")

if(ODYPLAN_USE_SYSTEM_DEPS)
    find_package(nlohmann_json REQUIRED)
    message(STATUS "Found system nlohmann_json: ${nlohmann_json_DIR}")
else()
    find_package(nlohmann_json QUIET)
    if(NOT nlohmann_json_FOUND)
        include(FetchContent)
        FetchContent_Declare(
                nlohmann_json
                GIT_REPOSITORY https://github.com/nlohmann/json.git
                GIT_TAG v3.12.0
                SYSTEM
        )
        FetchContent_MakeAvailable(nlohmann_json)
        message(STATUS "Fetched nlohmann_json: ${nlohmann_json_SOURCE_DIR}")
    endif()
endif()

message(STATUS "Searching cli11")

if(ODYPLAN_USE_SYSTEM_DEPS)
    find_package(CLI11 REQUIRED)
    message(STATUS "Found system CLI11: ${CLI11_DIR}")
else()
    find_package(CLI11 QUIET)
    if(NOT CLI11_FOUND)
        include(FetchContent)
        FetchContent_Declare(
                CLI11
                GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
                GIT_TAG v2.5.0
                SYSTEM
        )
        FetchContent_MakeAvailable(CLI11)
        message(STATUS "Fetched CLI11: ${CLI11_SOURCE_DIR}")
    endif()
endif()

message(STATUS "Searching boost")

if(ODYPLAN_USE_SYSTEM_DEPS)
    find_package(Boost REQUIRED COMPONENTS filesystem)
    message(STATUS "Found system Boost: ${Boost_INCLUDE_DIRS}")
else()
    find_package(Boost COMPONENTS dll QUIET)
    if(NOT Boost_FOUND)
        FetchContent_Declare(
                boost
                GIT_REPOSITORY https://github.com/boostorg/boost.git
                GIT_TAG boost-1.84.0
                SYSTEM
        )
        FetchContent_MakeAvailable(boost)
        message(STATUS "Fetched Boost: ${boost_SOURCE_DIR}")
    endif()
endif()

if(NOT ODYPLAN_USE_SYSTEM_DEPS)
    if(TARGET spdlog::spdlog)
        include_directories(${spdlog_SOURCE_DIR}/include)
        message(STATUS "Added spdlog include: ${spdlog_SOURCE_DIR}/include")
    endif()

    if(TARGET nlohmann_json::nlohmann_json)
        include_directories(${nlohmann_json_SOURCE_DIR}/include)
        message(STATUS "Added nlohmann_json include: ${nlohmann_json_SOURCE_DIR}/include")
    endif()

    if(TARGET CLI11::CLI11)
        include_directories(${CLI11_SOURCE_DIR}/include)
        message(STATUS "Added CLI11 include: ${CLI11_SOURCE_DIR}/include")
    endif()
endif()

message(STATUS "Searching zstd")

find_package(zstd QUIET)
if(NOT zstd_FOUND)
    find_library(ZSTD_LIB NAMES zstd zstd_static REQUIRED)
    if(NOT TARGET zstd::libzstd_static)
        add_library(zstd::libzstd_static UNKNOWN IMPORTED)
        set_target_properties(zstd::libzstd_static PROPERTIES
                IMPORTED_LOCATION "${ZSTD_LIB}"
        )
    endif()
    message(STATUS "Found zstd library: ${ZSTD_LIB}")
endif()

message(STATUS "Searching odyplan")

if(NOT OdyPlan_DIR AND DEFINED ENV{OdyPlan_DIR})
    set(OdyPlan_DIR "$ENV{OdyPlan_DIR}" CACHE PATH "OdyPlan CMake config directory")
    message(STATUS "Using OdyPlan_DIR from environment: ${OdyPlan_DIR}")
endif()

if(NOT OdyPlan_DIR AND DEFINED ENV{ODYPLAN_ROOT})
    set(_odyplan_root "$ENV{ODYPLAN_ROOT}")
    message(STATUS "Searching for OdyPlan config in: ${_odyplan_root}")

    set(_odyplan_subdirs
            "lib64/cmake/OdyPlan"
            "lib/cmake/OdyPlan"
            "cmake/OdyPlan"
            "cmake"
            "lib64/cmake"
            "lib/cmake"
    )

    if(WIN32)
        list(APPEND _odyplan_subdirs
                "cmake/OdyPlan"
                "OdyPlan/cmake"
                "lib/cmake/OdyPlan"
        )
    endif()

    foreach(_subdir IN LISTS _odyplan_subdirs)
        set(_config_path "${_odyplan_root}/${_subdir}/OdyPlanConfig.cmake")
        if(EXISTS "${_config_path}")
            set(OdyPlan_DIR "${_odyplan_root}/${_subdir}" CACHE PATH "OdyPlan CMake config directory")
            message(STATUS "Auto-detected OdyPlan config at: ${OdyPlan_DIR}")
            break()
        endif()
    endforeach()
endif()

if(NOT OdyPlan_DIR AND ODYPLAN_ROOT)
    list(APPEND CMAKE_PREFIX_PATH "${ODYPLAN_ROOT}")
    message(STATUS "Using CMAKE_PREFIX_PATH: ${ODYPLAN_ROOT}")
endif()

find_package(OdyPlan REQUIRED CONFIG
        HINTS ${OdyPlan_DIR}
)

message(STATUS "Found OdyPlan: ${OdyPlan_DIR}")
if(OdyPlan_VERSION)
    message(STATUS "OdyPlan Version: ${OdyPlan_VERSION}")
endif()

set(ODYCOMPILER_SYSTEM_LIBS "")

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_package(Threads REQUIRED)
    list(APPEND ODYCOMPILER_SYSTEM_LIBS Threads::Threads dl)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    find_package(Threads REQUIRED)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    list(APPEND ODYCOMPILER_SYSTEM_LIBS ws2_32)
endif()

if(TARGET OdyPlan::Odyplan)
    get_target_property(ODYPLAN_LIB_LOCATION OdyPlan::Odyplan LOCATION)
    get_filename_component(ODYPLAN_LIB_DIR_RUNTIME "${ODYPLAN_LIB_LOCATION}" DIRECTORY)
else()
    get_filename_component(ODYPLAN_LIB_DIR_RUNTIME "${OdyPlan_DIR}" DIRECTORY)
    get_filename_component(ODYPLAN_LIB_DIR_RUNTIME "${ODYPLAN_LIB_DIR_RUNTIME}" DIRECTORY)
endif()

message(STATUS "OdyPlan Lib Dir: ${ODYPLAN_LIB_DIR_RUNTIME}")