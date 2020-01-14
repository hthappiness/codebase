cmake_minimum_required (VERSION 2.6)
project (thriftDemo)

#The version number

set (Design_VERSION_MAJOR 1)
set (Design_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings 
# to the source code
#configure_file (
#	"${PROJECT_SOURCE_DIR}"
#	"${PROJECT_BINARY_DIR}"
#	)
	
#add the binary tree to the search path for include files
#so that we will find the header
#include_directories("${PROJECT_BINARY_DIR}")

# add the executable
#add_executable()

SET(LIB_INCLUDE_DIR "/home/luchuanfeng")
SET(LIB_LINK_DIR "/home/luchunfeng")
LINK_DIRECTORIES(${LIB_LINK_DIR} )

SET(CMAKE_C_COMPIILER)
add_compile_options(-std=c++11)

add_subdirectory(server)
add_subdirectory(client)

#set (EXTRA_LIBS ${EXTRA_LIBS} MathFunctions)
#target_link_libraries (Tutorial  ${EXTRA_LIBS})