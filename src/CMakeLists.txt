
include_directories(Array)
include_directories(Graph)
include_directories(definitions)

include_directories(timers)


file( GLOB graphSRC Graph/*)
file(GLOB utilitySRC Array/*)
add_executable(runner runner.cpp ${graphSRC} ${utilitySRC} timers/timers.cpp)

install (TARGETS runner DESTINATION ${CMAKE_BINARY_DIR})
