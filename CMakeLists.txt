cmake_minimum_required(VERSION 3.30.0)

project(HF-Miner VERSION 0.0.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 23)

# Determine if Debug or Release build (default to Debug for presets without CMAKE_BUILD_TYPE)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Set option defaults based on build type
# Debug: Most options enabled for development and testing
# Release: Most options disabled for optimized production builds
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(DEBUG_BUILD ON)
else()
    set(DEBUG_BUILD OFF)
endif()

# Options - enabled by default in Debug, disabled in Release
option(ENABLE_DOXYGEN "Enable Doxygen documentation generation" OFF)
option(ENABLE_CPPCHECK "Enable Cppcheck static analysis" ${DEBUG_BUILD})
option(ENABLE_COVERAGE "Enable code coverage analysis" ${DEBUG_BUILD})
option(ENABLE_FORMAT "Enable code formatting" ${DEBUG_BUILD})
option(ENABLE_MEMCHECK "Enable memory checking" ${DEBUG_BUILD})
option(ENABLE_BENCHMARKS "Enable benchmark targets" ${DEBUG_BUILD})
option(ENABLE_UNIT_TESTING "Enable test targets" ${DEBUG_BUILD})

# Include common modules
include(Format)
include(Doxygen)
include(Testing)
include(Cppcheck)
include(Coverage)
include(Memcheck)
include(Benchmarking)


# Include subdirectories
add_subdirectory(src)

if(ENABLE_UNIT_TESTING)
    add_subdirectory(test)
endif()

if(ENABLE_BENCHMARKS)
    add_subdirectory(benchmark)
endif()