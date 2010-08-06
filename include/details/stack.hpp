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

template <template <size_t> class StackImpl = stack::Static,
		 size_t STACK_SIZE = 8192>
class Stack: public StackImpl<STACK_SIZE>
{
	public:
		
	private:
};

} // namespace details
} // namespace coro

#endif /* STACK_H */
