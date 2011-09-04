/*
 * builder.hpp
 * Copyright © 2011 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef BUILDER_H
#define BUILDER_H

#include <coroutine.hpp>
#include <yielder.hpp>

namespace coroutine {
namespace details {

struct nil {};

template <typename HEAD, typename TAIL>
struct type_list;

template <template <typename> class P, typename D, typename L>
struct find_type;

template <template <typename> class P, typename D>
struct find_type< P, D, nil > { typedef D type; };

template <typename T, template <typename> class P, typename D, typename TAIL>
struct find_type< P, D, type_list< P<T>, TAIL> >
{
	typedef typename P<T>::type type;
};

template <template <typename> class P, typename D, typename HEAD, typename TAIL>
struct find_type< P, D, type_list<HEAD, TAIL> >:
	find_type< P, D, TAIL > {};

template <typename C, template <C> class P, C D, typename L>
struct find_value;

template <typename C, template <C> class P, C D>
struct find_value< C, P, D, nil >
{
	static const C value = D;
};

template <typename C, template <C> class P, C D, typename HEAD, typename TAIL>
struct extract_value_or_continue:
	find_value< C, P, D, TAIL > {};

template <typename C, C V, template <C> class P, C D, typename TAIL>
struct extract_value_or_continue< C, P, D, P<V>, TAIL >
{
	static const C value = P<V>::value;
};

template <typename C, template <C> class P, C D, typename HEAD, typename TAIL>
struct find_value< C, P, D, type_list<HEAD, TAIL> >:
	extract_value_or_continue< C, P, D, HEAD, TAIL > {};


template <typename C, template <C> class HEAD, typename TAIL>
struct value_list;

template <typename C, typename VL, C D, typename L>
struct find_multiple_value;

template <typename C, template <C> class HEAD, typename TAIL, C D, typename L>
struct find_multiple_value< C, value_list< C, HEAD, TAIL >, D, L >
{
	static const C found_value = find_value<C, HEAD, D, L>::value;
	static const C value = (found_value != D) ? found_value
		: find_multiple_value<C, TAIL, D, L>::value;
};

template <typename C, C D, typename L>
struct find_multiple_value< C, nil, D, L >
{
	static const C value = D;
};

template <typename C, template < template <C> class > class P,
		 template <C> class D, typename L>
struct find_type_ctemplate;

template <typename C, template < template <C> class > class P,
		 template <C> class D>
struct find_type_ctemplate< C, P, D, nil > {
	template <C V>
		struct apply {
			typedef D<V> type;
		};
};

template <typename C, template <C> class T,
		 template < template <C> class > class P,
		 template <C> class D, typename TAIL>
struct find_type_ctemplate< C, P, D, type_list< P<T>, TAIL> >
{
	template <C V>
		struct apply {
			typedef typename P<T>::template apply<V> type;
		};
};

template <typename C, template < template <C> class > class P,
		 template <C> class D, typename HEAD, typename TAIL>
struct find_type_ctemplate< C, P, D, type_list<HEAD, TAIL> >:
	find_type_ctemplate< C, P, D, TAIL > {};

} // namespace details

namespace conf {

template <typename T> struct rval { typedef T type; };
template <typename T> struct fval { typedef T type; };
template <typename T> struct func { typedef T type; };

template <size_t V> struct ssize {
	static const size_t value = V;
};

template <size_t V> struct ssize_k {
	static const size_t value = V * 1024;
};

template <size_t V> struct ssize_m {
	static const size_t value = V * 1024 * 1024;
};

template <template <size_t> class T> struct stack {
	template <size_t SSIZE>
		struct apply { typedef T<SSIZE> type; };
};
template <template <typename> class T> struct context {
	template <typename STACK>
		struct apply { typedef T<STACK> type; };
};

} // namespace conf

template <
	typename P1 = details::nil,
	typename P2 = details::nil,
	typename P3 = details::nil,
	typename P4 = details::nil,
	typename P5 = details::nil,
	typename P6 = details::nil,
	typename P7 = details::nil,
	typename P8 = details::nil,
	typename P9 = details::nil
	>
struct builder
{
	typedef details::type_list<P1,
		details::type_list<P2,
			details::type_list<P3,
				details::type_list<P4,
					details::type_list<P5,
						details::type_list<P6,
							details::type_list<P7,
								details::type_list<P8,
									details::type_list<P9,
										details::nil
											>
										>
									>
								>
							>
						>
					>
				>
			>
		parameters;

	typedef typename
		details::find_type< conf::rval, void, parameters >::type return_t;

	typedef typename
		details::find_type< conf::fval, void, parameters >::type feedval_t;

	typedef typename
		details::find_type< conf::func,
		typename details::func_type<return_t, feedval_t>::type,
		parameters >::type func_t;

	typedef details::value_list<size_t, conf::ssize,
				details::value_list<size_t, conf::ssize_k,
					details::value_list<size_t, conf::ssize_m,
						details::nil
						>
					>
				>
			ssize_params;

	static const size_t ssize =
		details::find_multiple_value< size_t, ssize_params,
		coroutine::stack::default_size, parameters >::value;

	typedef typename
		details::find_type_ctemplate< size_t, conf::stack,
		stack::Default, parameters > stack_meta_t;

	typedef typename stack_meta_t::template apply<1024> stack_t;

	/*typedef typename
		details::find_type_template< conf::context,
		context::Default, parameters >::type context_meta_t;
		*/

	typedef coroutine::Coroutine2<
		return_t, feedval_t, func_t
		> coro;
};

} // namespace coroutine

#endif /* BUILDER_H */
