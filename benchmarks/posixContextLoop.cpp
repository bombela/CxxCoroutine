/*
 * posixContext.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <benchmark.hpp>
#include <coroutine/impl/context_posix.hpp>

typedef coroutine::context::posix context_tag;

#define BENCH_ARCH posix
#define BENCH_ARCH_POSIX
#include "context_loop.cpp.inc"

BENCH_MAIN(loop)
