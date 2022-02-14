cmake_minimum_required(VERSION 3.0)
project(
	kmap 
	VERSION 1.0 
	DESCRIPTION "MAP for kay/value pair library"
	HOMEPAGE_URL ""
	LANGUAGES C
)

set (CMAKE_C_STANDARD 11)

add_subdirectory(klist)

add_library(kmap STATIC kmap.c)

add_executable(kmap_test test.c)
target_link_libraries(kmap_test kmap klist)
