add_ody_test(example_ody_example_union
        INPUTS "testplan2.plan" "nexmark.person.10k.ndjson" "nexmark.person.10k.rev.ndjson"
        COMPILER_ARGS "-f testplan2.plan -l --parser OdysseusPlanParser"
        EXECUTABLE_NAME "example_ody_example_union"
#        EXEC_EXPECTED "ody_check_output.txt"
#       EXEC_ARGS ""
)