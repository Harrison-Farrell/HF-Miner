if(ENABLE_UNIT_TESTING) 
	enable_testing()
	include(FetchContent)
	FetchContent_Declare(
		googletest
		GIT_REPOSITORY https://github.com/google/googletest.git
		GIT_TAG v1.17.0
		GIT_SHALLOW 1
	)

	# For Windows: Prevent overriding the parent project's compiler/linker settings
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

	option(INSTALL_GMOCK OFF)
	option(INSTALL_GTEST OFF)

	FetchContent_MakeAvailable(googletest)

	include(CTest)
	include(GoogleTest)
	include(Coverage)
	include(Memcheck)
endif()

macro(AddGTests target)
	if(NOT ENABLE_UNIT_TESTING)
		message(STATUS "Unit testing not enabled. Test target will not be created.")
		return()
	endif()
	AddCoverage(${target})
	target_link_libraries(${target}
		PRIVATE GTest::gtest
		PRIVATE GTest::gtest_main
		PRIVATE GTest::gmock
		PRIVATE GTest::gmock_main
	)
	gtest_discover_tests(${target})
	AddMemcheck(${target})
endmacro()