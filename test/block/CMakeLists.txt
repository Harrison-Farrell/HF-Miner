add_executable(test_block
	test_block.cpp
)

target_link_libraries(test_block
	PRIVATE HFM::types		
	PRIVATE HFM::sha256
	PRIVATE HFM::block
)

Format(test_block ${CMAKE_CURRENT_SOURCE_DIR})
AddGTests(test_block)
EnableCoverage(block)
