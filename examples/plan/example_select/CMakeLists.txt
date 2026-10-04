add_ody_test(example_select
    INPUTS "select_plan.json" "nexmark.person.10k.ndjson"
    COMPILER_ARGS "-f select_plan.json -l"
    EXECUTABLE_NAME "select_plan"
#    EXEC_EXPECTED "expected_exec.txt"
#    EXEC_ARGS ""
)