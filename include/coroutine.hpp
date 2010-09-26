/*
 * include/coroutine.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef COROUTINE_H
#define COROUTINE_H

#include <context.hpp>
#include <yielder.hpp>

namespace coroutine {

template <typename T, class C = Context<> >
		 /*
		 template <size_t> class StackImpl = stack::Static,
		 size_t STACK_SIZE = coroutine::stack::DEFAULT_SIZE,
		 template <template <size_t > class, size_t> class C = Context>*/
class Coroutine
{
	public:
		typedef C context_t;

		template <typename F>
		Coroutine(F f)
		{
		}
		
	private:
};

} // namespace coroutine

#endif /* COROUTINE_H */
