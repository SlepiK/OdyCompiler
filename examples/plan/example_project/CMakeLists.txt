add_ody_test(example_project
    INPUTS "project_plan.json" "nexmark.person.10k.ndjson"
    COMPILER_ARGS "-f project_plan.json -l"
    EXECUTABLE_NAME "project_plan"
#    EXEC_EXPECTED "expected_exec.txt"
#    EXEC_ARGS ""
)