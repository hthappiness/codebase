
set(demoService
	
	test_service.cpp
	)
	
add_executable(demo_service ${demoService})

target_link_libraries(demo_service PUBLIC pthread
glog
leveldb
)

message("include dir:" ${ThirdPartyIncludeDir})
message("library dir:" ${ThirdPartyLibDir})
message(${CMAKE_CURRENT_SOURCE_DIR}/../libdf)

include_directories(${ThirdPartyIncludeDir} ${CMAKE_CURRENT_SOURCE_DIR}/../ ${DataFlowLibPath})
link_directories(${ThirdPartyLibDir})