
cmake_minimum_required(VERSION 2.8)

project("c++ learning"
	VERSION 0.0.1
	LANGUAGES CXX C
)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_VISIBILITY_PRESET hidden)

message("${CMAKE_CURRENT_SOURCE_DIR}")

SET(CMAKE_C_COMPILER)
add_compile_options(-std=c++11)

#set(ThirdPartyIncludeDir ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/include )
#set(ThirdPartyLibDir ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/lib )

#link_directories(${ThirdPartyLibDir} ${CMAKE_CURRENT_SOURCE_DIR}/driver/lib_x86 )

#include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../${ThirdPartyIncludeDir})

add_executable(cv cv.cpp)

#单文件构建目标
#扫描文件夹下的cpp文件，生成src，然后build

#cmake 内置宏 ： ${PROJECT_SOURCE_DIR}   

#关键字
#option  option (USE_MYMATH "Use tutorial provided math implementation" ON)
#marco endmarco
#install

#command
#add_executable
#add_custom_command
#include_directories
#add_library 

#跨文件