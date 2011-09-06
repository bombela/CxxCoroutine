/* vim: ft=cpp
 * context.inc.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <test.hpp>
#include <iostream>
#include <coroutine/impl/stack_dynamic.hpp>
#include <coroutine/impl/stack_static.hpp>

using namespace coroutine;

static const size_t stack_size = 8 * 1024 * 1024;
typedef stack::stack<stack::dynamic, stack_size> stack_t;
typedef context::context<
	context::resolve_alias<context_tag>::type
	, stack_t> context_t;

void function(void*) {}

BOOST_AUTO_TEST_CASE(test_compile)
{
	context_t context(&function, 0);
	BOOST_CHECK_EQUAL(42, 42);
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

BOOST_AUTO_TEST_CASE(execution_and_yield)
{
	TestYield< context_t > test;
	test.context.enter();
	BOOST_CHECK(test.executed);
}

template <typename Context>
struct LotOfStack: TestYield<Context>
{
	void operator()() __attribute__ ((noinline))
	{
		TestYield<Context>::operator()();
		UNUSED char data[1024*40];
		data[0] = 42;
	}
};

BOOST_AUTO_TEST_CASE(uselotofstack)
{
	LotOfStack<context_t> test;
	test.context.enter();
	BOOST_CHECK(test.executed);
}


template <typename Context>
struct TestYieldInfinite
{
    Context context;
	int cnt;
	TestYieldInfinite(): context(&TestYieldInfinite::dotest, this), cnt(0) {}
	void operator()()
	{
		while (42) {
			++cnt;
			context.leave();
		}
	}
	static void dotest(void* te) {
		(*(TestYieldInfinite*)(te))();
	}
};

BOOST_AUTO_TEST_CASE(enter_leave_few_times)
{
	TestYieldInfinite<context_t> tester;

	for (int i = 0; i < 42; ++i)
	{
		BOOST_CHECK_EQUAL(tester.cnt, i);
        tester.context.enter();
		BOOST_CHECK_EQUAL(tester.cnt, i + 1);
	}
}

BOOST_AUTO_TEST_CASE(move_context_with_dynamic_stack)
{
	TestYieldInfinite<context_t> a;

	BOOST_CHECK(a.cnt == 0);
	a.context.enter();
	BOOST_CHECK(a.cnt == 1);

	context_t b = std::move(a.context);
	b.enter();
	BOOST_CHECK(a.cnt == 2);
}

template <typename Context>
struct TestYieldInfinite2
{
	int cnt;
	Context* volatile used;
	void operator()()
	{
		while (42) {
			++cnt;
			used->leave();
		}
	}
	static void dotest(void* te) {
		(*(TestYieldInfinite2*)(te))();
	}
};
BOOST_AUTO_TEST_CASE(move_context_with_static_stack)
{
	typedef stack::stack<stack::static_, 1024 * 4> stack_t;
	typedef context::context<
	context::resolve_alias<context_tag>::type
	, stack_t> context_t;

	typedef TestYieldInfinite2<context_t> tester_t;
	tester_t tester;
	context_t a(&tester.dotest, &tester);
	tester.cnt = 0;
	tester.used = &a;

	BOOST_CHECK(tester.cnt == 0);
	a.enter();
	BOOST_CHECK(tester.cnt == 1);
	a.reset();
	a.enter();
	BOOST_CHECK(tester.cnt == 2);
	a.enter();
	a.reset();
	a.enter();
	BOOST_CHECK(tester.cnt == 4);

	context_t b = std::move(a);
	b.reset();
	tester.used = &b;
	b.enter();
	BOOST_CHECK(tester.cnt == 5);
	b.enter();
	b.enter();
	BOOST_CHECK(tester.cnt == 7);
}
