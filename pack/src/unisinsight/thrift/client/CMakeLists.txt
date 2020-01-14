option (USE_MYMATH  
        "Use tutorial provided math implementation" ON)

set(Thrift_LIBRARIES )
INCLUDE_DIRECTORIES(/home/hting/boost/include)

set(Client_src thrift_client.cpp)

add_executable(client ${Client_src})
TARGET_LINK_LIBRARIES(client ${Thrift_LIBRARIES})