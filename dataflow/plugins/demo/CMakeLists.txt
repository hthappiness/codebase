
set(demoSrc 
	demo.cpp
	)

add_library(demo MODULE ${demoSrc})

target_link_libraries(demo PRIVATE dataflow
rt
uv
pthread
)