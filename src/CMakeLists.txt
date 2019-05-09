cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

project(ModelAPI)

set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

include_directories(../pstreams-1.0.1)

set(SOURCE_EXE main.cpp vgg-face-api.cpp)	

add_executable(main ${SOURCE_EXE})

target_link_libraries(main 
	"-framework Python")