/*
 * stack.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef STACK_H
#define STACK_H

#include <details/stack_static.hpp>
#include <details/stack_dynamic.hpp>

namespace coroutine {
namespace details {

namespace stack {

enum { DEFAULT_SIZE = 2048 };

} // namespace stack

template <template <size_t> class StackImpl = stack::Static,
		 size_t STACK_SIZE = stack::DEFAULT_SIZE>
class Stack: public StackImpl<STACK_SIZE>
{
	public:
		enum { SIZE = STACK_SIZE };
		size_t getSize()  const { return STACK_SIZE; }
};

} // namespace details
} // namespace coro

#endif /* STACK_H */
