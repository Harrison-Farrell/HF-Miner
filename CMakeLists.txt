cmake_minimum_required(VERSION 3.30.0)

project(HF-Miner VERSION 0.0.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set (CMAKE_CXX_STANDARD 23)

# Include common modules
include(Coverage)
include(Cppcheck)
include(Doxygen)
include(Format)

# Include subdirectories
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(benchmark)
