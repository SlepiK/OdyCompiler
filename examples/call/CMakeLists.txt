add_executable(example_test_call
        src/test_call.cpp
)

target_link_libraries(example_test_call PRIVATE
        odycompiler::lib
        spdlog::spdlog
        nlohmann_json::nlohmann_json
)

target_include_directories(example_test_call PRIVATE
        ${CMAKE_SOURCE_DIR}/odycompiler/include
)

file(COPY
        data/read.ndjson
        data/test_plan.json
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

target_compile_definitions(example_test_call PRIVATE
        ODYPLAN_LIB_PATH="${ODYPLAN_LIB_DIR_RUNTIME}"
        LLVM_LIB_PATH="${LLVM_LIB_DIR_RUNTIME}"
)

set(CMAKE_CXX_FLAGS_DEBUG "-fsanitize=address,undefined -fno-omit-frame-pointer -g")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-fsanitize=address,undefined")