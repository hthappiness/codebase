#编译arm版本时，cmake命令
#cmake -DCMAKE_TOOLCHAIN_FILE=arm.cmake ..

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

option(_ARM_ "hi3559 compile" ON)
add_definitions(-D_ARM_)

#指向其它头文件和链接库的目录
set(ThirdPartyIncludeDir ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/include )
set(ThirdPartyLibDir ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/lib )

set(CMAKE_FIND_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/arm64/)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGEAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)