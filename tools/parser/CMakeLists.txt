set(executable_name blockheader_parser)

add_executable(${executable_name} blockheader_parser.cpp)

target_link_libraries(${executable_name}
	PRIVATE spdlog::spdlog
)

target_compile_options(${executable_name}
	PRIVATE ${DEFAULT_CXX_COMPILE_FLAGS}
	PRIVATE ${DEFAULT_CXX_OPTIMIZE_FLAG}
)

target_include_directories(${executable_name}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(${executable_name} PROPERTIES
	FOLDER "tools"
)

# project custom macros
CleanCoverage(${executable_name})
Format(${executable_name} .)
AddCppcheck(${executable_name})
AddSpdlog(${executable_name})
