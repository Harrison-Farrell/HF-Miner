add_library(block_obj OBJECT
	block.cpp
)

target_compile_options(block_obj
	PRIVATE ${DEFAULT_CXX_COMPILE_FLAGS}
	PRIVATE ${DEFAULT_CXX_OPTIMIZE_FLAG}
)

target_include_directories(block_obj
	PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
)

set_target_properties(block_obj PROPERTIES
	PUBLIC_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/Block/block.h
	POSITION_INDEPENDENT_CODE 1
)

add_library(block_shared SHARED)
target_link_libraries(block_shared block_obj)
set_target_properties(block_shared PROPERTIES
	PREFIX "lib"
	OUTPUT_NAME "HF-Miner$<$<CONFIG:Debug>:d>"
)

add_library(block_static STATIC)
target_link_libraries(block_static block_obj)
set_target_properties(block_static PROPERTIES
	PREFIX "lib"
	OUTPUT_NAME "HF-Miner$<$<CONFIG:Debug>:d>"
)

CleanCoverage(block_static)
Format(block_static .)
AddCppcheck(block_static)
Doxygen(block_static Block)
