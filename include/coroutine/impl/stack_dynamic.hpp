/*
 * stack_dynamic.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef STACK_DYNAMIC_H
#define STACK_DYNAMIC_H

namespace coroutine {
	namespace stack {

		struct dynamic: stack_tag {};

		template <size_t SSIZE>
			struct stack<dynamic, SSIZE> {
				static const size_t size = SSIZE;

				public:
					Dynamic(): _stack(new char [STACK_SIZE]) {}
					~Dynamic() { delete[] _stack; }
					Dynamic(const Dynamic& from) = delete;
					Dynamic& operator=(const Dynamic& from) = delete;

					static size_t get_size()  { return STACK_SIZE; }
					char*  get_stack_ptr() { return _stack; }

				private:
					char* _stack;
			};

	} // namespace stack
} // namespace coroutine

#endif /* STACK_DYNAMIC_H */
