/*
 * virtcall.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <benchmark.hpp>
#include <details/stack.hpp>

namespace {

class Interface
{
	public:
		virtual ~Interface() {}
		virtual void doTest() = 0;
};

class Virtcall: public Interface
{
	public:
		virtual void doTest();
};

void Virtcall::doTest() {}

struct BenchContext
{
	BenchContext(): obj(new Virtcall) {}
	~BenchContext() { delete obj; }

	Interface* obj;
} bench;

} // namespace

BENCH(virtcall, 24)
{
	bench.obj->doTest();
}

BENCH_MAIN(fullbench)
