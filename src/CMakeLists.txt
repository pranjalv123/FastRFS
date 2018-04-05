cmake_minimum_required (VERSION 2.6)
project (FastRFS)

file(GLOB SOURCES *.cpp)

add_definitions(-std=c++14 -g -O3 -Wall)

add_executable(FastRFS ${SOURCES} FastRFS.cpp)
set_target_properties(FastRFS PROPERTIES OUTPUT_NAME "FastRFS-glob")
target_link_libraries(FastRFS phylokit phylonaut)

INCLUDE_DIRECTORIES("boost/" )
install(TARGETS FastRFS
		RUNTIME DESTINATION bin)
