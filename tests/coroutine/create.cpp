/*
 * createCoroutine.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <test.hpp>
#include <coroutine.hpp>
#include <details/context_posix.hpp>

using namespace coroutine;

void function(Yielder<int>& yield) {}

BOOST_AUTO_TEST_CASE(defvoid)
{
	Coroutine<void> coro(&function);
}

BOOST_AUTO_TEST_CASE(defint)
{
	Coroutine<int> coro(&function);
}

BOOST_AUTO_TEST_CASE(customStack)
{
	Coroutine<int, Context<stack::Dynamic> > coro(&function);
}

BOOST_AUTO_TEST_CASE(customStackSize)
{
	Coroutine<int, Context<stack::Static, 2048> > coro(&function);
}

BOOST_AUTO_TEST_CASE(customContext)
{
	Coroutine<int, details::posix::Context<> > coro(&function);
}
