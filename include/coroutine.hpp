/*
 * include/coroutine.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef COROUTINE_H
#define COROUTINE_H

#include <context.hpp>

namespace coroutine {

template <typename T>
class Yielder;

template <typename R,
		 typename F  = void (*)(Yielder<R>&),
		 class C = Context<> >
class Coroutine;

template <typename T>
class Yielder
{
	public:
		//Yielder(): _c(c) {}

		void operator()(T value)
		{
		}

	private:
		//Coroutine& _c;

		Yielder(const Yielder& from); // disabled
		Yielder& operator=(const Yielder& from); // disabled
};

/*
 *
 * Polymorphisme
 * 	vtable
 * 	indirect call
 *
 * Coro: CoroBase
 * 	yield et hop reprise du control
 *
 * Yielder Coro* coro
 * 	-> coro->yield
 *
 * ============================
 *
 * Polymorphisme mano
 * 	pas de vtable
 * 	direct call func contenant inline
 *
 * Coro
 * 		static func(void*)
 * 			static_cast and hop reprise du control
 *
 * Yielder (void* coro, pf(void*) [context static func]) 
 * 	-> pf(coro)
 *
 * CoroBase
 * 	virtual yield() = 0
 * 
 */

template <typename R, typename F, class C>
class Coroutine
{
	public:
		typedef R          result_t;
		typedef C          context_t;
		typedef Yielder<R> yielder_t;

		Coroutine(F f):
			_trampoline(*this),
			_context(&_trampoline),
			_f(f)
		{
		}
		
	private:
		struct Trampoline
		{
			Coroutine& _coroutine;
			Trampoline(Coroutine& c): _coroutine(c) {}
			void operator()() { _coroutine.start(); }
		}          _trampoline;
		context_t  _context;
		F          _f;

		void start()
		{
			//yielder_t yielder;
			//_f(yielder);
		}
};

} // namespace coroutine

#endif /* COROUTINE_H */
