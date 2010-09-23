/*
 * context.inc.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <test.hpp>
#include <iostream>

using namespace coroutine::details;

void function() {}

BOOST_AUTO_TEST_CASE(create)
{
	Context<> context(function);
}

BOOST_AUTO_TEST_CASE(createWithSpecificStackSize)
{
    Context<stack::Static, 1024> context(function);
}

struct Functor
{
	void operator()() { }
};

BOOST_AUTO_TEST_CASE(functor)
{
	Functor f;
    Context<> context(f);
}

struct TestExecution
{
	bool executed;
	TestExecution(): executed(false) { }
	void operator()() { executed = true; }
};

BOOST_AUTO_TEST_CASE(execution)
{
	TestExecution f;
    Context<> context(f);
	context.run();
	BOOST_CHECK(f.executed);
}

template <typename Context>
struct TestYield: TestExecution
{
    Context context;
	TestYield(): context(*this) {}
	void operator()()
	{
		TestExecution::operator()();
		context.yield();
		BOOST_ERROR("yield passed");
	}
};

BOOST_AUTO_TEST_CASE(yield)
{
	TestYield<Context<> > test;
	test.context.run();
	BOOST_CHECK(test.executed);
}

BOOST_AUTO_TEST_CASE(yieldWithDynamicStack)
{
	TestYield<Context<stack::Dynamic> > test;
	test.context.run();
	BOOST_CHECK(test.executed);
}

struct LotOfStack: TestExecution
{
	void operator()() __attribute__ ((noinline))
	{
		TestExecution::operator()();
		char data[1024*40];
		data[0] = 42;
	}
};

BOOST_AUTO_TEST_CASE(uselotofstack)
{
	LotOfStack test;
    Context<stack::Dynamic, 1024*41> context(test);
	context.run();
	BOOST_CHECK(test.executed);
}
