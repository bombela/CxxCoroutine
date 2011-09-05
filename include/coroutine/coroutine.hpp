/*
 * coroutine.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef COROUTINE_H
#define COROUTINE_H

#include <coroutine/context.hpp>
#include <coroutine/yielder.hpp>


#include<iostream>

namespace coroutine {

	template <typename S, typename IMPL>
		class coroutine_base;

	// RV f(FV feedValue)
	template <typename RV, typename FV, typename IMPL>
		class coroutine_base<RV (FV), IMPL>
		{
			public:
				RV operator ()(FV fval)
				{
					_fv = &fval;
					IMPL* impl = static_cast<IMPL*>(this);
					impl->_context.enter();
					return *_rv;
				}

			protected:
				static void bootstrap_trampoline(void* self) {
					reinterpret_cast<coroutine_base*>(self)->bootstrap();
				}

			private:
				RV* _rv;
				FV* _fv;

				void bootstrap()
				{
					yielder<RV, FV> yielder(&yield_trampoline, this);
					yield(static_cast<IMPL*>(this)->_func(yielder, *_fv));
					abort();
				}

				static FV yield_trampoline(void* self, RV value) {
					return reinterpret_cast<coroutine_base*>(self)
						->yield(value);
				}

				FV yield(RV value)
				{
					_rv = &value;
					IMPL* impl = static_cast<IMPL*>(this);
					impl->_context.leave();
					return *_fv;
				}
		};

	// RV f()
	template <typename RV, typename IMPL>
		class coroutine_base<RV (), IMPL>
		{
			public:
				RV operator ()()
				{
					static_cast<IMPL*>(this)->_context.enter();
					return *_rv;
				}

			protected:
				static void bootstrap_trampoline(void* self) {
					reinterpret_cast<coroutine_base*>(self)->bootstrap();
				}

			private:
				RV* _rv;

				void bootstrap()
				{
					yielder<RV, void> yielder(&yield_trampoline, this);
					yield(static_cast<IMPL*>(this)->_func(yielder));
					abort();
				}

				static void yield_trampoline(void* self, RV value) {
					reinterpret_cast<coroutine_base*>(self)->yield(value);
				}

				void yield(RV value)
				{
					_rv = &value;
					static_cast<IMPL*>(this)->_context.leave();
				}
		};

	// void f(FV feedValue)
	template <typename FV, typename IMPL>
		class coroutine_base<void (FV), IMPL>
		{
			public:
				void operator ()(FV fval)
				{
					_fv = &fval;
					static_cast<IMPL*>(this)->_context.enter();
				}

			protected:
				static void bootstrap_trampoline(void* self) {
					reinterpret_cast<coroutine_base*>(self)->bootstrap();
				}

			private:
				FV* _fv;

				void bootstrap()
				{
					yielder<void, FV> yielder(&yield_trampoline, this);
					static_cast<IMPL*>(this)->_func(yielder, *_fv);
					yield();
					abort();
				}

				static FV yield_trampoline(void* self) {
					return reinterpret_cast<coroutine_base*>(self)->yield();
				}

				FV yield()
				{
					static_cast<IMPL*>(this)->_context.leave();
					return *_fv;
				}
		};

	// void f()
	template <typename IMPL>
		class coroutine_base<void (), IMPL>
		{
			public:
				void operator ()()
				{
					static_cast<IMPL*>(this)->_context.enter();
				}

			protected:
				static void bootstrap_trampoline(void* self) {
					reinterpret_cast<coroutine_base*>(self)->bootstrap();
				}

			private:

				void bootstrap()
				{
					yielder<void, void> yielder(&yield_trampoline, this);
					static_cast<IMPL*>(this)->_func(yielder);
					yield();
					abort();
				}

				static void yield_trampoline(void* self) {
					reinterpret_cast<coroutine_base*>(self)->yield();
				}

				void yield()
				{
					static_cast<IMPL*>(this)->_context.leave();
				}
		};

	template <typename S, typename F, typename CONTEXT>
		class coroutine: public coroutine_base<S, coroutine<S, F, CONTEXT> > {
			typedef coroutine_base<S, coroutine<S, F, CONTEXT> > base_t;
			typedef F       func_t;
			typedef CONTEXT context_t;
			friend base_t;

			public:
			coroutine(func_t f):
				_context(&base_t::bootstrap_trampoline, this),
				_func(f)
			{}

			private:
			context_t _context;
			func_t    _func;
		};

} // namespace coroutine

#endif /* COROUTINE_H */
