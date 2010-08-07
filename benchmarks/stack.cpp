/*
 * stack.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <benchmark.hpp>
#include <details/stack.hpp>

using namespace coroutine::details;

BENCH(nothing, 10000000)
{
}

BENCH(createStaticStack, 10000000)
{
	UNUSED Stack<stack::Static> stack;
}

BENCH(createDynamicStack, 10000000)
{
	UNUSED Stack<stack::Dynamic> stack;
}

BENCH_MAIN()
