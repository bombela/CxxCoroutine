/*
 * stack_dynamic.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef STACK_DYNAMIC_H
#define STACK_DYNAMIC_H

#include <algorithm>
#include <coroutine/stack.hpp>

#include <iostream>

namespace coroutine {
	namespace stack {

		struct dynamic: stack_tag {};

		template <size_t SSIZE>
			class stack<dynamic, SSIZE> {
				static const size_t size = SSIZE;

				public:
					stack(): _stack(new char [size]) {}
					~stack() { delete[] _stack; }

					stack(const stack& from) = delete;
					stack& operator=(const stack& from) = delete;

					stack(stack&& from): _stack(from._stack) {
						from._stack = 0;
					}

					stack& operator=(stack&& from) {
						swap(_stack, from._stack);
					};

					static size_t get_size()  { return size; }
					char*  get_stack_ptr() { return _stack; }

				private:
					char* _stack;
			};

	} // namespace stack
} // namespace coroutine

#endif /* STACK_DYNAMIC_H */
