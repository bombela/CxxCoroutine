#
# CMakeLists.txt
# Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
#
#

set(TEST_SRCS
	yielder.cpp
	coroutine.cpp
	builder.cpp
)

foreach(f ${TEST_SRCS})
	coroutine_add_test(${f})
endforeach()
