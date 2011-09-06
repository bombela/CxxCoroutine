/*
 * oslinuxContext.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <benchmark.hpp>
#include <coroutine/impl/context_linux.hpp>

typedef coroutine::context::linux context_tag;

#define BENCH_ARCH oslinux
#include "context.cpp.inc"

BENCH_MAIN(fullbench)
