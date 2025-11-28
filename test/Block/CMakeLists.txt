add_executable(test_block
	test_block.cpp
)

target_link_libraries(test_block
	PRIVATE HFM::Block
	PRIVATE HFM::SHA256
)

Format(test_block ${CMAKE_CURRENT_SOURCE_DIR})
AddGTests(test_block)
EnableCoverage(Block)
