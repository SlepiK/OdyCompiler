add_ody_test(example_write
    INPUTS "write_plan.json" "nexmark.person.10k.ndjson"
    COMPILER_ARGS "-f write_plan.json -l"
    EXECUTABLE_NAME "write_plan"
#    EXEC_EXPECTED "expected_exec.txt"
#    EXEC_ARGS ""
)