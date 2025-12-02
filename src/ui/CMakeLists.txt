set(library_name ui)

add_library(${library_name} STATIC homepage.cpp)
add_library(HFM::${library_name} ALIAS ${library_name})

target_link_libraries(${library_name}
	PRIVATE HFM::block
    PRIVATE HFM::types
    PRIVATE HFM::sha256
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
	PUBLIC_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/${library_name}/homepage.h
	POSITION_INDEPENDENT_CODE 1
)

CleanCoverage(${library_name})
Format(${library_name} .)
AddCppcheck(${library_name})
Doxygen(${library_name} ${library_name})
AddFTXUI(${library_name})
