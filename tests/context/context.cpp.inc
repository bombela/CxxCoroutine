/* vim: ft=cpp
 * context.inc.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <test.hpp>
#include <iostream>

using namespace coroutine;

void function(void*) {}

BOOST_AUTO_TEST_CASE(create)
{
	Context<> context(&function, 0);
}

BOOST_AUTO_TEST_CASE(createWithSpecificStackSize)
{
    Context< stack::Default<1024> > context(&function, 0);
}

struct TestExecution
{
	bool executed;
	TestExecution(): executed(false) { }
	virtual void operator()() { executed = true; }

	static void dotest(void* te) {
		(*(TestExecution*)(te))();
	}
};

BOOST_AUTO_TEST_CASE(execution)
{
	TestExecution f;
    Context<> context(&TestExecution::dotest, &f);
	context.enter();
	BOOST_CHECK(f.executed);
}

template <typename Context>
struct TestYield: TestExecution
{
    Context context;
	TestYield(): context(&TestYield::dotest, this) {}
	void operator()()
	{
		TestExecution::operator()();
		context.leave();
		BOOST_ERROR("yield passed");
	}
};

BOOST_AUTO_TEST_CASE(yield)
{
	TestYield< Context<> > test;
	test.context.enter();
	BOOST_CHECK(test.executed);
}

BOOST_AUTO_TEST_CASE(yieldWithDynamicStack)
{
	TestYield< Context< stack::Dynamic<> > > test;
	test.context.enter();
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
    Context< stack::Dynamic< 1024*41> > context(&LotOfStack::dotest, &test);
	context.enter();
	BOOST_CHECK(test.executed);
}

BOOST_AUTO_TEST_CASE(copyContext)
{
	TestExecution tester;

	Context< stack::Static<> > a(&TestExecution::dotest, &tester);
	tester.executed = false;
	a.enter();
	BOOST_CHECK(tester.executed);
	
	Context< stack::Static<> > b = a;
	tester.executed = false;
	b.enter();
	BOOST_CHECK(tester.executed);

	a = b;
	tester.executed = false;
	a.enter();
	BOOST_CHECK(tester.executed);
}
