add_library(sha256_obj OBJECT
	sha256.cpp
)

target_compile_options(sha256_obj
	PRIVATE ${DEFAULT_CXX_COMPILE_FLAGS}
	PRIVATE ${DEFAULT_CXX_OPTIMIZE_FLAG}
)

target_include_directories(sha256_obj
	PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
)

set_target_properties(sha256_obj PROPERTIES
	PUBLIC_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/SHA256/sha256.h
	POSITION_INDEPENDENT_CODE 1
)

add_library(sha256_shared SHARED)
target_link_libraries(sha256_shared sha256_obj)
set_target_properties(sha256_shared PROPERTIES
	PREFIX "lib"
	OUTPUT_NAME "HF-Miner$<$<CONFIG:Debug>:d>"
)

add_library(sha256_static STATIC)
target_link_libraries(sha256_static sha256_obj)
set_target_properties(sha256_static PROPERTIES
	PREFIX "lib"
	OUTPUT_NAME "HF-Miner$<$<CONFIG:Debug>:d>"
)

CleanCoverage(sha256_static)
Format(sha256_static .)
AddCppcheck(sha256_static)
Doxygen(sha256_static SHA256)
