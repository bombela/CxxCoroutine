/*
 * context_posix.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef CONTEXT_POSIX_H
#define CONTEXT_POSIX_H

#include <error.hpp>

namespace coroutine {
namespace details {
namespace posix {

#include <ucontext.h>

template <typename F>
inline static void trampoline(F* f)
{
	(*f)();
}

template <typename F, size_t STACK_SIZE = SIGSTKSZ>
class Context
{
	public:
		Context(const F& cb)
		{
			if (getcontext(&_coroContext) == -1)
				throw error::System("coroutine::details::ContextBase(): "
						"getcontext failed");
			_coroContext.uc_link = &_mainContext;
			_coroContext.uc_stack.ss_sp = &_stack;
			_coroContext.uc_stack.ss_size = sizeof _stack;
			void (*cb_ptr)(F*) = &trampoline<F>;
			makecontext(&_coroContext, (void (*)())cb_ptr, 1, &cb);
		}
		
	private:
		ucontext _mainContext;
		ucontext _coroContext;
		char     _stack[STACK_SIZE];
};

} // namespace posix
} // namespace details
} // namespace coroutine

#endif /* CONTEXT_POSIX_H */
