/*
 * context_oslinux.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef CONTEXT_OSLINUX_H
#define CONTEXT_OSLINUX_H

#include <details/stack.hpp>

namespace coroutine {
namespace details {
namespace oslinux {

template <class Stack, size_t PTR_SIZE>
class ContextImpl;

// TODO Only use one ptr to the trampoline
template <typename C, typename F>
inline void trampoline(C* context, F* f)
{
	(*f)();
	context->swapContext();
	abort();
}

} // namespace oslinux
} // namespace details
} // namespace coroutine

#include <details/context_oslinux_x86_64.hpp>
#include <details/context_oslinux_x86_32.hpp>

namespace coroutine {
namespace details {
namespace oslinux {

template <template <size_t> class StackImpl = stack::Static,
		 size_t STACK_SIZE = stack::DEFAULT_SIZE>
class Context: public ContextImpl<Stack<StackImpl, STACK_SIZE>, sizeof(void*)>
{
	public:
		template <typename F>
		Context(F& cb):
			ContextImpl<Stack<StackImpl, STACK_SIZE>, sizeof(void*)>(cb) {}
};

} // namespace oslinux
} // namespace details
} // namespace coroutine

#endif /* CONTEXT_OSLINUX_H */
