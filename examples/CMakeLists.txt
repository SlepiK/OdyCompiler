set(ODYPLAN_LIB_DIR "" CACHE PATH "Path to OdyPlan lib directory")
set(LLVM_LIB_DIR "" CACHE PATH "Path to LLVM lib directory")

enable_testing()
add_custom_target(odycompiler_example_all)

add_subdirectory(call)
add_subdirectory(plan)

add_dependencies(odycompiler_example_all
        odycompiler_example_plan
)