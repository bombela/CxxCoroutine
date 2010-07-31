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
	Context<> context(function);
}

BOOST_AUTO_TEST_CASE(createWithSpecificStackSize)
{
    Context<1024> context(function);
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
	TestExecution(): executed(false) {}
	void operator()() { executed = true; }
};

BOOST_AUTO_TEST_CASE(execution)
{
	TestExecution f;
    Context<> context(f);
	context.run();
	BOOST_CHECK(f.executed);
}

struct TestYield: TestExecution
{
    Context<> context;
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
	TestYield test;
	test.context.run();
	BOOST_CHECK(test.executed);
}
