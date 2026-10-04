add_ody_test(example_union
        INPUTS "union_plan.json" "nexmark.person.10k.ndjson" "nexmark.person.10k.2.ndjson"
        COMPILER_ARGS "-f union_plan.json -l"
        EXECUTABLE_NAME "union_plan"
        EXEC_EXPECTED "ody_check_output.txt"
#       EXEC_ARGS ""
)