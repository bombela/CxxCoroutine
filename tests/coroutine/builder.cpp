/*
 * builder.cpp
 * Copyright © 2011 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <test.hpp>
#include <coroutine/builder.hpp>
#include <coroutine/impl/context_posix.hpp>

#include<iostream>

using namespace coroutine;

int f(yielder<int ()>) { return 42; }
void f2(yielder<>) { }
struct F { int operator()(yielder<int ()>) const { return 42; } };

BOOST_AUTO_TEST_CASE(try_to_compile)
{
	auto c1 = coro<int ()>(F());
	auto c2 = coro<int ()
		 ,context::posix
		 ,stack::static_
		 >(F());
	auto c3 = coro<int (),
		 stack::size_in_kb<42>
			 >(&f);

	typedef builder<int (), F, stack::size_in_b<42> > coro_info_t;

	BOOST_CHECK(coro_info_t::stack_size == 42);
}

void allvoid(yielder<void ()>) {}

template <typename T>
	void do_extraction(T) {
		typename details::extract_signature<T>::type* a = (void (*)())0;
	}

int alittlefunc(yielder<int (float)>, float) {}

template <typename T>
	void do_extraction2(T) {
		typename details::extract_signature<T>::type* a = (int (*)(float))0;
	}

BOOST_AUTO_TEST_CASE(extract_signature)
{
	do_extraction(&allvoid);
	do_extraction2(&alittlefunc);
}

BOOST_AUTO_TEST_CASE(fcoro)
{
	auto a = corof(&allvoid);
	auto b = corof(&alittlefunc);
}
