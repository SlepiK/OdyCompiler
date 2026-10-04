add_ody_test(example_count_window
        INPUTS "count_window_plan.json" "nexmark.person.10k.ndjson"
        COMPILER_ARGS "-f count_window_plan.json -l"
        EXECUTABLE_NAME "count_window_plan"
        EXEC_EXPECTED "ody_check_output.txt"
#       EXEC_ARGS ""
)