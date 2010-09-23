/*
 * context_posix.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef CONTEXT_POSIX_H
#define CONTEXT_POSIX_H

#include <error.hpp>
#include <details/stack.hpp>

namespace coroutine {
namespace details {
namespace posix {

#include <ucontext.h>
#include <string.h>
#include <errno.h>

template <typename F>
inline void trampoline(F* f)
{
	(*f)();
}

template <template <size_t> class StackImpl = stack::Static,
		 size_t STACK_SIZE = stack::DEFAULT_SIZE>
class Context
{
	typedef Stack<StackImpl, STACK_SIZE> stack_t;
	typedef void (callback_t)();

	public:
		template <typename F>
		Context(F& cb):
			_cbptr( (callback_t*) (void (*)(F*)) &trampoline<F>),
			_funcptr( (void*) &cb)
		{
			reset();
		}

		Context(const Context& from):
			_cbptr(from._cbptr),
			_funcptr(from._funcptr)
		{
			reset();
		}

		Context& operator=(const Context& from)
		{
			_cbptr = from._cbptr;
			_funcptr = from._funcptr;
			reset();
			return *this;
		}
		
		void reset()
		{
			if (getcontext(&_coroContext) == -1)
				throw createError("coroutine::details::ContextBase(), "
						"getcontext failed");
			_coroContext.uc_link = &_mainContext;
			_coroContext.uc_stack.ss_sp = _stack.getStack();
			_coroContext.uc_stack.ss_size = stack_t::SIZE;
			makecontext(&_coroContext, _cbptr, 1, _funcptr);
		}

		void run()
		{
			if (swapcontext(&_mainContext, &_coroContext) == -1)
				throw createError("coroutine::details::ContextBase::run(), "
						"swapcontext failed");
		}

		void yield()
		{
			if (swapcontext(&_coroContext, &_mainContext) == -1)
				throw createError("coroutine::details::ContextBase::yield(), "
						"swapcontext failed");
		}
		
	private:
		ucontext    _mainContext;
		ucontext    _coroContext;
		callback_t* _cbptr;
		void*       _funcptr;
		stack_t     _stack;

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
