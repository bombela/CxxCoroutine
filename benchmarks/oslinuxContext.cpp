/*
 * oslinuxContext.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <benchmark.hpp>
#include <details/context_oslinux.hpp>

using namespace coroutine::details;
using namespace coroutine::details::oslinux;

#define _BENCH(n, p) BENCH(oslinux_##n, p)
#include "context.cpp.inc"

BENCH_MAIN()
