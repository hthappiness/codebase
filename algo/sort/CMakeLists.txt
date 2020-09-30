

set(SORT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE FILEPATH "include " )
include_directories(${SORT_INCLUDE_DIR} ./)

add_compile_options(-fPIC) #位置无关代码
set(sortLibSrc merge_sort.cpp)
add_library(sort SHARED ${sortLibSrc})
#add_library(sort STATIC ${sortLibSrc})  #默认生成静态库

#STATIC， 代表静态链接库，编译的时候link到工程中；
#SHARED，代表动态链接库，运行时候加载；
#MODULE，是一些插件，运行时候使用dlopen-like的功能进行动态加载；

#添加这个目标之后，则不需要link_directories
add_library(sort_lib INTERFACE)
target_link_libraries(sort_lib INTERFACE sort)
target_compile_options(sort_lib INTERFACE -fPIE) #位置无关的可执行文件

list(APPEND EXTRA_LIBS sort)

# list(LENGTH <list><output variable>)
#     list(GET <list> <elementindex> [<element index> ...]<output variable>)
#     list(APPEND <list><element> [<element> ...])
#     list(FIND <list> <value><output variable>)
#     list(INSERT <list><element_index> <element> [<element> ...])
#     list(REMOVE_ITEM <list> <value>[<value> ...])
#     list(REMOVE_AT <list><index> [<index> ...])
#     list(REMOVE_DUPLICATES <list>)
#     list(REVERSE <list>)
#     list(SORT <list>)

#for test
set(THRIFT_SRCS ${PROJECT_SOURCE_DIR}/sort/shell_sort.cpp )

#处理依赖关系
add_custom_command(
	OUTPUT ${THRIFT_SRCS}
    #COMMAND thrift --gen cpp ${PROJECT_SOURCE_DIR}/thrift/my_server.thrift -out ${PROJECT_SOURCE_DIR}/thrift
	#COMMAND rm -f {PRJJECT_SOURCE_DIR}/thrift/xxx.skeleton.cpp
	COMMAND echo "test cwc"
    #DEPENDS ${PROJECT_SOURCE_DIR}/thrift/my_server.thrift
    COMMENT "Now Generating thrift files..."
)

message("--------------------" ${SORT_INCLUDE_DIR}/../build/sort)
#这里相当于gcc的-L起的作用，指示动态库存在的路径；对于外部的动态库，不是该cmake工程生成的对象，需要指定链接路径
#link_directories(${SORT_INCLUDE_DIR}/../build/sort)

set(sortSrc ${THRIFT_SRCS})

add_executable(shell_sort ${sortSrc})
message("--------------------" ${SORT_INCLUDE_DIR})


#这里分为cmake本身管理的对象，以及加载的外部的库
#target_link_libraries(shell_sort PUBLIC sort_lib)
#target_link_libraries(shell_sort PUBLIC -lsort)
target_link_libraries(shell_sort PUBLIC sort)

target_include_directories(shell_sort PUBLIC
						   "${PROJECT_BINARY_DIR}"
						   "${SORT_INCLUDE_DIR}"
		)

		
		# 如果add_custom_command和add_executable在不同的CMakeLists.txt，
		# 那么add_executable那个CMakeLists.txt无法引用到add_custom_command，
		# 也就是说，最好add_custom_command和add_executable在同一个CMakeLists.txt中