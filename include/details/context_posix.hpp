/*
 * context_posix.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef CONTEXT_POSIX_H
#define CONTEXT_POSIX_H

#include <error.hpp>
#include <stack.hpp>

namespace coroutine {
namespace details {
namespace posix {

#include <ucontext.h>
#include <string.h>
#include <errno.h>

template <typename F>
inline void trampoline(F f)
{
	(*f)();
}

template <class Stack>
class Context
{
	public:
		Context(void (*f)(void*), void* arg):
			_f(f), _arg(arg)
		{
			reset();
		}

		Context(const Context& from):
			_f(from.f), _arg(from.arg)
		{
			reset();
		}

		Context& operator=(const Context& from)
		{
			_f   = from._f;
			_arg = from._arg;
			reset();
			return *this;
		}
		
		void reset()
		{
			if (getcontext(&_coroContext) == -1)
				throw createError(__PRETTY_FUNCTION__
						"getcontext failed");
			_coroContext.uc_link          = &_mainContext;
			_coroContext.uc_stack.ss_sp   = _stack.getStackPointer();
			_coroContext.uc_stack.ss_size = Stack::SIZE;
			makecontext(&_coroContext, _cbptr, 1, _funcptr);
		}

		void enter()
		{
			if (swapcontext(&_mainContext, &_coroContext) == -1)
				throw createError(__PRETTY_FUNCTION__
						"swapcontext failed");
		}

		void enter()
		{
			if (swapcontext(&_coroContext, &_mainContext) == -1)
				throw createError(__PRETTY_FUNCTION__
						"swapcontext failed");
		}

		static const char* getImplName() { return "posix"; }
		
	private:
		ucontext          _mainContext;
		ucontext          _coroContext;
		void (*_f)(void*) _f;
		void             *_arg;
		Stack             _stack;

		error::System createError(const char* msg)
		{
			char buf[256];
			if (strerror_r(errno, buf, sizeof buf) == 0)
				return error::System(std::string(msg) + ", " + buf);
			return error::System(std::string(msg) + ", unknown error");
		}
};

} // namespace posix
} // namespace details
} // namespace coroutine

#endif /* CONTEXT_POSIX_H */
