add_ody_test(example_time_window
        INPUTS "time_window_plan.json" "nexmark.person.10k.ndjson"
        COMPILER_ARGS "-f time_window_plan.json -l"
        EXECUTABLE_NAME "time_window_plan"
        EXEC_EXPECTED "ody_check_output.txt"
#       EXEC_ARGS ""
)