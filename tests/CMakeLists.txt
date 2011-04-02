#
# CMakeLists.txt
# Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
#
#

find_package(Boost 1.38 REQUIRED system unit_test_framework test_exec_monitor)
include_directories(${Boost_INCLUDE_DIRS})
link_libraries(${Boost_LIBRARIES})

macro(coroutine_add_test src)
	get_filename_component(name ${src} NAME_WE)
	set(name "test_${name}")
	add_executable(${name} ${src})
	add_test(${name} ${name})
endmacro()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(stack)
add_subdirectory(context)
add_subdirectory(coroutine)
