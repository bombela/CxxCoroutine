/*
 * benchmark.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <benchmark/timer.hpp>
#include <list>
#include <functional>
#include <algorithm>

namespace benchmark {

class Bench;

namespace details {
	
	std::list<Bench*> benchs;

} // namespace details

class Bench
{
	public:
		Bench() { details::benchs.push_back(this); }
		virtual ~Bench() {}
		virtual void do_bench() const = 0;
};

void runalls()
{
	std::for_each(details::benchs.begin(), details::benchs.end(),
			std::mem_fun(&Bench::do_bench));
}

} // namespace benchmark

#define BENCH(name, cntpower) \
struct bench_##name: benchmark::Bench { \
 \
	void do_bench() const { \
		unsigned cnt = 1u << cntpower; \
	   	benchmark::Timer t(#name, cnt); \
		for (;cnt;cnt--) bench(); }\
	void bench() const; \
} bench_instance_##name; \
void bench_##name::bench() const

#define BENCH_MAIN() \
int main() { \
	benchmark::runalls(); \
	return 0; \
}

#define UNUSED __attribute__ ((unused))

#endif /* BENCHMARK_H */
