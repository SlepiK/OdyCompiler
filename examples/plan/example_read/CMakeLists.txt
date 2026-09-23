add_ody_test(example_read
    INPUTS "read_plan.json" "read.ndjson"
    COMPILER_ARGS "-f read_plan.json -l"
    EXECUTABLE_NAME "readtestplan"
#    EXEC_EXPECTED "expected_exec.txt"
#    EXEC_ARGS ""
)