cmake_minimum_required(VERSION 3.30.0)

project(HF-Miner VERSION 0.0.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 23)

# options - manually enabled/disabled
option(ENABLE_UNIT_TESTING "Enable test targets" ON)
option(ENABLE_FORMAT "Enable code formatting" OFF)
option(ENABLE_CPPCHECK "Enable Cppcheck static analysis" OFF)
option(ENABLE_DOXYGEN "Enable Doxygen documentation generation" OFF)
option(ENABLE_COVERAGE "Enable code coverage analysis" OFF)
option(ENABLE_MEMCHECK "Enable memory checking" OFF)
option(ENABLE_BENCHMARKS "Enable benchmark targets" OFF)

# options modules
include(Memcheck)
include(Benchmarking)
include(Testing)
include(Cppcheck)
include(Coverage)
include(Format)
include(Doxygen)

# require modules
include(Spdlog)

# Include subdirectories
add_subdirectory(src)
add_subdirectory(tools)

if(ENABLE_UNIT_TESTING)
    add_subdirectory(test)
endif()

if(ENABLE_BENCHMARKS)
    add_subdirectory(benchmark)
endif()