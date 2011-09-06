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
#include <cstdlib>

namespace coroutine {
	namespace stack {

		struct dynamic: stack_tag {
			static const bool really_moveable = true;
		};

		template <size_t SSIZE>
			class stack<dynamic, SSIZE> {
				static const size_t size = SSIZE;

				public:
					stack(): _stack((char*)::malloc(size)) { }
					~stack() { ::free(_stack); }

					stack(const stack& from) = delete;
					stack& operator=(const stack& from) = delete;
					stack& operator=(stack&& from) = delete;

					stack(stack&& from): _stack(from._stack) {
						from._stack = 0;
					}

					static size_t get_size() { return size; }
					char* get_stack_ptr() { return _stack; }

				private:
					char* _stack;
			};

	} // namespace stack
} // namespace coroutine

#endif /* STACK_DYNAMIC_H */
