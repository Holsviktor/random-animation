cmake_minimum_required(VERSION 4.1)
project(introproject CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
find_package(OpenCV REQUIRED)

if (NOT OpenCV_FOUND)
	message(FATAL_ERROR "OpenCV not found")
endif()

include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(introproject
	src/main.cpp
	src/noise.cpp
)

target_link_libraries(introproject ${OpenCV_LIBS})
