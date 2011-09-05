/*
 * stack.hpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef STACK_H
#define STACK_H

namespace coroutine {
	namespace stack {

		struct size_tag {};

		template <size_t V>
			struct size_in_b: size_tag {
				static const size_t value = V;
			};

		template <size_t V>
			struct size_in_kb: size_tag {
				static const size_t value = V * 1024;
			};

		template <size_t V>
			struct size_in_mb: size_tag {
				static const size_t value = V * 1024 * 1024;
			};

		template <typename T>
			struct is_size {
				static const bool value
					= details::is_base_of<size_tag, T>::value;
			};

		struct stack_tag {};

		template <typename TAG, size_t SSIZE>
			struct stack;

		template <typename T>
			struct is {
				static const bool value
					= details::is_base_of<stack_tag, T>::value;
			};

	} // namespace stack
} // namespace coroutine

#endif /* STACK_H */
