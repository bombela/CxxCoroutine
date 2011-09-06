/*
 * stack.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <test.hpp>
#include <coroutine/stack.hpp>
#include <coroutine/impl/stack_static.hpp>
#include <coroutine/impl/stack_dynamic.hpp>

using namespace coroutine;

template <typename S>
void validateStack(S& s)
{
	char* data = s.get_stack_ptr();
	for (size_t i = 0; i < s.get_size(); ++i)
		data[i] = (char)i;
	for (size_t i = 0; i < s.get_size(); ++i)
		BOOST_CHECK_EQUAL(data[i], (char)i);
}

BOOST_AUTO_TEST_CASE(test_dynamic_create)
{
	{
		stack::stack<stack::dynamic, 2048> a;
		BOOST_CHECK_EQUAL(a.get_size(), 2048);
		validateStack(a);
	}

	{
		stack::stack<stack::dynamic, stack::size_in_b<1024>::value > a;
		BOOST_CHECK_EQUAL(a.get_size(), 1024);
	}

	{
		stack::stack<stack::dynamic, stack::size_in_kb<5>::value > a;
		BOOST_CHECK_EQUAL(a.get_size(), 1024 * 5);
		validateStack(a);
	}

	{
		stack::stack<stack::dynamic, stack::size_in_mb<1>::value > a;
		BOOST_CHECK_EQUAL(a.get_size(), 1024 * 1024);
	}
}

BOOST_AUTO_TEST_CASE(test_dynamic_move)
{
	{
		typedef stack::stack<stack::dynamic, 2048> stack_t;
		stack_t a;

		char* data = a.get_stack_ptr();
		for (size_t i = 0; i < a.get_size(); ++i)
			data[i] = (char)i;

		char* old_addr = a.get_stack_ptr();

		stack_t b = std::move(a);

		BOOST_CHECK_EQUAL(a.get_size(), 2048);
		BOOST_CHECK_EQUAL((void*)a.get_stack_ptr(), (void*)0);

		BOOST_CHECK_EQUAL(b.get_size(), 2048);
		BOOST_CHECK_EQUAL((void*)b.get_stack_ptr(), (void*)old_addr);

		data = b.get_stack_ptr();
		for (size_t i = 0; i < b.get_size(); ++i)
			BOOST_CHECK_EQUAL(data[i], (char)i);
	}
}

BOOST_AUTO_TEST_CASE(test_static_)
{
	{
		stack::stack<stack::static_, 2048> a;
		BOOST_CHECK_EQUAL(a.get_size(), 2048);
		validateStack(a);
	}

	{
		stack::stack<stack::static_, stack::size_in_b<1024>::value > a;
		BOOST_CHECK_EQUAL(a.get_size(), 1024);
	}

	{
		stack::stack<stack::static_, stack::size_in_kb<5>::value > a;
		BOOST_CHECK_EQUAL(a.get_size(), 1024 * 5);
		validateStack(a);
	}

	{
		stack::stack<stack::static_, stack::size_in_mb<1>::value > a;
		BOOST_CHECK_EQUAL(a.get_size(), 1024 * 1024);
	}
}

BOOST_AUTO_TEST_CASE(test_static__move)
{
	{
		typedef stack::stack<stack::static_, 2048> stack_t;
		stack_t a;

		char* data = a.get_stack_ptr();
		for (size_t i = 0; i < a.get_size(); ++i)
			data[i] = (char)i;

		char* old_addr = a.get_stack_ptr();

		stack_t b = std::move(a);

		BOOST_CHECK_EQUAL(a.get_size(), 2048);
		BOOST_CHECK_EQUAL((void*)a.get_stack_ptr(), (void*)old_addr);

		BOOST_CHECK_EQUAL(b.get_size(), 2048);
		BOOST_CHECK_NE((void*)b.get_stack_ptr(), (void*)old_addr);

		data = b.get_stack_ptr();
		int same = 0;
		for (size_t i = 0; i < b.get_size(); ++i)
			data[i] == (char)i && ++same;

		BOOST_CHECK_NE(same, b.get_size());
	}
}

BOOST_AUTO_TEST_CASE(test_dynamic_really_moveable)
{
	typedef stack::stack<stack::dynamic, 2048> stack_t;
	size_t v = stack::is_really_moveable<stack_t>::value;
	BOOST_CHECK_EQUAL(v, true);
}

BOOST_AUTO_TEST_CASE(test_static__not_really_moveable)
{
	typedef stack::stack<stack::static_, 2048> stack_t;
	size_t v = stack::is_really_moveable<stack_t>::value;
	BOOST_CHECK_EQUAL(v, false);
}
