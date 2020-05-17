
cmake_minimum_required(VERSION 3.0)

project("dataFlow"
	VERSION 0.0.1
	LANGUAGES CXX C
)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_VISIBILITY_PRESET hidden)

option(BUILD_TEST "build test" OFF)

if(BUILD_TEST)
message("build finish")
endif()

message("${CMAKE_CURRENT_SOURCE_DIR}")

SET(CMAKE_C_COMPILER)
add_compile_options(-std=c++11)

set(DATAFLOW_ROOT_PATH /home/hting)
set(ThirdPartyIncludeDir ${DATAFLOW_ROOT_PATH}/3rdparty/include )
set(ThirdPartyLibDir ${DATAFLOW_ROOT_PATH}/3rdparty/lib )
#set(DataFlowLibPath ${DATAFLOW_ROOT_PATH}/libdf )

link_directories(${ThirdPartyLibDir} ${CMAKE_CURRENT_SOURCE_DIR}/libdf ${DataFlowLibPath})

include_directories( ${ThirdPartyIncludeDir} ${CMAKE_CURRENT_SOURCE_DIR}/libdf)

add_subdirectory(libdf)
add_subdirectory(plugins)
add_subdirectory(service)