/*
 * createCoroutine.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <test.hpp>
#include <coroutine/builder.hpp>
#include <iostream>
#include <coroutine/impl/context_posix.hpp>
#include <coroutine/stack.hpp>

using namespace coroutine;

void f_trow_int(yielder<void ()> yield)
{
	std::cout << "I am gonna throw an int!!!" << std::endl;
	throw 42;
}

BOOST_AUTO_TEST_CASE(throw_int)
{
	auto c = corof(&f_trow_int);

	std::cout << "before call" << std::endl;
	BOOST_REQUIRE_THROW( c(), int );
	std::cout << "after call " << std::endl;
}

BOOST_AUTO_TEST_CASE(throw_already_terminated_with_throw)
{
	auto c = corof(&f_trow_int);

	std::cout << "before call" << std::endl;
	BOOST_REQUIRE_THROW( c(), int );
	std::cout << "after call " << std::endl;
	BOOST_REQUIRE_THROW( c(), std::runtime_error );
}

void f_nothing(yielder<void ()> yield)
{
	std::cout << "Hi from a coroutine" << std::endl;
}

BOOST_AUTO_TEST_CASE(throw_already_terminated)
{
	auto c = corof(&f_nothing);

	c();
	BOOST_REQUIRE_THROW( c(), std::runtime_error );
}

int f_little_loop(yielder<int ()> yield)
{
	int next = 0;
	int end = 25;
	while (true) {
		auto v = next;
		++next;
		if (next == end)
			return v;
		yield(v);
	}
}

BOOST_AUTO_TEST_CASE(check_loop_and_stop)
{
	auto c = corof(&f_little_loop);

	for (int i = 0; i < 25; ++i)
		BOOST_CHECK(c() == i);
	BOOST_REQUIRE_THROW( c(), std::runtime_error );
}

BOOST_AUTO_TEST_CASE(check_terminated_status)
{
	auto c = corof(&f_little_loop);

	int i = 0;
	while (c)
		BOOST_CHECK(c() == i++);
	BOOST_REQUIRE_THROW( c(), std::runtime_error );
}

