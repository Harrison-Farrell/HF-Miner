# CMakeLists.txt for test/util

EnableCoverage(util)

################################################
add_executable(test_transcode test_transcode.cpp)

target_link_libraries(test_transcode PRIVATE HFM::util)

Format(test_transcode ${CMAKE_CURRENT_SOURCE_DIR})
AddGTests(test_transcode)


################################################
add_executable(test_endian test_endian.cpp)

target_link_libraries(test_endian PRIVATE HFM::util)

Format(test_endian ${CMAKE_CURRENT_SOURCE_DIR})
AddGTests(test_endian)