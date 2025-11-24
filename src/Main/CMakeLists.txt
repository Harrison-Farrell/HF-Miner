set(executable_name HF-Miner)

add_executable(${executable_name} main.cpp)

target_compile_options(${executable_name}
	PRIVATE ${DEFAULT_CXX_COMPILE_FLAGS}
	PRIVATE ${DEFAULT_CXX_OPTIMIZE_FLAG}
)

target_link_libraries(${executable_name}
	PRIVATE sha256_static
)