
cmake_minimum_required(VERSION 3.6)

project("osd testing"
	VERSION 3.6.2
	LANGUAGES CXX C
)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_VISIBILITY_PRESET hidden)

message("${CMAKE_CURRENT_SOURCE_DIR}")

SET(CMAKE_C_COMPILER)
add_compile_options(-std=c++11)

set( curLibraryDir  /home/hting/3rdparty)

set(ThirdPartyIncludeDir ${curLibraryDir}/include )
set(ThirdPartyLibDir ${curLibraryDir}/lib )

link_directories(${ThirdPartyLibDir}  )

include_directories( ${ThirdPartyIncludeDir} ${ThirdPartyIncludeDir}/freetype2)

find_package(OpenCV REQUIRED)

set (osdSrc
	CvOsd.cpp
	CvxText.cpp
	)

add_executable(osdTest ${osdSrc})

target_link_libraries(osdTest freetype ${OpenCV_LIBS})

#cmake + pkg-config
#先设置环境变量PKG_CONFIG_PATH，以调用该命令
#set(ENV{PKG_CONFIG_PATH} /seastar/build/release)
#找包
#find_package(PkgConfig)
#
#pkg_search_module(SEASTARPC REQUIRED seastar)
#