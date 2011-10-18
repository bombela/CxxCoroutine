/*
 * stack.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <benchmark.hpp>
#include <coroutine/stack.hpp>
#include <coroutine/builder.hpp>
#include <coroutine/stack.hpp>
#include <iostream>

using namespace coroutine;

const long int LOOP_SIZE = 1L << 27;

void empty_coro(yielder<void ()> yield) {
	for (long int i = 0; i < LOOP_SIZE; ++i)
		yield();
	abort();
}

__attribute__((noinline)) void dothebench() {
	auto c = corof(empty_coro);
	for (long long int i = 0; i < LOOP_SIZE; ++i)
		c();
}

BENCH(coroutine, 3)
{
	dothebench();
}


__attribute__((noinline)) void dothebench2() {
	auto c = corof(empty_coro);
	for (long long int i = 0; i < LOOP_SIZE; ++i)
		asm("nop");
}

BENCH(simpleloop, 3)
{
	dothebench2();
}

BENCH_MAIN(fullbench)
