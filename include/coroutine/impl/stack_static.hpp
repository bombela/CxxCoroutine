/*
 * stack_static.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef STACK_STATIC_H
#define STACK_STATIC_H

namespace coroutine {
	namespace stack {

		struct static_: stack_tag {};

		template <size_t SSIZE>
			class stack<static_, SSIZE> {
				static const size_t size = SSIZE;

				public:
					static size_t get_size() { return size; }
					char*  get_stack_ptr() { return _stack; }

				private:
					char _stack[STACK_SIZE];
			};

	} // namespace stack
} // namespace coroutine

#endif /* STACK_STATIC_H */
