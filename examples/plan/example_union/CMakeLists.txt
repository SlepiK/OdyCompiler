add_ody_test(example_union
    INPUTS "union_plan.json" "nexmark.person.10k.ndjson"
    COMPILER_ARGS "-f union_plan.json -l"
    EXECUTABLE_NAME "union_plan"
#    EXEC_EXPECTED "expected_exec.txt"
#    EXEC_ARGS ""
)