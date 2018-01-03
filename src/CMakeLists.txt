cmake_minimum_required (VERSION 2.6)
project (FastRFS)

file(GLOB SOURCES *.cpp)

add_definitions(-std=c++14 -g -O3 -Wall)

add_executable(FastRFS ${SOURCES} FastRFS.cpp)
target_link_libraries(FastRFS phylokit phylonaut)

install(TARGETS FastRFS 
		RUNTIME DESTINATION bin)
