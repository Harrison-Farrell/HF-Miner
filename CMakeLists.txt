cmake_minimum_required(VERSION 3.30.0)

project(HF-Miner VERSION 0.0.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# Set compiler flags
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


# Set default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

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

# Include subdirectories
add_subdirectory(src)

if(ENABLE_UNIT_TESTING)
    add_subdirectory(test)
endif()

if(ENABLE_BENCHMARKS)
    add_subdirectory(benchmark)
endif()

if(ENABLE_DOXYGEN)
    Doxygen(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR})
endif()