cmake_minimum_required(VERSION 3.13)
project(modelAPI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/bin/g++)

set(MODELAPI_NEEDED_INCLUDES
	external/pstreams-1.0.1/include)

set(PUBLIC_HEADERS
	vgg-face-api.h)

set(RESOURCE_FILES
	vgg-face.py
	face_extractor.py
	deploy.prototxt
	weights.caffemodel
	vgg_face_weights.h5
	)

include_directories(${MODELAPI_NEEDED_INCLUDES})

set(SOURCES
	vgg-face-api.cpp)

if (NOT DEFINED BUILD_FRAMEWORK)
    set(BUILD_FRAMEWORK false)
endif()

if (BUILD_FRAMEWORK)
	ADD_LIBRARY(${PROJECT_NAME} SHARED ${SOURCES} ${PUBLIC_HEADERS} ${RESOURCE_FILES})

	set_target_properties(${PROJECT_NAME} PROPERTIES
			FRAMEWORK TRUE
			MACOSX_FRAMEWORK_IDENTIFIER com.uuuu.${PROJECT_NAME}
			PUBLIC_HEADER ${PUBLIC_HEADERS}
			RESOURCE "${RESOURCE_FILES}"
			)
else()
	ADD_LIBRARY(${PROJECT_NAME} SHARED ${SOURCES})

	target_include_directories(${PROJECT_NAME}
			INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
			INTERFACE $<INSTALL_INTERFACE:include>
			PUBLIC ${MODELAPI_NEEDED_INCLUDES}
			)

	foreach(RESOURCE ${RESOURCE_FILES})
        configure_file(${RESOURCE} ${CMAKE_BINARY_DIR}/${RESOURCE} COPYONLY)
	endforeach()

	add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
endif()