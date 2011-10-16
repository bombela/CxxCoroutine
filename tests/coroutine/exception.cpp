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

	std::cout << "before call" << std::endl;
	BOOST_REQUIRE_THROW( c(), int );
	std::cout << "after call " << std::endl;
}

BOOST_AUTO_TEST_CASE(throw_already_terminated)
{
	auto c = corof<stack::dynamic, context::posix,
		 stack::size_in_mb<8> >(&f_trow_int);

	std::cout << "before call" << std::endl;
	BOOST_REQUIRE_THROW( c(), int );
	std::cout << "after call " << std::endl;
	BOOST_REQUIRE_THROW( c(), std::runtime_error );
}

