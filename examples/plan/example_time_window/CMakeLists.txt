add_ody_test(example_time_window
    INPUTS "time_window_plan.json" "nexmark.person.10k.ndjson"
    COMPILER_ARGS "-f time_window_plan.json -l"
    EXECUTABLE_NAME "time_window_plan"
#    EXEC_EXPECTED "expected_exec.txt"
#    EXEC_ARGS ""
)