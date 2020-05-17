
find_package(Threads REQUIRED)
add_library(basic_process INTERFACE )
#target_compile_options(basic_process INTERFACE -Wall -Wextra -Werror)
# target_compile_features(basic_process INTERFACE cxx_std_11)

target_compile_options(basic_process INTERFACE -Wall -Werror)
add_definitions(-std=c++11)
target_link_libraries(basic_process INTERFACE Threads::Threads)

set(dfsrc 
	dataFlow.cpp
	df_exception.cpp
	)

add_library(dataFlow SHARED ${dfsrc})
target_compile_definitions(dataFlow PUBLIC BOOST_THREAD_VERSION=4)
target_include_directories(dataFlow PUBLIC
./
${}
)

target_link_libraries(dataFlow PUBLIC
${CMAKE_DL_LIBS}
boost_filesystem
boost_thread
boost_system
glog
gflags
basic_process)

#add_library(dataFlow_app INTERFACE )
#add_library(dataFlow_plugin INTERFACE )

#target_link_libraries(dataFlow_app INTERFACE dataFlow)
#target_link_libraries(dataFlow_plugin INTERFACE dataFlow)