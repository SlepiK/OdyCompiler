add_custom_target(odycompiler_example_plan
        COMMENT "Building all OdyCompiler example plan tests"
)

if(TARGET odycompiler)
    message(STATUS "Using OdyCompiler from build target")
    set(ODYCOMPILER_EXECUTABLE "$<TARGET_FILE:odycompiler>")
    set(ODYCOMPILER_DEPENDENCY "odycompiler")
else()
    if(TARGET OdyPlan::Odyplan)
        get_target_property(ODYPLAN_BIN_DIR OdyPlan::Odyplan IMPORTED_LOCATION)
        get_filename_component(ODYPLAN_BIN_DIR "${ODYPLAN_BIN_DIR}" DIRECTORY)
    else()
        set(ODYPLAN_BIN_DIR "${ODYPLAN_LIB_DIR_RUNTIME}")
    endif()

    find_program(ODYCOMPILER_EXECUTABLE
            NAMES odycompiler ody-compiler
            HINTS ${ODYPLAN_BIN_DIR}
            PATHS /opt/odyplan/bin /usr/local/bin
            REQUIRED
    )
    set(ODYCOMPILER_DEPENDENCY "")
endif()

message(STATUS "OdyCompiler executable: ${ODYCOMPILER_EXECUTABLE}")

set(TEST_OUTPUT_DIR "${CMAKE_BINARY_DIR}/examples/plan")
file(MAKE_DIRECTORY ${TEST_OUTPUT_DIR})

enable_testing()

function(add_ody_test NAME)
    cmake_parse_arguments(ARG
            ""
            "EXECUTABLE_NAME;EXEC_EXPECTED"
            "INPUTS;COMPILER_ARGS;EXEC_ARGS"
            ${ARGN}
    )

    set(TEST_DIR "${TEST_OUTPUT_DIR}/${NAME}")
    file(MAKE_DIRECTORY ${TEST_DIR})
    set(TEST_INPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

    set(COPIED_FILES "")
    foreach(INPUT_FILE IN LISTS ARG_INPUTS)
        get_filename_component(FILE_NAME "${INPUT_FILE}" NAME)
        set(INPUT_SRC "${TEST_INPUT_DIR}/${INPUT_FILE}")
        set(INPUT_DST "${TEST_DIR}/${FILE_NAME}")

        add_custom_command(
                OUTPUT ${INPUT_DST}
                COMMAND ${CMAKE_COMMAND} -E copy "${INPUT_SRC}" "${INPUT_DST}"
                DEPENDS "${INPUT_SRC}"
                COMMENT "Copying ${INPUT_FILE} for ${NAME}"
        )
        list(APPEND COPIED_FILES ${INPUT_DST})
    endforeach()

    set(_exec_name "${ARG_EXECUTABLE_NAME}")
    if(NOT _exec_name)
        set(_exec_name "odytestplan")
    endif()

    set(COMPILE_EXECUTABLE "${TEST_DIR}/${_exec_name}")
    separate_arguments(_args_list UNIX_COMMAND "${ARG_COMPILER_ARGS}")

    set(COMPILE_ARGS_LIST
            ${_args_list}
            -o "${_exec_name}"
            --odyplan_lib "${ODYPLAN_LIB_DIR_RUNTIME}"
            --llvm_lib "${LLVM_LIB_DIR_RUNTIME}"
    )

    separate_arguments(EXEC_ARGS_LIST UNIX_COMMAND "${ARG_EXEC_ARGS}")

    set(RUN_ENV
            "ODYPLAN_LIB_PATH=${ODYPLAN_LIB_DIR_RUNTIME}"
            "LLVM_LIB_PATH=${LLVM_LIB_DIR_RUNTIME}"
            "LD_LIBRARY_PATH=${ODYPLAN_LIB_DIR_RUNTIME}:${LLVM_LIB_DIR_RUNTIME}:$ENV{LD_LIBRARY_PATH}"
    )

    set(TARGET_NAME "odycompiler_example_plan_${NAME}")

    set(OUTPUT_FILE "${TEST_DIR}/output.txt")

    set(RUN_SCRIPT "${TEST_DIR}/run_test.sh")
    file(WRITE ${RUN_SCRIPT}
            "#!/bin/bash\n"
            "set -e\n"
            "export ODYPLAN_LIB_PATH='${ODYPLAN_LIB_DIR_RUNTIME}'\n"
            "export LLVM_LIB_PATH='${LLVM_LIB_DIR_RUNTIME}'\n"
            "export LD_LIBRARY_PATH='${ODYPLAN_LIB_DIR_RUNTIME}:${LLVM_LIB_DIR_RUNTIME}:$LD_LIBRARY_PATH'\n"
            "exec '${COMPILE_EXECUTABLE}' ${EXEC_ARGS_LIST}\n"
    )
    execute_process(COMMAND chmod +x ${RUN_SCRIPT})

    add_custom_command(
            OUTPUT ${OUTPUT_FILE}
            COMMAND ${CMAKE_COMMAND} -E echo "=== ${NAME} ==="
            COMMAND ${CMAKE_COMMAND} -E echo "Compiling..."
            COMMAND ${ODYCOMPILER_EXECUTABLE} ${COMPILE_ARGS_LIST}
            COMMAND ${CMAKE_COMMAND} -E echo "Running..."
            COMMAND sh -c "${RUN_SCRIPT} 2>&1 | tee ${OUTPUT_FILE}"
            DEPENDS ${COPIED_FILES} ${ODYCOMPILER_DEPENDENCY}
            WORKING_DIRECTORY ${TEST_DIR}
            COMMENT "Building and running ${NAME}"
            VERBATIM
    )

    add_custom_target(${TARGET_NAME} ALL DEPENDS ${OUTPUT_FILE})

    add_dependencies(odycompiler_example_plan ${TARGET_NAME})

    if(ARG_EXEC_EXPECTED)
        set(EXPECTED_FILE "${TEST_INPUT_DIR}/${ARG_EXEC_EXPECTED}")
        add_test(
                NAME ${TARGET_NAME}
                COMMAND ${CMAKE_COMMAND} -E compare_files ${OUTPUT_FILE} ${EXPECTED_FILE}
                WORKING_DIRECTORY ${TEST_DIR}
        )
        set_tests_properties(${TARGET_NAME} PROPERTIES
                WORKING_DIRECTORY ${TEST_DIR}
        )
    endif()
endfunction()

add_subdirectory(example_read)
add_subdirectory(example_project)
add_subdirectory(example_select)
add_subdirectory(example_count_window)
add_subdirectory(example_time_window)
add_subdirectory(example_write)
add_subdirectory(example_union)
add_subdirectory(example_join)
add_subdirectory(example_join_window)

target_compile_definitions(odycompiler_lib PRIVATE ODYSSEUS_PLAN_PARSER_AUTO_PRINT_OPERATOR)
add_subdirectory(ody_example)
add_subdirectory(ody_example_join)
add_subdirectory(ody_example_project)
add_subdirectory(ody_example_select)
add_subdirectory(ody_example_elementwindow)