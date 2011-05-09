/*
 * createCoroutine.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#define YIELDER_TEST_MODE

#include <test.hpp>
#include <coroutine.hpp>
#include <details/context_posix.hpp>

#include<iostream>

using namespace coroutine;

int f_ret_feed(Yielder<int, float> yield, float v)
{
	std::cout << "v=" << v << std::endl;
	float a = yield(42);
	std::cout << "a=" << a << std::endl;
	return 84;
}

BOOST_AUTO_TEST_CASE(ret_feed)
{
	typedef Coroutine<int, float, int (*)(Yielder<int, float>, float),
		1024*8, stack::Default, Context> coro_t;

	coro_t c(&f_ret_feed);

	int r1 = c(2.2f);
	return;
	std::cout << "r1=" << r1 << std::endl;
	int r2 = c(99.f);
	std::cout << "r2=" << r2 << std::endl;
}

#if 0
int def2_c(Yielder<int, float> yield, float v)
{
	std::cout << "v=" << v << std::endl;
	float a = yield(42);
	std::cout << "a=" << a << std::endl;
	return 84;
}

BOOST_AUTO_TEST_CASE(def2)
{
	Coroutine<int, float, int (*)(Yielder<int, float>, float),
		4096, stack::Default, details::oslinux::Context> c(&def2_c);

	int r1 = c(2.2f);
	std::cout << "r1=" << r1 << std::endl;
	int r2 = c(99.f);
	std::cout << "r2=" << r2 << std::endl;
}
#endif
