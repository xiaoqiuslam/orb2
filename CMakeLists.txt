cmake_minimum_required(VERSION 2.8)

project(orb2)

set(CMAKE_BUILD_TYPE debug)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O3 -march=native ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -march=native -std=c++11")

find_package(OpenCV QUIET)
if(OpenCV_FOUND)
   message(STATUS "Found OpenCV version " "${OpenCV_VERSION_MAJOR}.${OpenCV_VERSION_MINOR}.${OpenCV_VERSION_PATCH}")
else()
   message(FATAL_ERROR "OpenCV not found. Please install OpenCV first.")
endif()

add_executable(stereo_euroc stereo_euroc.cc)
target_link_libraries(stereo_euroc ${OpenCV_LIBS})