add_ody_test(example_ody_example_join
        INPUTS "testplan.plan" "nexmark.person.10k.ndjson" "nexmark.bid.10k.ndjson"
        COMPILER_ARGS "-f testplan.plan -l --parser OdysseusPlanParser -v"
        EXECUTABLE_NAME "example_ody_example_join"
#        EXEC_EXPECTED "ody_check_output.txt"
#       EXEC_ARGS ""
)