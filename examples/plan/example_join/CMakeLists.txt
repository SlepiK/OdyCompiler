add_ody_test(example_join
    INPUTS "join_plan.json" "nexmark.person.10k.ndjson" "nexmark.bid.10k.ndjson"
    COMPILER_ARGS "-f join_plan.json -l"
    EXECUTABLE_NAME "join_plan"
#    EXEC_EXPECTED "expected_exec.txt"
#    EXEC_ARGS ""
)