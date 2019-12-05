
include_directories(Array)
include_directories(Graph)
include_directories(definitions)
include_directories(experiments)
include_directories(timers)


file( GLOB graphSRC Graph/*)
file(GLOB utilitySRC Array/*)
add_library(moduleLib STATIC ${graphSRC} ${utilitySRC} timers/timers.cpp)
add_executable(runner runner.cpp experiments/redundant.cpp)
target_link_libraries(runner moduleLib)

install (TARGETS runner DESTINATION ${CMAKE_BINARY_DIR})


add_executable(preprocessor preprocessor.cpp)
install (TARGETS preprocessor DESTINATION ${CMAKE_BINARY_DIR})