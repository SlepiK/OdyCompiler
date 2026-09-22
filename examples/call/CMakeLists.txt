add_executable(odycompiler_example_test_call
        src/test_call.cpp
)

target_link_libraries(odycompiler_example_test_call PRIVATE
        odycompiler::lib
        spdlog::spdlog
        nlohmann_json::nlohmann_json
)

target_include_directories(odycompiler_example_test_call PRIVATE
        ${CMAKE_SOURCE_DIR}/odycompiler/include
)

add_custom_command(
        TARGET odycompiler_example_test_call
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/data/read.ndjson ${CMAKE_CURRENT_BINARY_DIR}/
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/data/test_plan.json ${CMAKE_CURRENT_BINARY_DIR}/
        COMMENT "Copying test data files"
)

target_compile_definitions(odycompiler_example_test_call PRIVATE
        ODYPLAN_LIB_PATH="${ODYPLAN_LIB_DIR_RUNTIME}"
        LLVM_LIB_PATH="${LLVM_LIB_DIR_RUNTIME}"
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(odycompiler_example_test_call PRIVATE
            -fsanitize=address,undefined
            -fno-omit-frame-pointer
    )
    target_link_options(odycompiler_example_test_call PRIVATE
            -fsanitize=address,undefined
    )
endif()

add_dependencies(odycompiler_example_all odycompiler_example_test_call)