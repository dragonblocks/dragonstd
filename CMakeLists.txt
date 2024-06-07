cmake_minimum_required(VERSION 3.14)
project(Dragonstd)

add_compile_options(
	-Wall
	-Wextra
)

add_library(dragonstd
	dragonstd/array.c
	dragonstd/flag.c
	dragonstd/map.c
	dragonstd/list.c
	dragonstd/queue.c
	dragonstd/refcount.c
	dragonstd/tree.c
	dragonstd/bits/compare.c
)

target_link_libraries(dragonstd
	pthread
)

target_include_directories(dragonstd
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
)
