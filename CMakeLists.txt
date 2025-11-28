cmake_minimum_required(VERSION 3.30.0)

project(HF-Miner VERSION 0.0.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 23)

# Options
option(ENABLE_DOXYGEN "Enable Doxygen documentation generation" OFF)
option(ENABLE_CPPCHECK "Enable Cppcheck static analysis" OFF)           #-DENABLE_CPPCHECK=ON
option(ENABLE_COVERAGE "Enable code coverage analysis" OFF)             #-DENABLE_COVERAGE=ON
option(ENABLE_FORMAT "Enable code formatting" OFF)                      #-DENABLE_FORMAT=ON
option(ENABLE_MEMCHECK "Enable memory checking" OFF)                    #-DENABLE_MEMCHECK=ON
option(ENABLE_BENCHMARKS "Enable benchmark targets" OFF)
option(ENABLE_UNIT_TESTING "Enable test targets" OFF)                   #-DENABLE_UNIT_TESTING=ON

# Include common modules
include(Doxygen)
include(Cppcheck)
include(Coverage)
include(Format)
include(Memcheck)
include(Benchmarking)
include(Testing)

# Include subdirectories
add_subdirectory(src)

if(ENABLE_UNIT_TESTING)
    add_subdirectory(test)
endif()

if(ENABLE_BENCHMARKS)
    add_subdirectory(benchmark)
endif()