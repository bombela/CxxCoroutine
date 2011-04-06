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


#include<iostream>

namespace coroutine {

template<
	typename                R     = void,
	typename                T     = void,
	typename                F     = R (*)(Yielder<R, T>, T),
	size_t                  SSIZE = stack::default_size,
	template <size_t> class S     = stack::Default,
	template <class>  class C     = Context
	>
class Coroutine
{
	typedef R          return_t;
	typedef F          func_t;
	typedef S<SSIZE>   stack_t;
	typedef C<stack_t> context_t;

	public:
		Coroutine(func_t f): _context(&execute_trampoline, this), _func(f) {}

		R operator()(T value)
		{
			_send_arg = &value;
			_context.enter();
			return *_back_arg;
		}
		
	private:
		context_t _context;
		func_t    _func;
		T*        _send_arg;
		R*        _back_arg;

		static void execute_trampoline(void* self) {
			((Coroutine*)self)->execute();
		}
	
		static T yield_trampoline(void* self, R value) {
			return ((Coroutine*)self)->yield(value);
		}

		void execute()
		{
			Yielder<R, T> y(&yield_trampoline, this);
			yield(_func(y, *_send_arg));
		}
		
		T yield(R value)
		{
			_back_arg = &value;
			_context.leave();
			return *_send_arg;
		}
};

} // namespace coroutine

#endif /* COROUTINE_H */
