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

#if 0
int f_ret_feed(yielder<int (float)> yield, float v)
{
	BOOST_CHECK(v == 2.2f);
	std::cout << "v=" << v << std::endl;
	float a = yield(42);
	BOOST_CHECK(a == 99.f);
	std::cout << "a=" << a << std::endl;
	return 84;
}

BOOST_AUTO_TEST_CASE(ret_feed)
{
	auto c = corof<stack::dynamic, context::posix>(&f_ret_feed);

	int r1 = c(2.2f);
	BOOST_CHECK_EQUAL(r1, 42);
	std::cout << "r1=" << r1 << std::endl;
	int r2 = c(99.f);
	BOOST_CHECK(r2 == 84);
	std::cout << "r2=" << r2 << std::endl;
}
#endif

struct Speaker {
	const char* _msg;
	Speaker(const char* msg): _msg(msg) {}
	Speaker(const Speaker& from): _msg(from._msg) {
		std::cout << "copy: " << msg() << std::endl;
	}
	Speaker(Speaker&& from): _msg(from._msg) {
		from._msg = "wiped";
		std::cout << "move: " << msg() << std::endl;
	}
	const char* msg() const { return _msg; }
};

int f_elide_cpy(yielder<int (int)> yield, int v)
{
	std::cout << "f_elide_cpy " << v << std::endl;
	return 10;
}

Speaker f_elide_cpyu(yielder<Speaker (int)> yield, int)
{
//    yield(Speaker("yielded 1"));
//    yield(Speaker("yielded 2"));
	return Speaker("returned");
}

BOOST_AUTO_TEST_CASE(elide_cpy)
{
	auto c = corof<stack::dynamic, context::posix, stack::size_in_mb<64> >(&f_elide_cpy);

//    yielder<int (float)> a(( float (*)(void*, int) )reinterpret_cast<void*>(42), reinterpret_cast<void*>(21));
//    auto b = a;

	auto a = c(42);
//    std::cout << a.msg() << std::endl;
//    auto b = c(42);
//    std::cout << b.msg() << std::endl;
//    auto d = c(42);
//    std::cout << d.msg() << std::endl;
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
