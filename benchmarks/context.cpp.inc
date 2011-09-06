/* vim: ft=cpp
 * posixContext.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <coroutine/impl/stack_static.hpp>
#include <coroutine/impl/stack_dynamic.hpp>

#ifndef BENCH_TYPE

#define BENCH_NAME(n) BENCH_CAT(n, BENCH_CAT(BENCH_TYPE, BENCH_CAT(_, BENCH_ARCH)))

static const size_t stack_size = 2 * 1024 * 1024;

#define BENCH_TYPE _static
#define STACK stack::static_
#include "context.cpp.inc"
#undef STACK
#undef BENCH_TYPE

#define BENCH_TYPE _dynamic
#define STACK stack::dynamic
#include "context.cpp.inc"
#undef STACK
#undef BENCH_TYPE

#else

#include <benchmark.hpp>

using namespace coroutine;

namespace BENCH_TYPE {

void function(void*) { }

BENCH(BENCH_NAME(createContext), 23, BENCH_CAT(createStack, BENCH_TYPE))
{
	typedef stack::stack<STACK, stack_size> stack_t;
	typedef context::context<
	context::resolve_alias<context_tag>::type
	, stack_t> context_t;

	UNUSED context_t context(&function, 0);
}

BENCH(BENCH_NAME(runContext), 23, BENCH_NAME(createContext))
{
	typedef stack::stack<STACK, stack_size> stack_t;
	typedef context::context<
	context::resolve_alias<context_tag>::type
	, stack_t> context_t;

	UNUSED context_t context(&function, 0);
	context.enter();
}

} // namespace

#endif
