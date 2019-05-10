cmake_minimum_required(VERSION 3.13)
project(modelAPI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/bin/g++)

set(modelAPI_needed_includes
	external/pstreams-1.0.1/include)

include_directories(${modelAPI_needed_includes})

set(SOURCES
	vgg-face-api.cpp)

set(BUILD_SHARED_LIBS false)

ADD_LIBRARY(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
		INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
		INTERFACE $<INSTALL_INTERFACE:include>
		PUBLIC ${modelAPI_needed_includes}
		)

configure_file(face_extractor.py ${CMAKE_BINARY_DIR}/face_extractor.py COPYONLY)
configure_file(vgg-face.py ${CMAKE_BINARY_DIR}/vgg-face.py COPYONLY)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})