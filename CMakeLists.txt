cmake_minimum_required (VERSION 3.3)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(memory_pool CXX)

if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

file(GLOB_RECURSE SRC_ALL
	"memory/*.h*"
	"memory/*.c*"

	"tests/*.c*"
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)

foreach(FILE ${SRC_ALL})
	# Get the directory of the source file
	get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
		
	# Remove common directory prefix to make the group
	string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")

	# Make sure we are using windows slashes
	string(REPLACE "/" "\\" GROUP "${GROUP}")

	#include_directories(${PARENT_DIR})

	source_group("${GROUP}" FILES "${FILE}")
endforeach()

add_executable(memory_pool ${SRC_ALL})
