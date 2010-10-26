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

struct LoopTester
{
	static const long int LOOP_SIZE = 1 << 14;

	typedef Context<STACK> context_t;
	
	context_t* context;
	void setContext(context_t* c) { context = c; }

	void operator()()
	{
		for (int i = 0; i < LOOP_SIZE; ++i)
		context->yield();
	}
};

#ifdef BENCH_ARCH_POSIX
#	define RUN_CONTEXT_LOOP_CNT 10
#else
#	define RUN_CONTEXT_LOOP_CNT 14
#endif

BENCH(BENCH_NAME(runContextLoop), RUN_CONTEXT_LOOP_CNT)
{
	LoopTester functor;
	Context<STACK> context(&functor);
	functor.setContext(&context);

	for (int i = 0; i < LoopTester::LOOP_SIZE; ++i)
		context.run();
}

} // namespace 

#endif