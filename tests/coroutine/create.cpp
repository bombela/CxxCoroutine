/*
 * createCoroutine.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <test.hpp>
#include <coroutine.hpp>
#include <details/context_posix.hpp>

using namespace coroutine;

void functionVoid(Yielder<void>&) {}

BOOST_AUTO_TEST_CASE(defvoid)
{
	Coroutine<void> coro(&functionVoid);
}

void function(Yielder<int>&) {}

BOOST_AUTO_TEST_CASE(defint)
{
	Coroutine<int> coro(&function);
}

BOOST_AUTO_TEST_CASE(customStack)
{
	Coroutine<int, void (*)(Yielder<int>&),
		Context<stack::Dynamic> > coro(&function);
}

BOOST_AUTO_TEST_CASE(customStackSize)
{
	Coroutine<int, void (*)(Yielder<int>&),
		Context<stack::Static, 2048> > coro(&function);
}

BOOST_AUTO_TEST_CASE(customContext)
{
	Coroutine<int, void (*)(Yielder<int>&),
		details::posix::Context<> > coro(&function);
}
