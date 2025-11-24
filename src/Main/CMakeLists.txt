set(executeable_name HF-Miner)

add_executable(${executeable_name} main.cpp)

target_compile_options(${executeable_name}
	PRIVATE ${DEFAULT_CXX_COMPILE_FLAGS}
	PRIVATE ${DEFAULT_CXX_OPTIMIZE_FLAG}
)

target_link_libraries(${executeable_name}
	PRIVATE sha256_static
)