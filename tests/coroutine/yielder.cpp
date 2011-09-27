/*
 * yielder.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <test.hpp>
#include <coroutine/yielder.hpp>

using namespace coroutine;

BOOST_AUTO_TEST_CASE(create)
{
	yielder<double (float)> y1(0, 0); // double f(float)
	yielder<int ()> y2(0, 0); // int f()
	yielder<void (long)> y3(0, 0); // void f(long)
	yielder<> y4(0, 0); // void f()
}

int someobj;

void cb_voidcall(void* a)
{
	BOOST_CHECK_EQUAL(a, &someobj);
}

BOOST_AUTO_TEST_CASE(voidcall)
{
	yielder<> y(&cb_voidcall, &someobj);

	y();
}

void cb_argcall(void* a, int b)
{
	BOOST_CHECK_EQUAL(a, &someobj);
	BOOST_CHECK_EQUAL(b, 42);
}

BOOST_AUTO_TEST_CASE(argcall)
{
	yielder<int ()> y(&cb_argcall, &someobj);

	y(42);
}

int cb_voidcall_return(void* a)
{
	BOOST_CHECK_EQUAL(a, &someobj);
	return 99;
}

BOOST_AUTO_TEST_CASE(voidcall_return)
{
    yielder<void (int)> y(&cb_voidcall_return, &someobj);

	BOOST_CHECK_EQUAL(y(), 99);
}

float cb_argcall_return(void* a, int b)
{
	BOOST_CHECK_EQUAL(a, &someobj);
	BOOST_CHECK_EQUAL(b, 45);
	return 98;
}

BOOST_AUTO_TEST_CASE(argcall_return)
{
	yielder<int (float)> y(&cb_argcall_return, &someobj);

	BOOST_CHECK_EQUAL(y(45), 98);
}
