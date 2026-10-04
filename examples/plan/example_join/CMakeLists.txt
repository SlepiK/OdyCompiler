add_ody_test(example_join
        INPUTS "join_plan.json" "nexmark.person.10k.ndjson" "nexmark.bid.10k.ndjson"
        COMPILER_ARGS "-f join_plan.json -l"
        EXECUTABLE_NAME "join_plan"
        EXEC_EXPECTED "ody_check_output.txt"
#       EXEC_ARGS ""
)