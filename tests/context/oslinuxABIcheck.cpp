/*
 * oslinuxABIcheck.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <test.hpp>
#include <coroutine/impl/context_linux.hpp>

using namespace coroutine;

static const size_t stack_size = 8 * 1024 * 1024;
typedef stack::stack<stack::dynamic, stack_size> stack_t;
typedef context::context<
	context::resolve_alias<context::linux>::type
	, stack_t> context_t;

template <size_t ARCH_SIZE>
struct ABIConsistencyCheck;

template <>
struct ABIConsistencyCheck<8>
{
	static void check()
	{
		asm (
				"push $42\n\t"
				"mov $0x42, %%rax\n\t"
				"mov $0x43, %%rbx\n\t"
				"mov $0x44, %%rcx\n\t"
				"mov $0x45, %%rdx\n\t"
				"mov $0x46, %%r8\n\t"
				"mov $0x47, %%r9\n\t"
				"mov $0x48, %%r10\n\t"
				"mov $0x49, %%r11\n\t"
				"mov $0x4a, %%r12\n\t"
				"mov $0x4b, %%r13\n\t"
				"mov $0x4c, %%r14\n\t"
				"mov $0x4d, %%r15\n\t"
				"mov $0x4e, %%rsi\n\t"
				"mov $0x4f, %%rdi\n\t"
				"pop %%rax\n\t"
				: // output
				// not used
				: // input
				// not used
				: // modified
				"rax", "rbx", "rcx", "rdx", "rdi", "rsi",
				"r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"
				);
	}
};

template <>
struct ABIConsistencyCheck<4>
{
	static void check()
	{
		asm (
				"push $42\n\t"
				"mov $0x42, %%eax\n\t"
				"mov $0x43, %%ebx\n\t"
				"mov $0x44, %%ecx\n\t"
				"mov $0x45, %%edx\n\t"
				"mov $0x4e, %%esi\n\t"
				"mov $0x4f, %%edi\n\t"
				"mov $0x50, %%ebp\n\t"
				"pop %%eax\n\t"
				: // output
				// not used
				: // input
				// not used
				: // modified
				"eax", "ebx", "ecx", "edx", "edi", "esi", "ebp"
				);
	}
};

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
		ABIConsistencyCheck<sizeof (void*)>::check();
		context.leave();
		BOOST_ERROR("yield passed");
	}
};

BOOST_AUTO_TEST_CASE(checkABIconsistency)
{
	for (int i = 0; i < 500; ++i)
	{
		TestYield<context_t> test;
		test.context.enter();
		BOOST_CHECK(test.executed);
	}
}
