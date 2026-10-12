add_ody_test(example_ody_example_project
        INPUTS "testplan2.plan" "nexmark.person.10k.ndjson"
        COMPILER_ARGS "-f testplan2.plan -l --parser OdysseusPlanParser"
        EXECUTABLE_NAME "example_ody_example_project"
#        EXEC_EXPECTED "ody_check_output.txt"
#       EXEC_ARGS ""
)