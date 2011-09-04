/*
 * stack_dynamic.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef STACK_DYNAMIC_H
#define STACK_DYNAMIC_H

namespace coroutine {
namespace details {
namespace stack {

template <size_t STACK_SIZE>
class Dynamic
{
	public:
		Dynamic(): _stack(new char [STACK_SIZE]) {}
		~Dynamic() { delete[] _stack; }

		static size_t getSize()  { return STACK_SIZE; }
		char*  getStackPointer() { return _stack; }

	private:
		char* _stack;

		Dynamic(const Dynamic& from); // disabled
		Dynamic& operator=(const Dynamic& from); // disabled
};

} // namespace stack
} // namespace details
} // namespace coro

#endif /* STACK_DYNAMIC_H */
