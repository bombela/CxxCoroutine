/*
 * context_oslinux.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef CONTEXT_OSLINUX_H
#define CONTEXT_OSLINUX_H

namespace coroutine {
namespace details {
namespace oslinux {

template <size_t STACK_SIZE, size_t PTR_SIZE>
class ContextImpl;

template <typename F, typename C>
inline void trampoline(C* context, F* f)
{
	std::cout << "___ " << f << " - " << context << std::endl;
	(*f)();
	std::cout << "awwww" << std::endl;
	context->swapContext();
	abort();
}

template <size_t STACK_SIZE = 8192>
class Context: public ContextImpl<STACK_SIZE, sizeof(void*)>
{
	public:
		template <typename F>
		Context(F& cb): ContextImpl<STACK_SIZE, sizeof(void*)>(cb) {}
};

} // namespace oslinux
} // namespace details
} // namespace coroutine

#include <details/context_oslinux_x86_64.hpp>

#endif /* CONTEXT_OSLINUX_H */
