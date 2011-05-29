/*
 * functor_signature.cpp
 * Copyright © 2011 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <test.hpp>
#include <details/functor_signature.hpp>

#include<iostream>

using namespace coroutine;
using namespace coroutine::details;

template <typename A, typename B>
struct same_type { static const bool value = false; };

template <typename T>
struct same_type<T, T> { static const bool value = true; };

struct take_void_return_void_f {
	void operator()(Yielder<>) const {}
};

typedef MemberSignature<take_void_return_void_f,
		&take_void_return_void_f::operator()> lol_t;

BOOST_AUTO_TEST_CASE(take_void_return_void)
{
	BOOST_CHECK((same_type<
			FunctorSignature<take_void_return_void_f>::rv_t, void
			>::value));
	BOOST_CHECK((same_type<
			FunctorSignature<take_void_return_void_f>::fv_t, void
			>::value));
}

struct take_void_return_int_f {
	int operator()(Yielder<int>) const { return 42; }
};

BOOST_AUTO_TEST_CASE(take_void_return_int)
{
	BOOST_CHECK((same_type<
			FunctorSignature<take_void_return_int_f>::rv_t, int
			>::value));
	BOOST_CHECK((same_type<
			FunctorSignature<take_void_return_int_f>::fv_t, void
			>::value));
}

struct take_float_return_void_f {
	void operator()(Yielder<void, float>, float) const {}
};

BOOST_AUTO_TEST_CASE(take_float_return_void)
{
	BOOST_CHECK((same_type<
			FunctorSignature<take_float_return_void_f>::rv_t, void
			>::value));
	BOOST_CHECK((same_type<
			FunctorSignature<take_float_return_void_f>::fv_t, float 
			>::value));
}

struct take_long_return_double_f {
	double operator()(Yielder<double, long>, long) const { return 2.2; }
};

BOOST_AUTO_TEST_CASE(take_long_return_double)
{
	BOOST_CHECK((same_type<
			FunctorSignature<take_long_return_double_f>::rv_t, double
			>::value));
	BOOST_CHECK((same_type<
			FunctorSignature<take_long_return_double_f>::fv_t, long
			>::value));
}
