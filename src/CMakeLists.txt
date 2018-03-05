cmake_minimum_required (VERSION 2.6)
project (FastRFS)

file(GLOB SOURCES *.cpp)
if(WIN32)
add_definitions(/std:c++14)
INCLUDE_DIRECTORIES("C:/Users/pranjal/src/phylokit/install/x64-Release/include")
INCLUDE_DIRECTORIES("C:/Users/pranjal/src/phylonaut/install/x64-Release/include")

if(WIN32)
LINK_DIRECTORIES("C:/Users/pranjal/src/phylokit/install/x64-Release/lib/")

LINK_DIRECTORIES("C:/Users/pranjal/src/phylonaut/install/x64-Release/lib/")
endif()

else()
add_definitions(-std=c++14 -g -O3 -Wall)
endif()

add_executable(FastRFS ${SOURCES} FastRFS.cpp)
target_link_libraries(FastRFS phylokit phylonaut)

INCLUDE_DIRECTORIES("boost/" )
install(TARGETS FastRFS 
		RUNTIME DESTINATION bin)
