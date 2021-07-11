
set(demoSrc 
	json11.cpp
	test.cpp
	)

#SET(CMAKE_C_COMPILER)
add_compile_options(-std=c++11)

add_executable(test ${demoSrc})

target_link_libraries(test PRIVATE 
rt
pthread
)