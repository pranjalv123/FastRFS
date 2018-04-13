cmake_minimum_required (VERSION 2.6)
project (FastRFS)
OPTION(BUILD_STATIC "build static" ON)
OPTION(BUILD_SHARED "build static" ON)
file(GLOB SOURCES *.cpp)

add_definitions(-std=c++14 -g -O3 -Wall)


if (BUILD_SHARED)
	add_executable(FastRFS ${SOURCES} FastRFS.cpp)
	target_link_libraries(FastRFS phylokit phylonaut)

	INCLUDE_DIRECTORIES("boost/" )
	install(TARGETS FastRFS
			RUNTIME DESTINATION bin)
endif()
if (BUILD_STATIC)
	SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
	SET(BUILD_SHARED_LIBRARIES OFF)

	add_executable(FastRFS_static ${SOURCES} FastRFS.cpp)
  SET_TARGET_PROPERTIES(FastRFS_static PROPERTIES OUTPUT_NAME static/FastRFS)
	target_link_libraries(FastRFS_static phylonaut phylokit )
	set_target_properties(FastRFS_static PROPERTIES LINK_FLAGS "-static" )

	INCLUDE_DIRECTORIES("boost/" )
endif()
