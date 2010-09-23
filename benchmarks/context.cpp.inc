/*
 * posixContext.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#ifndef BENCH_TYPE

#define BENCH_NAME(n) BENCH_CAT(n, BENCH_CAT(BENCH_TYPE, BENCH_CAT(_, BENCH_ARCH)))

#define BENCH_TYPE _static
#define STACK stack::Static
#include "context.cpp.inc"
#undef STACK
#undef BENCH_TYPE

#define BENCH_TYPE _dynamic
#define STACK stack::Dynamic
#include "context.cpp.inc"
#undef STACK
#undef BENCH_TYPE

#else

#include <benchmark.hpp>

namespace BENCH_TYPE {

void function() { }

BENCH(BENCH_NAME(createContext), 23, BENCH_CAT(createStack, BENCH_TYPE))
{
	UNUSED Context<STACK> context(function);
}

BENCH(BENCH_NAME(runContext), 23, BENCH_NAME(createContext))
{
	UNUSED Context<STACK> context(function);
	context.run();
}

} // namespace 

#endif
