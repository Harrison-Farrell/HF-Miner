# CMake module to set up Google Benchmark for benchmarking
if(ENABLE_BENCHMARKS)
	include(FetchContent)

	FetchContent_Declare(
		goooglebenchmark
		GIT_REPOSITORY https://github.com/google/benchmark
		GIT_TAG v1.9.4
		GIT_SHALLOW 1
	)

	FetchContent_MakeAvailable(goooglebenchmark)
endif()

macro(AddBenchmarks target)
	if(NOT ENABLE_BENCHMARKS)
		message(STATUS "Benchmarking not enabled. Benchmark target will not be created.")
		return()
	endif()
	target_link_libraries(${target}
		PRIVATE benchmark::benchmark
		PRIVATE benchmark::benchmark_main
	)
endmacro()
