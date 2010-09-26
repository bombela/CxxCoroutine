#
# CMakeLists.txt
# Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
#
#

cmake_minimum_required(VERSION 2.6)

project(zia CXX)

###############################################################################
# BOOST CONFIGURATION
###############################################################################

set(Boost_USE_STATIC_LIBS   OFF)
set(Boost_USE_MULTITHREADED ON)
set(BOOST_INCLUDEDIR "/usr/local/include")
set(Boost_ADDITIONAL_VERSIONS 1.44 1.43 1.42 1.41 1.40 1.39 1.38)
add_definitions(-DBOOST_ALL_NO_LIB)

###############################################################################
# COMPILER FLAGS
###############################################################################

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_COMPILER_IS_GNUCXX)
	set(PROFILING false CACHE BOOL "Use profilling")
	if (PROFILING)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
	endif()
	
	set(EXTRADEBUG false CACHE BOOL "Force extra debug info")
	
	set(NATIVECODE false CACHE BOOL "Generate native code")
	if (NATIVECODE)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
	endif()
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
	
	if (EXTRADEBUG)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")
	endif()
	
	set(OMIT_FRAME_POINTER true CACHE BOOL "Omit frame pointer")
	if (OMIT_FRAME_POINTER)
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fomit-frame-pointer")
	endif()
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
	
	if (EXTRADEBUG)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -D_GLIBCXX_DEBUG")
	endif()
	
	include(CheckCXXCompilerFlag OPTIONAL RESULT_VARIABLE CMAKE_CHECK_CXX_FLAGS)
	if (CMAKE_CHECK_CXX_FLAGS)
		CHECK_CXX_COMPILER_FLAG(-flto GCC_WITH_LTO)
		if (GCC_WITH_LTO)
			set(LTO true CACHE BOOL "Use GCC Link Time Optimisation.")
		endif(GCC_WITH_LTO)
		CHECK_CXX_COMPILER_FLAG(-fwhole-program GCC_WITH_WP)
		if (GCC_WITH_WP)
			set(LTO_WP false CACHE BOOL
				"Use GCC Whole Program Link Time Optimisation.")
		endif(GCC_WITH_WP)
	endif(CMAKE_CHECK_CXX_FLAGS)
	
	if (LTO)
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto")
		set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS_RELEASE} -flto")
	endif()
	
	if (LTO_WP)
		set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fwhole-program")
		set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS_RELEASE}
			-fwhole-program")
	endif()
endif(CMAKE_COMPILER_IS_GNUCXX)

###############################################################################
# COROUTINE CONFIGURATION
###############################################################################
include_directories(${CMAKE_SOURCE_DIR}/include)

set(LINUX_8664_BOOTSTRAP_STACK true CACHE BOOL
	"Linux x86_64 custom asm: trampoline boostraped by stack rather than registers")
if (LINUX_8664_BOOTSTRAP_STACK)
	add_definitions(-DCORO_LINUX_8664_BOOTSTRAP_STACK)
endif()

set(LINUX_8664_2SWAPSITE true CACHE BOOL
	"Linux x86_64: swapContext copy pasted for help CPU prediction")
if (LINUX_8664_2SWAPSITE)
	add_definitions(-DCORO_LINUX_8664_2SWAPSITE)
endif()

###############################################################################
# SUBDIRS
###############################################################################

enable_testing()
add_subdirectory(tests)
add_subdirectory(benchmarks)
