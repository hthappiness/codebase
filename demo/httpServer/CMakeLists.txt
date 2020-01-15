
cmake_minimum_required(VERSION 3.0)

project("httpServer"
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

set(3rdBaseDir /home/hting)

set(ThirdPartyIncludeDir ${3rdBaseDir}/3rdparty/include )
set(ThirdPartyLibDir ${3rdBaseDir}/3rdparty/lib )

link_directories(${ThirdPartyLibDir} )

include_directories( ${ThirdPartyIncludeDir} ${CMAKE_CURRENT_SOURCE_DIR}/../${ThirdPartyIncludeDir})

#add_subdirectory(src)

FILE (GLOB ALL_SOURCES "*.cpp" "*.c" )

set(httpSrc 
	${ALL_SOURCES}
)

add_executable(httpServer ${httpSrc})

target_link_libraries(httpServer PUBLIC
${CMAKE_DL_LIBS}
boost_filesystem
boost_thread
boost_system
pthread
)