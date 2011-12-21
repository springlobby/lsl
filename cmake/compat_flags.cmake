Include(TestCXXAcceptsFlag)
execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion
				OUTPUT_VARIABLE GCC_VERSION)
