add_ody_test(example_ody_example
        INPUTS "testplan2.plan" "nexmark.person.10k.ndjson"
        COMPILER_ARGS "-f testplan2.plan -l --parser OdysseusPlanParser"
        EXECUTABLE_NAME "example_ody_example"
#        EXEC_EXPECTED "ody_check_output.txt"
#       EXEC_ARGS ""
)
target_compile_definitions(odycompiler_lib PRIVATE ODYSSEUS_PLAN_PARSER_AUTO_PRINT_OPERATOR)