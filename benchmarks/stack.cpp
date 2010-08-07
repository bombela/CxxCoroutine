/*
 * stack.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <benchmark.hpp>
#include <details/stack.hpp>

using namespace coroutine::details;

BENCH(nothing, 24)
{
}

BENCH(createStaticStack, 24)
{
	UNUSED Stack<stack::Static> stack;
}

BENCH(createDynamicStack, 24)
{
	UNUSED Stack<stack::Dynamic> stack;
}

BENCH_MAIN()
