cmake_minimum_required(VERSION 4.3.1)
project(mt)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SFML COMPONENTS Graphics Window System REQUIRED)

add_executable(${PROJECT_NAME} UtilsTest.cpp)

message(STATUS "Type: ${CMAKE_BUILD_TYPE}")

