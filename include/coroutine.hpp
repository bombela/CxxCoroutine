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

// RV f(FV feedValue)
template <typename RV, typename FV, typename IMPL>
class CoroutineBase
{
	public:
		RV operator ()(FV fval)
		{
			_fv = &fval;
			static_cast<IMPL>(this)->_context.enter();
			return *_rv;
		}

	protected:
		static void bootstrap_trampoline(void* self) {
			reinterpret_cast<CoroutineBase*>(self)->bootstrap();
		}
		
	private:
		RV* _rv;
		FV* _fv;

		void bootstrap()
		{
			Yielder<RV, FV> yielder(&yield_trampoline, this);
			yield(static_cast<IMPL>(this)->_func(yielder, *_fv));
			abort();
		}
	
		static FV yield_trampoline(void* self, RV value) {
			return reinterpret_cast<CoroutineBase*>(self)->yield(value);
		}
		
		FV yield(RV value)
		{
			_rv = &value;
			static_cast<IMPL>(this)->_context.leave();
			return *_fv;
		}
};

// RV f()
template <typename RV, typename IMPL>
class CoroutineBase<RV, void, IMPL>
{
	public:
		RV operator ()()
		{
			static_cast<IMPL>(this)->_context.enter();
			return *_rv;
		}

	protected:
		static void bootstrap_trampoline(void* self) {
			reinterpret_cast<CoroutineBase*>(self)->bootstrap();
		}
		
	private:
		RV* _rv;

		void bootstrap()
		{
			Yielder<RV, void> yielder(&yield_trampoline, this);
			yield(static_cast<IMPL>(this)->_func(yielder));
			abort();
		}
	
		static void yield_trampoline(void* self, RV value) {
			reinterpret_cast<CoroutineBase*>(self)->yield(value);
		}
		
		void yield(RV value)
		{
			_rv = &value;
			static_cast<IMPL>(this)->_context.leave();
		}
};

// void f(FV feedValue)
template <typename FV, typename IMPL>
class CoroutineBase<void, FV, IMPL>
{
	public:
		void operator ()(FV fval)
		{
			_fv = &fval;
			static_cast<IMPL>(this)->_context.enter();
		}

	protected:
		static void bootstrap_trampoline(void* self) {
			reinterpret_cast<CoroutineBase*>(self)->bootstrap();
		}
		
	private:
		FV* _fv;

		void bootstrap()
		{
			Yielder<void, FV> yielder(&yield_trampoline, this);
			yield(static_cast<IMPL>(this)->_func(yielder, *_fv));
			abort();
		}
	
		static FV yield_trampoline(void* self) {
			return reinterpret_cast<CoroutineBase*>(self)->yield();
		}
		
		FV yield()
		{
			static_cast<IMPL>(this)->_context.leave();
			return *_fv;
		}
};

// void f()
template <typename IMPL>
class CoroutineBase<void, void, IMPL>
{
	public:
		void operator ()()
		{
			static_cast<IMPL>(this)->_context.enter();
		}

	protected:
		static void bootstrap_trampoline(void* self) {
			reinterpret_cast<CoroutineBase*>(self)->bootstrap();
		}
		
	private:

		void bootstrap()
		{
			Yielder<void, void> yielder(&yield_trampoline, this);
			yield(static_cast<IMPL>(this)->_func(yielder));
			abort();
		}
	
		static void yield_trampoline(void* self) {
			reinterpret_cast<CoroutineBase*>(self)->yield();
		}
		
		void yield()
		{
			static_cast<IMPL>(this)->_context.leave();
		}
};

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
