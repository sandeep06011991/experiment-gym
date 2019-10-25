
include_directories(Array)
include_directories(Graph)
include_directories(definitions)

file( GLOB graphSRC Graph/*)
file(GLOB utilitySRC Array/*)
add_executable(runner runner.cpp ${graphSRC} ${utilitySRC} )

install (TARGETS runner DESTINATION ${CMAKE_BINARY_DIR})
