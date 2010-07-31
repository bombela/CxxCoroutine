/*
 * posixContext.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <test.hpp>
#include <details/context_posix.hpp>

using namespace coroutine::details::posix;

void function() {}

BOOST_AUTO_TEST_CASE(create)
{
	Context<void ()> context(function);
}

BOOST_AUTO_TEST_CASE(createWithSpecificStackSize)
{
    Context<void (), 1024> context(function);
}

struct Functor
{
	void operator()() { }
};

BOOST_AUTO_TEST_CASE(functor)
{
	Functor f;
    Context<Functor> context(f);
}
