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

if(NOT ODYPLAN_USE_SYSTEM_DEPS)
    if(TARGET spdlog::spdlog)
        include_directories(${spdlog_SOURCE_DIR}/include)
        message(STATUS "Added spdlog include: ${spdlog_SOURCE_DIR}/include")
    endif()

    if(TARGET nlohmann_json::nlohmann_json)
        include_directories(${nlohmann_json_SOURCE_DIR}/include)
        message(STATUS "Added nlohmann_json include: ${nlohmann_json_SOURCE_DIR}/include")
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

find_package(OdyPlan REQUIRED CONFIG
        PATHS
        $ENV{HOME}/dev/OdyPlan/install/lib64/cmake/OdyPlan
        $ENV{HOME}/dev/OdyPlan/install/lib/cmake/OdyPlan

        ${CMAKE_PREFIX_PATH}/lib64/cmake/OdyPlan
        ${CMAKE_PREFIX_PATH}/lib/cmake/OdyPlan

        /opt/odyplan/lib64/cmake/OdyPlan
        /opt/odyplan/lib/cmake/OdyPlan
        /usr/local/lib64/cmake/OdyPlan
        /usr/local/lib/cmake/OdyPlan
)

message(STATUS "Found OdyPlan: ${OdyPlan_DIR}")

set(ODYCOMPILER_SYSTEM_LIBS "")

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_package(Threads REQUIRED)
    list(APPEND ODYCOMPILER_SYSTEM_LIBS Threads::Threads dl)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    find_package(Threads REQUIRED)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    list(APPEND ODYCOMPILER_SYSTEM_LIBS ws2_32)
endif()