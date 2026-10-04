add_ody_test(example_project
        INPUTS "project_plan.json" "nexmark.person.10k.ndjson"
        COMPILER_ARGS "-f project_plan.json -l"
        EXECUTABLE_NAME "project_plan"
        EXEC_EXPECTED "ody_check_output.txt"
#       EXEC_ARGS ""
)