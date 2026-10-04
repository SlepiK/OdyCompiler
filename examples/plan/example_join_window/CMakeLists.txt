add_ody_test(example_join_window
    INPUTS "join_window_plan.json" "nexmark.person.10k.ndjson" "nexmark.bid.10k.ndjson"
    COMPILER_ARGS "-f join_window_plan.json -l"
    EXECUTABLE_NAME "join_window_plan"
#    EXEC_EXPECTED "expected_exec.txt"
#    EXEC_ARGS ""
)