/*
 * stack.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <benchmark.hpp>
#include <coroutine/stack.hpp>
#include <coroutine/impl/stack_static.hpp>
#include <coroutine/impl/stack_dynamic.hpp>

using namespace coroutine;

BENCH(createStack_static, 23)
{
	UNUSED stack::stack<stack::static_, 1024 * 1024 * 2> stack;
}

BENCH(createStack_dynamic, 23)
{
	UNUSED stack::stack<stack::dynamic, 1024 * 1024 * 2> stack;
}

BENCH_MAIN(fullbench)
