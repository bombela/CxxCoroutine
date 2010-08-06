/*
 * stack.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <test.hpp>
#include <details/stack.hpp>

using namespace coroutine::details;

template <typename S>
void validateStack(S& s)
{
	char* data = s.getStack();
	for (size_t i = 0; i < s.getSize(); ++i)
		data[i] = (char)i;
	for (size_t i = 0; i < s.getSize(); ++i)
		BOOST_CHECK_EQUAL(data[i], (char)i);
}

BOOST_AUTO_TEST_CASE(create)
{
	Stack<> a;
	BOOST_CHECK_EQUAL(a.getSize(), 8192);
	validateStack(a);
	
	Stack<stack::Static> b;
	BOOST_CHECK_EQUAL(b.getSize(), 8192);
	validateStack(b);

	Stack<stack::Dynamic> c;
	BOOST_CHECK_EQUAL(c.getSize(), 8192);
	validateStack(c);

	Stack<stack::Dynamic, 2048> d;
	BOOST_CHECK_EQUAL(d.getSize(), 2048);
	validateStack(d);

	Stack<stack::Static, 2048> e;
	BOOST_CHECK_EQUAL(e.getSize(), 2048);
	validateStack(e);
}
