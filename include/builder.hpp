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
namespace builder {

using coroutine::Yielder;

namespace details {

	using coroutine::details::func_type;

	template <typename F>
	struct FunctorSign {};

} // namespace details

template <typename F>
struct Coro
{
};

template <typename RV, typename FV>
struct Coro<RV (FV)>:
	public Coroutine<RV, FV, typename details::func_type<RV, FV>::type >
{
	typedef 
		Coroutine<RV, FV, typename details::func_type<RV, FV>::type >
		base_t;

	Coro(typename details::func_type<RV, FV>::type f):
   		base_t(f) {}
};

template <typename RV>
struct Coro<RV ()>:
	public Coroutine<RV, void, typename details::func_type<RV, void>::type >
{
	typedef 
		Coroutine<RV, void, typename details::func_type<RV, void>::type >
		base_t;

	Coro(typename details::func_type<RV, void>::type f):
   		base_t(f) {}
};

template <size_t SSIZE>
struct StackSize
{
};

} // namespace builder
} // namespace coroutine

#endif /* BUILDER_H */
