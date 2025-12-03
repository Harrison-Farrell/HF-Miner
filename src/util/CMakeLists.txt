set(library_name util)

add_library(${library_name} STATIC 
	endian.cpp
	transcode.cpp
)
add_library(HFM::${library_name} ALIAS ${library_name})

target_compile_options(${library_name} 
	PRIVATE ${DEFAULT_CXX_COMPILE_FLAGS}
	PRIVATE ${DEFAULT_CXX_OPTIMIZE_FLAG}
)

target_include_directories(${library_name}
	PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
	PUBLIC "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

set_target_properties(${library_name} PROPERTIES
	PUBLIC_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/${library_name}/endian.h
	PUBLIC_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/${library_name}/transcode.h
	POSITION_INDEPENDENT_CODE 1
)

CleanCoverage(${library_name})
Format(${library_name} .)
AddCppcheck(${library_name})
Doxygen(${library_name} ${library_name})