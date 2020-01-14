option (USE_MYMATH  
        "Use tutorial provided math implementation" ON)

set(Thrift_LIBRARIES )
INCLUDE_DIRECTORIES(/home/hting/boost/include)

set(Client_src thrift_server.cpp)

add_executable(server ${Client_src})
TARGET_LINK_LIBRARIES(server ${Thrift_LIBRARIES})