/*
 * stack.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <benchmark.hpp>
#include <stack.hpp>

using namespace coroutine;

BENCH(createStack_static, 23)
{
	UNUSED Stack<stack::Static> stack;
}

BENCH(createStack_dynamic, 23)
{
	UNUSED Stack<stack::Dynamic> stack;
}

BENCH_MAIN(fullbench)
