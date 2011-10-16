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
					static_cast<IMPL*>(this)->enter();
					return *_rv;
				}

			protected:
				void bootstrap()
				{
					yield_final(static_cast<IMPL*>(this)->_func(
								yielder<RV (FV)>(&yield_trampoline, this),
								*_fv));
					abort();
				}


			private:
				RV* _rv;
				FV* _fv;

				static FV yield_trampoline(void* self, RV value) {
					return reinterpret_cast<coroutine_base*>(self)
						->yield(value);
				}

				FV yield(RV value)
				{
					_rv = &value;
					static_cast<IMPL*>(this)->leave();
					return *_fv;
				}

				void yield_final(RV value)
				{
					_rv = &value;
					static_cast<IMPL*>(this)->leave_final();
				}
		};

	// RV f()
	template <typename RV, typename IMPL>
		class coroutine_base<RV (), IMPL>
		{
			public:
				RV operator ()()
				{
					static_cast<IMPL*>(this)->enter();
					return *_rv;
				}

			protected:
				void bootstrap()
				{
					yield_final(static_cast<IMPL*>(this)->_func(
								yielder<RV ()>(&yield_trampoline, this)
								));
				}

			private:
				RV* _rv;

				static void yield_trampoline(void* self, RV value) {
					reinterpret_cast<coroutine_base*>(self)->yield(value);
				}

				void yield(RV value)
				{
					_rv = &value;
					static_cast<IMPL*>(this)->leave();
				}

				void yield_final(RV value)
				{
					_rv = &value;
					static_cast<IMPL*>(this)->leave_final();
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
					static_cast<IMPL*>(this)->enter();
				}

			protected:
				void bootstrap()
				{
					static_cast<IMPL*>(this)->_func(
							yielder<void (FV)>(&yield_trampoline, this),
							*_fv);
					yield_final();
				}

			private:
				FV* _fv;

				static FV yield_trampoline(void* self) {
					return reinterpret_cast<coroutine_base*>(self)->yield();
				}

				FV yield()
				{
					static_cast<IMPL*>(this)->leave();
					return *_fv;
				}

				void yield_final()
				{
					static_cast<IMPL*>(this)->leave_final();
				}
		};

	// void f()
	template <typename IMPL>
		class coroutine_base<void (), IMPL>
		{
			public:
				void operator ()()
				{
					static_cast<IMPL*>(this)->enter();
				}

			protected:
				void bootstrap()
				{
					static_cast<IMPL*>(this)->_func(
							yielder<void ()>(&yield_trampoline, this));
					yield_final();
				}

			private:

				static void yield_trampoline(void* self) {
					reinterpret_cast<coroutine_base*>(self)->yield();
				}

				void yield()
				{
					static_cast<IMPL*>(this)->leave();
				}

				void yield_final()
				{
					static_cast<IMPL*>(this)->leave_final();
				}
		};

	template <typename S, typename F, typename CONTEXT>
		class coroutine: public coroutine_base<S, coroutine<S, F, CONTEXT> > {
			friend class coroutine_base<S, coroutine<S, F, CONTEXT> >;
			typedef coroutine_base<S, coroutine<S, F, CONTEXT> > base_t;
			typedef F       func_t;
			typedef CONTEXT context_t;

		public:
			enum state_t { INITIALIZED, RUNNING, TERMINATED };

			coroutine(func_t f):
				_context(&bootstrap_trampoline, this),
				_func(f),
				_exception(nullptr),
				_state(INITIALIZED)
			{}

			coroutine(const coroutine& from) = delete;
			coroutine& operator=(coroutine& from) = delete;
			coroutine& operator=(coroutine&& from) = delete;

			// TODO handle correctly move semantic
			// only if the coroutine can be moved
			// without any trouble.
			coroutine(coroutine&& from):
				_context(std::move(from._context)),
				_func(std::move(from._func)),
				_exception(nullptr),
				_state(INITIALIZED)
			{
				if (from._state == RUNNING)
					// TODO in fact you can if the stack do not move.
					// lets find a way to handle this edge case.
					throw std::runtime_error("cannot move running coroutine");
			};

			operator bool() const {
				return _state != TERMINATED;
			}

		private:
			context_t          _context;
			func_t             _func;
			std::exception_ptr _exception;
			state_t            _state;

			static void bootstrap_trampoline(void* self) {
					reinterpret_cast<coroutine*>(self)
						->bootstrap();
			}

			void bootstrap() {
				_state = RUNNING;
				try {
					this->base_t::bootstrap();
				} catch(...) {
					_exception = std::current_exception();
					this->leave_final();
				}
			}

			void enter() {
				if (_state == TERMINATED)
					throw std::runtime_error("terminated coroutine");
				_context.enter();
				// throw if caught any exception inside the coroutine.
				if (_exception)
					std::rethrow_exception(_exception);
			}
			void leave() {
				_context.leave();
			}
			void leave_final() {
				_state = TERMINATED;
				_context.leave();
			}

			// TODO integrate some similar check
			// to prevent moving if context
			// is not moveable after being started.
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
