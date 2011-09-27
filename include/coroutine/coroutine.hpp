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
//                    std::cout << "() fv " << (void*)_fv << std::endl;
//                    std::cout << "() rv " << (void*)_rv << std::endl;
					IMPL* impl = static_cast<IMPL*>(this);
//                    std::cout << "() fv " << (void*)_fv << std::endl;
//                    std::cout << "() rv " << (void*)_rv << std::endl;
//                    std::cout << "enter" << std::endl;
					impl->_context.enter();
//                    std::cout << "leave" << std::endl;
//                    std::cout << "() fv " << (void*)_fv << std::endl;
//                    std::cout << "() rv " << (void*)_rv << std::endl;
					FV* lol = _fv;
//                    std::cout << "() fv " << (void*)_fv << std::endl;
//                    std::cout << "() rv " << (void*)_rv << std::endl;
					return *_rv;
				}

			protected:
				static void bootstrap_trampoline(void* self) {
//                    std::cout << "bootstrap_trampoline this " << (void*)self << std::endl;
//                    std::cout << "bootstrap_trampoline fv" << (void*)
//                        reinterpret_cast<coroutine_base*>(self)->_fv
//                        << std::endl;
					IMPL* impl = static_cast<IMPL*>(self);
//                    std::cout << "bootstrap func " << (void*)impl->_func << std::endl;
					reinterpret_cast<coroutine_base*>(self)->bootstrap();
				}

			private:
				RV* _rv;
				FV* _fv;

				void bootstrap()
				{
					yielder<RV (FV)> yielder(&yield_trampoline, this);
					yield(static_cast<IMPL*>(this)->_func(yielder, *_fv));

//                    std::cout << "bootstrap this " << (void*)this << std::endl;
//                    yielder<RV (FV)> yielder(&yield_trampoline, this);
//                    std::cout << "bootstrap " << (void*)_fv << std::endl;
//                    IMPL* impl = static_cast<IMPL*>(this);
//                    std::cout << "bootstrap impl this " << (void*)impl << std::endl;
//                    std::cout << "bootstrap func " << (void*)impl->_func << std::endl;
//                    impl->_func(yielder, 23);
//                    std::cout << "yield" << std::endl;
//                    yield(42);
//                    std::cout << "abort!" << std::endl;
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
//                    std::cout << "yield() " << (void*)impl << std::endl;
//                    std::cout << "suspens" << std::endl;
					impl->_context.leave();
//                    std::cout << "passed!!!!" << std::endl;
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
					yielder<RV ()> yielder(&yield_trampoline, this);
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
					yielder<void (FV)> yielder(&yield_trampoline, this);
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
					yielder<void ()> yielder(&yield_trampoline, this);
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
			friend class coroutine_base<S, coroutine<S, F, CONTEXT> >;
			typedef coroutine_base<S, coroutine<S, F, CONTEXT> > base_t;
			typedef F       func_t;
			typedef CONTEXT context_t;

		public:
			coroutine(func_t f):
				_context(&base_t::bootstrap_trampoline, this),
				_func(f)
			{}

			coroutine(const coroutine& from) = delete;
			coroutine& operator=(coroutine& from) = delete;
			coroutine& operator=(coroutine&& from) = delete;

			coroutine(coroutine&& from):
				_context(std::move(from._context)),
				_func(std::move(from._func))
			{};

		private:
			context_t _context;
			func_t    _func;

			context_t&& move_context_or_throw(context_t&& c)
			{
				if (not stack::is_really_moveable<
						typename context::get_args<context_t>::stack_t
						>::value)
					std::cout << "CACA" << std::endl;
				return std::move(c);
			}
		};

} // namespace coroutine

#endif /* COROUTINE_H */
