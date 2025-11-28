# CMake module to set up Google Benchmark for benchmarking
include(FetchContent)

FetchContent_Declare(
	goooglebenchmark
	GIT_REPOSITORY https://github.com/google/benchmark
	GIT_TAG v1.9.4
	GIT_SHALLOW 1
)

FetchContent_MakeAvailable(goooglebenchmark)

macro(AddBenchmarks target)
	target_link_libraries(${target}
		PRIVATE benchmark::benchmark
		PRIVATE benchmark::benchmark_main
	)
endmacro()
