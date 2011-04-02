#
# CMakeLists.txt
# Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
#
#

find_package(Boost 1.38 REQUIRED system)
include_directories(${Boost_INCLUDE_DIRS})
link_libraries(${Boost_LIBRARIES})

add_custom_target(bench)

add_custom_target(cleanbench
	COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_CURRENT_BINARY_DIR}/bench.log
	VERBATIM
	)

add_custom_target(stat
	${CMAKE_CURRENT_SOURCE_DIR}/stats.py ${CMAKE_CURRENT_BINARY_DIR}/bench.log
	)

macro(coroutine_add_bench src)
	get_filename_component(name ${src} NAME_WE)
	set(name "bench_${name}")
	add_executable(${name} ${src})
	set(runname "${name}.run")
	add_custom_target(${runname}
		echo "Benchmarking ${name}..."
		COMMAND ${name}
		VERBATIM
		)
	add_dependencies(bench ${runname})
endmacro()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB BENCH_SRCS "*.cpp")
#foreach(f ${BENCH_SRCS})
#	coroutine_add_bench(${f})
#endforeach()
