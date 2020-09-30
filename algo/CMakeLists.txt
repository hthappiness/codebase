
cmake_minimum_required(VERSION 3.3)

if(POLICY CMP0063)
	cmake_policy(SET CMP0063 NEW)
 endif()

project("algo learning"
	#VERSION 0.0.1
	LANGUAGES CXX C
)

# does this system provide the log and exp functions?
include (${CMAKE_ROOT}/Modules/CheckFunctionExists.cmake)

check_function_exists (log HAVE_LOG)
check_function_exists (exp HAVE_EXP)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

#set(CMAKE_CXX_VISIBILITY_PRESET hidden)
#只导出动态库中特定的符号，则采用以下策略
#（1）修改CMakeList.txt，添加如下属性，从而使所有符号默认不导出。

# set(CMAKE_CXX_VISIBILITY_PRESET hidden)
# set(CMAKE_C_VISIBILITY_PRESET hidden)
# 1
# 2
# （2）设置visibility属性，导出so的关键符号

# void __attribute__ ((visibility ("default"))) fun();

message("${CMAKE_CURRENT_SOURCE_DIR}")
message(${PROJECT_SOURCE_DIR})

SET(CMAKE_C_COMPILER)
add_compile_options(-std=c++11 ) 
#SET(CMAKE_C_STANDARD 11)

#link_directories( ${CMAKE_CURRENT_SOURCE_DIR})

#find_package(CURL)

#if(CURL_FOUND)
#    target_include_directories(clib PRIVATE ${CURL_INCLUDE_DIR})
#    target_link_libraries(curltest ${CURL_LIBRARY})
#else(CURL_FOUND)
#    message(FATAL_ERROR ”CURL library not found”)
#endif(CURL_FOUND)

#set(ThirdPartyIncludeDir ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/include )
#set(ThirdPartyLibDir ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/lib )

#link_directories(${ThirdPartyLibDir} ${CMAKE_CURRENT_SOURCE_DIR}/driver/lib_x86 )

#include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../${ThirdPartyIncludeDir})

add_executable(main main.cpp)

add_subdirectory(common)
add_subdirectory(dp)
add_subdirectory(ds)
add_subdirectory(sort)

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

#link_lirary public module interface

#跨文件