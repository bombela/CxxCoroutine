/*
 * copyContext.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <test.hpp>
#include <details/context_oslinux.hpp>
#include <details/context_posix.hpp>


using namespace coroutine::details;

template <typename Context>
struct ContextTester
{
	bool executed;
	Context* context;
	ContextTester():
		executed(false) {}
	
	void operator()()
	{
		executed = true;
		context->yield();
		BOOST_ERROR("yield passed");
	}

	void test(Context& c)
	{
		context = &c;
		context->run();
		BOOST_CHECK(executed);
	}
};

BOOST_AUTO_TEST_CASE(copyPosixContext)
{
	ContextTester< posix::Context<stack::Static> > tester;

	posix::Context<stack::Static> a(&tester);
	tester.test(a);
	
	posix::Context<stack::Static> b = a;
	tester.test(b);

	a = b;
	tester.test(a);
}

BOOST_AUTO_TEST_CASE(copyOsLinuxContext)
{
	ContextTester< oslinux::Context<stack::Static> > tester;

	oslinux::Context<stack::Static> a(&tester);
	tester.test(a);

	oslinux::Context<stack::Static> b = a;
	tester.test(b);

	a = b;
	tester.test(a);
}
