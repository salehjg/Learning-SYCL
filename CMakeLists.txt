cmake_minimum_required(VERSION 3.23.5 FATAL_ERROR)

set(CMAKE_C_COMPILER icx)
set(CMAKE_CXX_COMPILER icpx)

project(LearningSYCL LANGUAGES CXX)
find_package(IntelSYCL REQUIRED)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Building in DEBUG mode.")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-O3")
endif()

add_subdirectory(examples/00.helloworld.cmake)
add_subdirectory(examples/01.queues)