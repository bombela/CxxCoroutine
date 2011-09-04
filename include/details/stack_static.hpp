/*
 * stack_static.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef STACK_STATIC_H
#define STACK_STATIC_H

namespace coroutine {
namespace details {
namespace stack {

template <size_t STACK_SIZE>
class Static
{
	public:
		static size_t getSize()  { return STACK_SIZE; }
		char*  getStackPointer() { return _stack; }

	private:
		char _stack[STACK_SIZE];
};

} // namespace stack
} // namespace details
} // namespace coro

#endif /* STACK_STATIC_H */
