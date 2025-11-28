add_executable(test_sha256
	test_sha256.cpp
)

target_link_libraries(test_sha256
	PRIVATE HFM::SHA256
)

Format(test_sha256 ${CMAKE_CURRENT_SOURCE_DIR})
AddGTests(test_sha256)
EnableCoverage(HFM::SHA256)
