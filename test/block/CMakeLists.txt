add_executable(test_blockHeader test_blockHeader.cpp)

target_link_libraries(test_blockHeader
	PRIVATE HFM::types		
	PRIVATE HFM::sha256
	PRIVATE HFM::block
)

Format(test_blockHeader ${CMAKE_CURRENT_SOURCE_DIR})
AddGTests(test_blockHeader)
EnableCoverage(block)
