cmake_minimum_required(VERSION 3.30.0)

project(HF-Miner VERSION 0.0.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 23)

# Options - manually enabled/disabled
option(ENABLE_UNIT_TESTING "Enable test targets" ON)
option(ENABLE_FORMAT "Enable code formatting" OFF)
option(ENABLE_CPPCHECK "Enable Cppcheck static analysis" OFF)
option(ENABLE_DOXYGEN "Enable Doxygen documentation generation" OFF)
option(ENABLE_COVERAGE "Enable code coverage analysis" OFF)
option(ENABLE_MEMCHECK "Enable memory checking" OFF)
option(ENABLE_BENCHMARKS "Enable benchmark targets" OFF)

# Include common modules
include(Format)
include(Doxygen)
include(Testing)
include(Cppcheck)
include(Coverage)
include(Memcheck)
include(Benchmarking)
include(FTXUI)

# Include subdirectories
add_subdirectory(src)

if(ENABLE_UNIT_TESTING)
    add_subdirectory(test)
endif()

if(ENABLE_BENCHMARKS)
    add_subdirectory(benchmark)
endif()