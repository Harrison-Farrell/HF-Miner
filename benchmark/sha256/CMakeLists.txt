# Add benchmark executable for SHA256
add_executable(benchmark_sha256
    benchmark_sha256.cpp
)

target_link_libraries(benchmark_sha256
    PRIVATE HFM::sha256
    PRIVATE HFM::util
    PRIVATE HFM::types
)

# Apply project formatting rules (if available) and link Google Benchmark
Format(benchmark_sha256 ${CMAKE_CURRENT_SOURCE_DIR})
AddBenchmarks(benchmark_sha256)
