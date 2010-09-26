/*
 * stack.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <test.hpp>
#include <stack.hpp>

using namespace coroutine;

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
	size_t default_size = stack::DEFAULT_SIZE;

	Stack<> a;
    BOOST_CHECK_EQUAL(a.getSize(), default_size);
	validateStack(a);
	
	Stack<stack::Static> b;
	BOOST_CHECK_EQUAL(b.getSize(), default_size);
	validateStack(b);

	Stack<stack::Dynamic> c;
	BOOST_CHECK_EQUAL(c.getSize(), default_size);
	validateStack(c);

	Stack<stack::Dynamic, 1024> d;
	BOOST_CHECK_EQUAL(d.getSize(), 1024);
	validateStack(d);

	Stack<stack::Static, 2048> e;
	BOOST_CHECK_EQUAL(e.getSize(), 2048);
	validateStack(e);
}
