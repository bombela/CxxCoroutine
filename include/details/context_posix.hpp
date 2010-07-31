/*
 * context_posix.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef CONTEXT_POSIX_H
#define CONTEXT_POSIX_H

namespace details {
namespace posix {

#include <ucontext.h>

template <typename F>
inline static void trampoline(F* f)
{
	(*f)();
}

template <size_t STACK_SIZE = SIGSTKSZ>
class Context
{
	public:
		
	private:
		ucontext _mainContext;
		ucontext _coroContext;
		char     _stack[STACK_SIZE];
};

} // namespace posix
} // namespace details

#endif /* CONTEXT_POSIX_H */
