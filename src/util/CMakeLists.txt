
find_package(Threads REQUIRED)
add_library(basic_process INTERFACE)

target_compile_options(basic_process INTERFACE -Wall -Wextra -Werror)
target_compile_features(basic_process INTERFACE cxx_std_11)
target_link_libraries(basic_process INTERFACE Threads::Threads)

set (LibUtilSrc

)

add_library(util SHARED ${libUtilSrc})
target_compile_definitions(util PUBLIC BOOST_THREAD_VERSION=4)
target_include_directories(vias PUBLIC
$
)

target_link_libraries(util PUBLIC
${CMAKE_DL_LIBS}
boost_filesystem
boost_thread
boost_system
glog
gflags
basic_process
libcurl.a
)

add_library(util_application INTERFACE)
target_link_libraries(util_application INTERFACE util)
target_compile_options(util_application INTERFACE -fPIE)

add_library(util_plugin INTERFACE)
target_link_libraries(util_plugin INTERFACE util)