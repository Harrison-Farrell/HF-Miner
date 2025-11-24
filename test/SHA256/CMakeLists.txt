add_executable(test_sha256
	test_sha256.cpp
)

target_link_libraries(test_sha256
	PRIVATE sha256_obj
)

Format(test_sha256 ${CMAKE_CURRENT_SOURCE_DIR})
AddGTests(test_sha256)
EnableCoverage(sha256_obj)
