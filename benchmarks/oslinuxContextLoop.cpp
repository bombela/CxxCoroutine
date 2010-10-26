/*
 * oslinuxContext.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <benchmark.hpp>
#include <details/context_oslinux.hpp>

using namespace coroutine::details;
using namespace coroutine::details::oslinux;

#define BENCH_ARCH oslinux
#include "context_loop.cpp.inc"

BENCH_MAIN(loop)
