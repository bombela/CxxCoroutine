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
		typedef ContextImpl<Stack<StackImpl, STACK_SIZE>, sizeof(void*)> impl_t;

		template <typename F>
		Context(F cb): impl_t(cb) {}
};

} // namespace oslinux
} // namespace details
} // namespace coroutine

#endif /* CONTEXT_OSLINUX_H */
