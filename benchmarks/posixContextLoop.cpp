/*
 * posixContext.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <benchmark.hpp>
#include <details/context_posix.hpp>

using namespace coroutine::details;
using namespace coroutine::details::posix;

#define BENCH_ARCH posix
#define BENCH_ARCH_POSIX
#include "context_loop.cpp.inc"

BENCH_MAIN(loop)
