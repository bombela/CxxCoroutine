/*
 * defaultContext.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <test.hpp>
#include <context.hpp>
#include <stack.hpp>
#include <iostream>

using namespace coroutine;

void function(void*) {}

BOOST_AUTO_TEST_CASE(create)
{
	Context<> context(&function, 0);
}

BOOST_AUTO_TEST_CASE(createWithSpecificStackSize)
{
    Context< stack::Static<1024> > context(&function, 0);
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
	TestYield<Context<> > test;
	test.context.enter();
	BOOST_CHECK(test.executed);
}

BOOST_AUTO_TEST_CASE(yieldWithDynamicStack)
{
	TestYield< Context<stack::Dynamic<1024*8> > > test;
	test.context.enter();
	BOOST_CHECK(test.executed);
}

BOOST_AUTO_TEST_CASE(printImpl)
{
	std::cout << "Default implemention selected: "
		<< Context<>::getImplName() << std::endl;
}
