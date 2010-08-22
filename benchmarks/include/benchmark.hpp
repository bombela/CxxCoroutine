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
		virtual void do_bench(const char* pkg) const = 0;
};

void runalls(const char* pkg = "other")
{
	std::for_each(details::benchs.begin(), details::benchs.end(),
			std::bind2nd(std::mem_fun(&Bench::do_bench), pkg));
}

} // namespace benchmark

#define BENCH(name, cntpower) \
struct bench_##name: benchmark::Bench { \
 \
	void do_bench(const char* pkg) const { \
		unsigned cnt = 1u << cntpower; \
	   	benchmark::Timer t(pkg, #name, cnt); \
		for (;cnt;cnt--) bench(); }\
	void bench() const; \
} bench_instance_##name; \
void bench_##name::bench() const

#define BENCH_MAIN(pkg) \
BENCH(nop, 24) {} \
int main() { \
	benchmark::runalls(#pkg); \
	return 0; \
}

#define UNUSED __attribute__ ((unused))

#endif /* BENCHMARK_H */
