cmake_minimum_required(VERSION 3.13)
project(Example)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(../modelAPI modelAPI_build)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} modelAPI::modelAPI)