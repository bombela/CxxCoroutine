/*
 * context_posix.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef CONTEXT_POSIX_H
#define CONTEXT_POSIX_H

#include <stdexcept>
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

template <class Stack = coroutine::stack::Default<> >
class Context
{
	public:
		typedef void (function_t)(void*);

		Context(function_t* f, void* arg):
			_f(f), _arg(arg)
		{
			reset();
		}

		Context(const Context& from):
			_f(from._f), _arg(from._arg)
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
				error(__PRETTY_FUNCTION__, "getcontext failed");
			_coroContext.uc_link          = &_mainContext;
			_coroContext.uc_stack.ss_sp   = _stack.getStackPointer();
			_coroContext.uc_stack.ss_size = _stack.getSize();
			makecontext(&_coroContext, (void (*)()) _f, 1, _arg);
		}

		void enter()
		{
			if (swapcontext(&_mainContext, &_coroContext) == -1)
				error(__PRETTY_FUNCTION__, "swapcontext failed");
		}

		void leave()
		{
			if (swapcontext(&_coroContext, &_mainContext) == -1)
				error(__PRETTY_FUNCTION__, "swapcontext failed");
		}

		static const char* getImplName() { return "posix"; }
		
	private:
		ucontext    _mainContext;
		ucontext    _coroContext;
		function_t* _f;
		void*       _arg;
		Stack       _stack;

		void error(const char* fname, const char* msg)
		{
			char buf[256];
			std::string errmsg = std::string(fname) + ": " + msg + ", ";

			if (strerror_r(errno, buf, sizeof buf) == 0)
			   errmsg += buf;
			else
			   errmsg += "unknown error";

			throw std::runtime_error(errmsg);
		}
};

} // namespace posix
} // namespace details
} // namespace coroutine

#endif /* CONTEXT_POSIX_H */
