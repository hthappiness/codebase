
set(clientSrc

	server.cpp
)
	
add_executable(webClient ${clientSrc})

set(serverSrc

	c.cpp
)

add_executable(webServer ${serverSrc})

target_link_libraries(webServer PUBLIC
${CMAKE_DL_LIBS}
boost_filesystem
boost_thread
boost_system
pthread
)

target_link_libraries(webClient PUBLIC
${CMAKE_DL_LIBS}
boost_filesystem
boost_thread
boost_system
pthread
)