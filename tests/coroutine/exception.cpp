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
	auto c = corof<stack::dynamic, context::posix,
		 stack::size_in_mb<8> >(&f_trow_int);

	c();
}


#if 0
int f_ret(yielder<int> yield)
{
	yield(43);
	return 85;
}

BOOST_AUTO_TEST_CASE(ret)
{
	typedef Coroutine<int, void, int (*)(yielder<int>),
		1024*8, stack::Default, context> coro_t;

	coro_t c(&f_ret);

	int r1 = c();
	BOOST_CHECK(r1 == 43);
	std::cout << "r1=" << r1 << std::endl;
	int r2 = c();
	BOOST_CHECK(r2 == 85);
	std::cout << "r2=" << r2 << std::endl;
}

void f_feed(yielder<void, float> yield, float v)
{
	BOOST_CHECK(v == 2.1f);
	std::cout << "v=" << v << std::endl;
	float a = yield();
	BOOST_CHECK(a == 98.f);
	std::cout << "a=" << a << std::endl;
}

BOOST_AUTO_TEST_CASE(feed)
{
	typedef Coroutine<void, float, void (*)(yielder<void, float>, float),
		1024*8, stack::Default, context> coro_t;

	coro_t c(&f_feed);

	c(2.1f);
	c(98.f);
}

// FUNCTOR

struct f_ret_feed_functor {

	int operator()(yielder<int, float> yield, float v)
	{
		BOOST_CHECK(v == 2.2f);
		std::cout << "v=" << v << std::endl;
		float a = yield(42);
		BOOST_CHECK(a == 99.f);
		std::cout << "a=" << a << std::endl;
		return 84;
	}

};

BOOST_AUTO_TEST_CASE(ret_feed_functor)
{
	typedef Coroutine<int, float, f_ret_feed_functor,
		1024*8, stack::Default, context> coro_t;

	coro_t c((f_ret_feed_functor())); // copy

	int r1 = c(2.2f);
	BOOST_CHECK(r1 == 42);
	std::cout << "r1=" << r1 << std::endl;
	int r2 = c(99.f);
	BOOST_CHECK(r2 == 84);
	std::cout << "r2=" << r2 << std::endl;
}

struct f_ret_functor {

	int operator()(yielder<int> yield)
	{
		yield(43);
		return 85;
	}

};

BOOST_AUTO_TEST_CASE(ret_functor)
{
	typedef Coroutine<int, void, f_ret_functor,
		1024*8, stack::Default, context> coro_t;

	f_ret_functor f;
	coro_t c(f); // copy

	int r1 = c();
	BOOST_CHECK(r1 == 43);
	std::cout << "r1=" << r1 << std::endl;
	int r2 = c();
	BOOST_CHECK(r2 == 85);
	std::cout << "r2=" << r2 << std::endl;
}

struct f_feed_functor {

	void operator()(yielder<void, float> yield, float v) const
	{
		BOOST_CHECK(v == 2.1f);
		std::cout << "v=" << v << std::endl;
		float a = yield();
		BOOST_CHECK(a == 98.f);
		std::cout << "a=" << a << std::endl;
	}

};

BOOST_AUTO_TEST_CASE(feed_functor)
{
	typedef Coroutine<void, float, const f_feed_functor&,
		1024*8, stack::Default, context> coro_t;

	f_feed_functor f;
	coro_t c(f); // const ref

	c(2.1f);
	c(98.f);
}

#endif
