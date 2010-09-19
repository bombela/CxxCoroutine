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

void runalls(const char* pkg)
{
	std::for_each(details::benchs.begin(), details::benchs.end(),
			std::bind2nd(std::mem_fun(&Bench::do_bench), pkg));
}

} // namespace benchmark

#define BENCH_STR(str) #str
#define BENCH_CAT_I(a, b) a##b
#define BENCH_CAT(a, b) BENCH_CAT_I(a, b)

#define BENCH(name, cntpower, ...) \
struct BENCH_CAT(bench_, name): benchmark::Bench { \
 \
	static const char* relatedTo(const char* rt) { return *rt ? rt : "nop"; } \
	void do_bench(const char* pkg) const { \
		unsigned cnt = 1u << cntpower; \
	   	benchmark::Timer t(pkg, BENCH_STR(name), \
				relatedTo(BENCH_STR(__VA_ARGS__)), cnt);\
		for (;cnt;cnt--) bench_impl(); }\
	void bench_impl() const; \
} BENCH_CAT(bench_instance_, name); \
void BENCH_CAT(bench_, name)::bench_impl() const

#define BENCH_MAIN(pkg) \
BENCH(nop, 24) {} \
int main() { \
	benchmark::runalls(#pkg); \
	return 0; \
}

#define UNUSED __attribute__ ((unused))

#endif /* BENCHMARK_H */
