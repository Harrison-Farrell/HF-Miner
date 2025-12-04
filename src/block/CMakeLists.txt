set(library_name block)

add_library(${library_name} STATIC blockHeader.cpp)
add_library(HFM::${library_name} ALIAS ${library_name})

target_link_libraries(${library_name}
	PRIVATE HFM::sha256
	PRIVATE HFM::types
)

target_compile_options(${library_name}
	PRIVATE ${DEFAULT_CXX_COMPILE_FLAGS}
	PRIVATE ${DEFAULT_CXX_OPTIMIZE_FLAG}
)

target_include_directories(${library_name}
	PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
	PUBLIC "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

set_target_properties(${library_name} PROPERTIES
	PUBLIC_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/${library_name}/blockHeader.h
	POSITION_INDEPENDENT_CODE 1
)

CleanCoverage(${library_name})
Format(${library_name} .)
AddCppcheck(${library_name})