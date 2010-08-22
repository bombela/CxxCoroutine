/*
 * posixContext.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#ifndef BENCH_TYPE

#define BENCH_NAME(n) BENCH_CAT(BENCH_ARCH, BENCH_CAT(BENCH_TYPE, n))

#define BENCH_TYPE _static_
#define CONTEXT Context<stack::Static>
#include "context.cpp.inc"
#undef CONTEXT
#undef BENCH_TYPE

#define BENCH_TYPE _dynamic_
#define CONTEXT Context<stack::Dynamic>
#include "context.cpp.inc"
#undef CONTEXT
#undef BENCH_TYPE

#else

#include <benchmark.hpp>

namespace BENCH_TYPE {

void function() {}

BENCH(BENCH_NAME(createContext), 5)//23)
{
	UNUSED CONTEXT context(function);
}

BENCH(BENCH_NAME(execContext), 5, BENCH_NAME(createContext))//23)
{
	UNUSED CONTEXT context(function);
	context.run();
}

} // namespace 

#endif
