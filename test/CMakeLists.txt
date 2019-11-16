

include_directories (${PROJECT_SOURCE_DIR}/src/Graph)
include_directories (${PROJECT_SOURCE_DIR}/src/experiments)
include_directories (${PROJECT_SOURCE_DIR}/src/definitions)
include_directories (${PROJECT_SOURCE_DIR}/src/Array)
include_directories (${PROJECT_SOURCE_DIR}/src/timers)


add_executable(test_driver Main_TestAll.cpp baseline.cpp arrays.cpp simd.cpp)

target_link_libraries(test_driver ${GTEST_LIBRARIES} pthread moduleLib)

install (TARGETS test_driver DESTINATION ${CMAKE_BINARY_DIR})
